// Copyright
#include <cstdio>
#include "city.h"  // NOLINT
#include "warrior.h"  // NOLINT
extern FILE* fp;
City::City(void):
    lifevalue(0), flag(0), last_combat(0), p_red_warrior(nullptr), p_blue_warrior(nullptr) {}

void City::lion_run_away(int hour) {
    if (p_red_warrior) {
        int run_away = p_red_warrior->run_away(hour);
        if (run_away)
            p_red_warrior = nullptr;
    }
    if (p_blue_warrior) {
        int run_away = p_blue_warrior->run_away(hour);
        if (run_away)
            p_blue_warrior = nullptr;
    }
}
void City::produce_elements(void) {
    lifevalue += 10;
}
void City::judge_flag(int hour, int city, int color) {
    if (color == 1 && last_combat == 1 && flag != 1) {
        flag = 1;
        fprintf(fp, "%03d:40 red flag raised in city %d\n", hour, city);
    } else if (color == 2 && last_combat == 2 && flag != 2) {
        flag = 2;
        fprintf(fp, "%03d:40 blue flag raised in city %d\n", hour, city);
    } else {
        last_combat = color;
    }
}
