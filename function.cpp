// Copyright
#include <cstdio>
#include "function.h"  // NOLINT
#include "warrior.h"  // NOLINT
#include "headquarter.h"  // NOLINT

extern FILE* fp;

void shot_arrow(int hour, int city_number, vector<City>& city_list) {  // NOLINT
    if (city_number == 1)
        return;
    // 打印消息
    if (city_list[0].p_red_warrior && city_list[0].p_red_warrior->able_to_shot() && city_list[1].p_blue_warrior) {
        city_list[0].p_red_warrior->shot_arrow(hour, city_list[1].p_blue_warrior);
    }
    for (int i = 1; i < city_number-1; ++i) {
        if (city_list[i].p_red_warrior && city_list[i].p_red_warrior->able_to_shot() && city_list[i+1].p_blue_warrior)
            city_list[i].p_red_warrior->shot_arrow(hour, city_list[i+1].p_blue_warrior);
        if (city_list[i].p_blue_warrior && city_list[i].p_blue_warrior->able_to_shot() && city_list[i-1].p_red_warrior)
            city_list[i].p_blue_warrior->shot_arrow(hour, city_list[i-1].p_red_warrior);
    }
    if (city_list[city_number-1].p_blue_warrior && city_list[city_number-1].p_blue_warrior->able_to_shot() &&
    city_list[city_number-2].p_red_warrior)
        city_list[city_number-1].p_blue_warrior->shot_arrow(hour, city_list[city_number-2].p_red_warrior);
}

void bomb_explode(int hour, vector<City>& city_list, int city_number) {  // NOLINT
    for (int i = 0; i < city_number; ++i) {
        if (city_list[i].p_red_warrior && city_list[i].p_red_warrior->lifevalue > 0 && city_list[i].p_blue_warrior
        && city_list[i].p_blue_warrior->lifevalue > 0) {
            int order = 0;
            if (city_list[i].flag == 1)
                order = 1;
            else if (city_list[i].flag == 2)
                order = 2;
            else
                order = (-1)*((i+1)%2)+2;
            if (city_list[i].p_red_warrior->have_bomb() &&
            city_list[i].p_red_warrior->judge_bomb(city_list[i].p_blue_warrior, order)) {
                city_list[i].p_red_warrior->explode_bomb(hour, city_list[i].p_blue_warrior);
                city_list[i].p_red_warrior = nullptr;
                city_list[i].p_blue_warrior = nullptr;
            } else if (city_list[i].p_blue_warrior->have_bomb() &&
            city_list[i].p_blue_warrior->judge_bomb(city_list[i].p_red_warrior, order)) {
                city_list[i].p_blue_warrior->explode_bomb(hour, city_list[i].p_red_warrior);
                city_list[i].p_blue_warrior = nullptr;
                city_list[i].p_red_warrior = nullptr;
            }
        }
    }
}

void fight(int hour, vector<City>& city_list, int city_number, Headquarter& red_headquarter, Headquarter& blue_headquarter) {  // NOLINT
    int kill_enemy[city_number];  // NOLINT
    for (int i = 0; i < city_number; ++i)
        kill_enemy[i] = 3;
    for (int i = 0; i < city_number; ++i) {
        if (city_list[i].p_red_warrior && city_list[i].p_blue_warrior) {
            if (city_list[i].p_red_warrior->lifevalue == 0 && city_list[i].p_blue_warrior->lifevalue == 0) {
                city_list[i].p_red_warrior = nullptr;
                city_list[i].p_blue_warrior = nullptr;
            } else if (city_list[i].p_red_warrior->lifevalue !=0 && city_list[i].p_blue_warrior->lifevalue == 0) {
                // 红方不战而胜
                // wolf捡武器
                city_list[i].p_red_warrior->pick_up_weapon(city_list[i].p_blue_warrior);
                // dragon欢呼
                city_list[i].p_red_warrior->increase_morale();
                if ((city_list[i].flag == 1) || (city_list[i].flag == 0 && i%2 == 0)) {
                    city_list[i].p_red_warrior->yell(hour, i+1);
                }
                // 夺取生命元
                city_list[i].p_red_warrior->earn_elements(hour, city_list[i].lifevalue);
                kill_enemy[i] = 1;
                // 处理旗帜
                city_list[i].judge_flag(hour, i+1, 1);
                city_list[i].p_blue_warrior = nullptr;
            } else if (city_list[i].p_blue_warrior->lifevalue != 0 && city_list[i].p_red_warrior->lifevalue  == 0) {
                city_list[i].p_blue_warrior->pick_up_weapon(city_list[i].p_red_warrior);
                city_list[i].p_blue_warrior->increase_morale();
                if ((city_list[i].flag == 2) || (city_list[i].flag == 0 && i%2 == 1))
                    city_list[i].p_blue_warrior->yell(hour, i+1);
                city_list[i].p_blue_warrior->earn_elements(hour, city_list[i].lifevalue);
                kill_enemy[i] = 2;
                city_list[i].judge_flag(hour, i+1, 2);
                city_list[i].p_red_warrior = nullptr;
            } else {
                // 红方先手
                if ((city_list[i].flag == 1) || (city_list[i].flag == 0 && i%2 == 0)) {
                    int res;
                    res = city_list[i].p_red_warrior->active_attack(hour, i+1, city_list[i].p_blue_warrior);
                    if (res == 1) {
                        city_list[i].p_blue_warrior = nullptr;
                        city_list[i].p_red_warrior->earn_elements(hour, city_list[i].lifevalue);
                    } else if (res == 2) {
                        city_list[i].p_red_warrior = nullptr;
                        city_list[i].p_blue_warrior->earn_elements(hour, city_list[i].lifevalue);
                    }
                    kill_enemy[i] = res;
                    city_list[i].judge_flag(hour, i+1, res);
                } else {
                    int res;
                    res = city_list[i].p_blue_warrior->active_attack(hour, i+1, city_list[i].p_red_warrior);
                    if (res == 1) {
                        city_list[i].p_blue_warrior = nullptr;
                        city_list[i].p_red_warrior->earn_elements(hour, city_list[i].lifevalue);
                    } else if (res == 2) {
                        city_list[i].p_red_warrior = nullptr;
                        city_list[i].p_blue_warrior->earn_elements(hour, city_list[i].lifevalue);
                    }
                    kill_enemy[i] = res;
                    city_list[i].judge_flag(hour, i+1, res);
                }
            }
        }
    }
    for (int i = city_number-1 ; i >= 0 ; --i) {
        if (kill_enemy[i] == 1 && red_headquarter.lifevalue >= 8) {
            red_headquarter.lifevalue -= 8;
            city_list[i].p_red_warrior->lifevalue += 8;
        }
    }
    for (int i = 0; i < city_number; ++i) {
        if (kill_enemy[i] == 2 && blue_headquarter.lifevalue >= 8) {
            blue_headquarter.lifevalue -= 8;
            city_list[i].p_blue_warrior->lifevalue += 8;
        }
    }
    for (int i = 0; i < city_number; ++i) {
        if (kill_enemy[i] == 1) {
            red_headquarter.lifevalue += city_list[i].lifevalue;
            city_list[i].lifevalue = 0;
        } else if (kill_enemy[i] == 2) {
            blue_headquarter.lifevalue += city_list[i].lifevalue;
            city_list[i].lifevalue = 0;
        }
    }
}
