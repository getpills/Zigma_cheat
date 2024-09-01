#pragma once

#include "../wh/draw.h"
#include "../src/include.h"

std::chrono::time_point<std::chrono::high_resolution_clock> startTimer() {
    return std::chrono::high_resolution_clock::now();
}
double getElapsedTime(std::chrono::time_point<std::chrono::high_resolution_clock> startTime) {
    auto endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(endTime - startTime).count();
}

void bomb_timer2() {
    bool isC4Planted = driver::read_memory<bool>(driv, client + client_dll::dwPlantedC4 - 0x8);
    if (isC4Planted && getElapsedTime(bomb::delay_timer) > 10000) {
        if (!bomb::timer_exists) {

            bomb::timer = startTimer();
            bomb::timer_exists = 1;
        }

        double time_left = 40 - getElapsedTime(bomb::timer) / 1000;
        if (time_left <= 0) {
            bomb::delay_timer = startTimer();
        }
        if (time_left < 0.01)
            return;
        string stime_left = to_string(time_left);
        Draw::drawText(923, 100, stime_left);
    }
    else {
        bomb::timer_exists = 0;
    }

}
