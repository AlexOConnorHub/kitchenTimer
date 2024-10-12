#include <Arduino.h>
#include <stdio.h>
#include "Debounce.h"
#include "Encoder.h"
#include "Screen.h"
#include "Timer.h"

#define ONBOARD_BUTTON PA0
#define BUTTON_PIN PA10
#define LED_PIN PC13
#define ENCODER_A PA12
#define ENCODER_B PA11

kitchenTimer::Debounce button = kitchenTimer::Debounce(BUTTON_PIN);
kitchenTimer::Timer timer = kitchenTimer::Timer();
kitchenTimer::Screen screen = kitchenTimer::Screen();
kitchenTimer::Encoder encoder(ENCODER_A, ENCODER_B);
enum TIMER_STATE {
    SELECT_TIME = 0,
    RUNNING = 1,
    ENDED = 2
};
enum SELECT_STATE {
    SECONDS = 0,
    MINUTES = 1,
    HOURS = 2
};

TIMER_STATE timerState = ENDED;
SELECT_STATE selectState = SECONDS;
int pos = 0;
unsigned long lastUserInteraction = 0;
bool beeping = false;
bool buttonWasReleased = false;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    screen.setup();
}

void loop() {
    const int PRINT_BUFFER_SIZE = 9;
    char printBuffer[PRINT_BUFFER_SIZE];
    int newPos = encoder.getValue();
    int buttonState = button.check();
    int newPress = buttonState == LOW && buttonWasReleased;
    int time = millis();

    if (buttonState == HIGH) {
        buttonWasReleased = true;
    }

    #pragma region Update state
    if (newPos != pos || newPress) {
        lastUserInteraction = time;
    }

    switch (timerState) {
        case TIMER_STATE::SELECT_TIME:
            if (time - lastUserInteraction > 5000) {
                if (timer.getTime() == 0) {
                    timerState = TIMER_STATE::ENDED;
                } else {
                    timerState = TIMER_STATE::RUNNING;
                    timer.start();
                }
            }
            {
                unsigned long currentTimer = timer.getTime(), impact;
                int currentSegment, newSegment;
                switch (selectState) {
                    case HOURS:
                        currentSegment = timer.getHours();
                        newSegment = (currentSegment + newPos - pos + 100) % 100;
                        impact = (newSegment - currentSegment) * 3600;
                        timer.set((currentTimer + impact));
                        break;
                    case MINUTES:
                        currentSegment = timer.getMinutes();
                        newSegment = (currentSegment + newPos - pos + 60) % 60;
                        impact = (newSegment - currentSegment) * 60;
                        timer.set((currentTimer + impact));
                        break;
                    case SECONDS:
                        currentSegment = timer.getSeconds();
                        newSegment = (currentSegment + newPos - pos + 60) % 60;
                        impact = newSegment - currentSegment;
                        timer.set((currentTimer + impact));
                        break;
                }
            }

            if (newPress) {
                selectState = (SELECT_STATE)((selectState + 1) % 3);
            }
            break;
        case TIMER_STATE::RUNNING:
            if (newPress) {
                switch (timer.state()) {
                    case kitchenTimer::Timer::RUNNING:
                        timer.stop();
                        break;
                    case kitchenTimer::Timer::STOPPED:
                        timer.start();
                        break;
                }
            }

            if (timer.getTime() <= 0) {
                timerState = TIMER_STATE::ENDED;
                beeping = true;
                timer.reset();
            }
            break;
        case TIMER_STATE::ENDED:
            if (newPress) {
                beeping = false;
            }
            break;
    }

    if (newPos != pos && timerState != TIMER_STATE::SELECT_TIME) {
        beeping = false; // There is a possibility that the timer is ended and the user is rotating the encoder
        timer.stop();
        unsigned long time = timer.getTime();
        timer.reset();
        timerState = TIMER_STATE::SELECT_TIME;
        selectState = SELECT_STATE::SECONDS;
        int hours = (time / 3600);
        if (hours > 99 || hours < 0) {
            hours = 0;
        }
        int minutes = ((int)time % 3600) / 60;
        if (minutes > 59 || minutes < 0) {
            minutes = 0;
        }
        int seconds = (int)time % 60;
        if (seconds > 59 || seconds < 0) {
            seconds = 0;
        }
        timer.set(hours, minutes, seconds);
    }

    if (newPress) {
        buttonWasReleased = false;
    }
    #pragma endregion

    #pragma region Update outputs
    screen.clear();
    switch (timerState) { // OLED
        case TIMER_STATE::SELECT_TIME:
            timer.getTimeStr(printBuffer, PRINT_BUFFER_SIZE, true);
            screen.underline(6 - selectState - (selectState * 2), 2);
            break;
        case TIMER_STATE::RUNNING:
            if (timer.state() == kitchenTimer::Timer::RUNNING) {
                timer.getTimeStr(printBuffer, PRINT_BUFFER_SIZE);
            } else {
                if (time / 1000 % 2) {
                    sprintf(printBuffer, "");
                } else {
                    timer.getTimeStr(printBuffer, PRINT_BUFFER_SIZE);
                }
            }
            break;
        case TIMER_STATE::ENDED:
            if (beeping) {
                sprintf(printBuffer, "");
            } else {
                sprintf(printBuffer, "TIMER");
            }
            break;
    }

    if (beeping && (millis() / 1000) % 2) {
        sprintf(printBuffer, "BEEP!");
        digitalWrite(LED_PIN, LOW);
    } else {
        digitalWrite(LED_PIN, HIGH);
    }


    pos = newPos;
    screen.print(printBuffer);
    screen.show();
    #pragma endregion
}
