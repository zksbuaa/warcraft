// Copyright

#include <cstdio>
#include <vector>
#include "headquarter.h"  // NOLINT
#include "city.h"  // NOLINT
#include "warrior.h" // NOLINT
#include "function.h"  // NOLINT
using std::vector;
FILE* fp = fopen("output.txt", "w");
int main(void) {
    FILE* fpin = fopen("input.txt", "r");
    int total_case = 0;
    fscanf(fpin, "%d", &total_case);
    for (int case_number = 1; case_number <= total_case; ++case_number) {
        // 初始化时间
        int hour = 0;
        // 读入数据
        int lifevalue, city_number, arrow_attack, loyalty_decrease, end_time;
        fscanf(fpin, "%d%d%d%d%d", &lifevalue, &city_number, &arrow_attack, &loyalty_decrease, &end_time);
        int warrior_lifevalue[5], warrior_attack[5];
        for (int i = 0; i < 5; ++i)
            fscanf(fpin, "%d", warrior_lifevalue+i);
        for (int i = 0; i < 5; ++i)
            fscanf(fpin, "%d", warrior_attack+i);
        // 建造司令部和城市，初始化数据
        Headquarter red_headquarter(1, lifevalue, warrior_lifevalue, warrior_attack, arrow_attack, loyalty_decrease);
        Headquarter blue_headquarter(2, lifevalue, warrior_lifevalue, warrior_attack, arrow_attack, loyalty_decrease);
        vector<City> city_list(city_number, City());
        // 开战
        fprintf(fp, "Case %d:\n", case_number);
        while (1) {
            // 0分制造武士
            if (hour*60 > end_time)
                break;
            red_headquarter.create_warrior(hour);
            blue_headquarter.create_warrior(hour);
            // 5分lion逃跑
            if (hour*60+5 > end_time)
                break;
            red_headquarter.lion_run_away(hour);
            for (int i = 0; i < city_number; ++i)
                city_list[i].lion_run_away(hour);
            blue_headquarter.lion_run_away(hour);
            // 10分打印武士前进消息
            if (hour*60+10 > end_time)
                break;
            if (city_list[0].p_blue_warrior) {
                city_list[0].p_blue_warrior->reach_headquarter(hour);
                ++red_headquarter.enemy_number;
                if (red_headquarter.enemy_number == 2) {
                    fprintf(fp, "%03d:10 red headquarter was taken\n", hour);
                }
            }
            if (red_headquarter.p_warrior)
                red_headquarter.p_warrior->move_forward(hour, 1);
            if (city_number == 1) {
                if (blue_headquarter.p_warrior)
                    blue_headquarter.p_warrior->move_forward(hour, 1);
                if (city_list[0].p_red_warrior) {
                    city_list[0].p_red_warrior->reach_headquarter(hour);
                    ++blue_headquarter.enemy_number;
                    if (blue_headquarter.enemy_number == 2)
                        fprintf(fp, "%03d:10 blue headquarter was taken\n", hour);
                }
            } else {
                if (city_list[1].p_blue_warrior)
                    city_list[1].p_blue_warrior->move_forward(hour, 1);
                for (int i = 1; i < city_number - 1; ++i) {
                    if (city_list[i-1].p_red_warrior)
                        city_list[i-1].p_red_warrior->move_forward(hour, i+1);
                    if (city_list[i+1].p_blue_warrior)
                        city_list[i+1].p_blue_warrior->move_forward(hour, i+1);
                }
                if (city_list[city_number-2].p_red_warrior)
                    city_list[city_number-2].p_red_warrior->move_forward(hour, city_number);
                if (blue_headquarter.p_warrior)
                    blue_headquarter.p_warrior->move_forward(hour, city_number);
                if (city_list[city_number-1].p_red_warrior) {
                    city_list[city_number-1].p_red_warrior->reach_headquarter(hour);
                    ++blue_headquarter.enemy_number;
                    if (blue_headquarter.enemy_number == 2)
                        fprintf(fp, "%03d:10 blue headquarter was taken\n", hour);
                }
            }
            // 10分武士前进
            if (city_list[city_number-1].p_red_warrior)
                blue_headquarter.p_enemy = city_list[city_number-1].p_red_warrior;
            for (int i = city_number-1; i >= 1; --i) {
                city_list[i].p_red_warrior = nullptr;
                if (city_list[i-1].p_red_warrior)
                    city_list[i].p_red_warrior = city_list[i-1].p_red_warrior;
            }
            city_list[0].p_red_warrior = nullptr;
            if (red_headquarter.p_warrior)
                city_list[0].p_red_warrior = red_headquarter.p_warrior;
            red_headquarter.p_warrior = nullptr;
            if (city_list[0].p_blue_warrior)
                red_headquarter.p_enemy = city_list[0].p_blue_warrior;
            for (int i = 0; i < city_number-1; i++) {
                city_list[i].p_blue_warrior = nullptr;
                if (city_list[i+1].p_blue_warrior)
                    city_list[i].p_blue_warrior = city_list[i+1].p_blue_warrior;
            }
            city_list[city_number-1].p_blue_warrior = nullptr;
            if (blue_headquarter.p_warrior)
                city_list[city_number-1].p_blue_warrior = blue_headquarter.p_warrior;
            blue_headquarter.p_warrior = nullptr;
            // 判断游戏是否结束
            if (red_headquarter.enemy_number == 2 || blue_headquarter.enemy_number == 2)
                break;
            // 20分城市产出生命元
            if (hour*60+20 > end_time)
                break;
            for (int i = 0; i< city_number; ++i)
                city_list[i].produce_elements();
            // 30分武士不通过战斗取走生命元
            if (hour*60+30 > end_time)
                break;
            for (int i = 0; i < city_number; ++i) {
                if (city_list[i].p_red_warrior != nullptr && city_list[i].p_blue_warrior == nullptr) {
                    city_list[i].p_red_warrior->earn_elements_without_combat(hour, city_list[i].lifevalue);
                    red_headquarter.lifevalue += city_list[i].lifevalue;
                    city_list[i].lifevalue = 0;
                }
                if (city_list[i].p_red_warrior == nullptr && city_list[i].p_blue_warrior != nullptr) {
                    city_list[i].p_blue_warrior->earn_elements_without_combat(hour, city_list[i].lifevalue);
                    blue_headquarter.lifevalue += city_list[i].lifevalue;
                    city_list[i].lifevalue = 0;
                }
            }
            // 35分放箭
            if (hour*60+35 > end_time)
                break;
            shot_arrow(hour, city_number, city_list);
            for (int i = 0; i < city_number; ++i) {
                if (city_list[i].p_red_warrior && !city_list[i].p_blue_warrior &&
                !city_list[i].p_red_warrior->lifevalue)
                    city_list[i].p_red_warrior = nullptr;
                if (city_list[i].p_blue_warrior && !city_list[i].p_red_warrior &&
                !city_list[i].p_blue_warrior->lifevalue)
                    city_list[i].p_blue_warrior = nullptr;
            }
            // 38分使用炸弹
            if (hour*60+38 > end_time)
                break;
            bomb_explode(hour, city_list, city_number);
            // 40分武士战斗
            if (hour*60+40 > end_time)
                break;
            fight(hour, city_list, city_number, red_headquarter, blue_headquarter);
            // 50分司令部报告生命元数量
            if (60*hour+50 > end_time)
                break;
            fprintf(fp, "%03d:50 %d elements in red headquarter\n", hour, red_headquarter.lifevalue);
            fprintf(fp, "%03d:50 %d elements in blue headquarter\n", hour, blue_headquarter.lifevalue);
            // 55分武士报告武器情况
            if (hour*60+55 > end_time)
                break;
            for (int i = 0; i < city_number; ++i) {
                if (city_list[i].p_red_warrior)
                    city_list[i].p_red_warrior->report_weapon(hour);
            }
            if (blue_headquarter.p_enemy)
                blue_headquarter.p_enemy->report_weapon(hour);
            if (red_headquarter.p_enemy)
                red_headquarter.p_enemy->report_weapon(hour);
            for (int i = 0; i < city_number; ++i) {
                if (city_list[i].p_blue_warrior)
                    city_list[i].p_blue_warrior->report_weapon(hour);
            }
            ++hour;
        }
    }
    fclose(fp);
    fclose(fpin);
}
