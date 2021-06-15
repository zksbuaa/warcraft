// Copyright

#include <cstdio>
#include "headquarter.h"  // NOLINT
#include "warrior.h"  // NOLINT
#include "weapon.h" // NOLINT
extern FILE* fp;
// 司令部构造函数
Headquarter::Headquarter(int _color, int _lifevalue, int* _warrior_lifevalue, int* _warrior_attack, int _arrow_attack,
    int _loyalty_decrease) :
    color(_color), lifevalue(_lifevalue), arrow_attack(_arrow_attack), loyalty_decrease(_loyalty_decrease),
    p_enemy(nullptr) {
    warrior_number = 0;
    next_create = 0;
    if (color == 1) {
        create_order[0] = 2;
        create_order[1] = 3;
        create_order[2] = 4;
        create_order[3] = 1;
        create_order[4] = 0;
    } else {
        create_order[0] = 3;
        create_order[1] = 0;
        create_order[2] = 1;
        create_order[3] = 2;
        create_order[4] = 4;
    }
    for (int i = 0; i < 5; ++i) {
        warrior_lifevalue[i] = _warrior_lifevalue[i];
    }
    for (int i = 0; i < 5; ++i) {
        warrior_attack[i] = _warrior_attack[i];
    }
    p_warrior = nullptr;
    enemy_number = 0;
}

void Headquarter::create_warrior(int hour) {
    if (lifevalue < warrior_lifevalue[create_order[next_create]])
        return;
    int create_number = create_order[next_create];
    next_create = (next_create+1)%5;
    warrior_number++;
    lifevalue -= warrior_lifevalue[create_number];
    p_warrior = new_warrior(create_number);
    fprintf(fp, "%03d:00 %s %s %d born\n", hour, (color == 1)?"red":"blue", name_list[create_number], warrior_number);
    if (create_number == 0)
        fprintf(fp, "Its morale is %.2lf\n", (double)lifevalue/warrior_lifevalue[0]);  // NOLINT
    if (create_number == 3)
        fprintf(fp, "Its loyalty is %d\n", lifevalue);
}

Warrior* Headquarter::new_warrior(int create_number) {
    Warrior* p_new_warrior;
    switch (create_number) {
        case 0:
        {
            Dragon* p_dragon;
            p_dragon = new Dragon(color, warrior_number, warrior_lifevalue[0], warrior_attack[0],
            (double)lifevalue/warrior_lifevalue[0]);  // NOLINT
            switch (warrior_number%3) {
                case 0:
                {
                    if (p_dragon->attack*2/10)
                        p_dragon->p_sword = new Sword(warrior_attack[0]*2/10);
                    break;
                }
                case 1:
                {
                    p_dragon->p_bomb = new Bomb();
                    break;
                }
                case 2:
                {
                    p_dragon->p_arrow = new Arrow(arrow_attack);
                    break;
                }
            }
            p_new_warrior = p_dragon;
            break;
        }
        case 1:
        {
            Ninja* p_ninja;
            p_ninja = new Ninja(color, warrior_number, warrior_lifevalue[1], warrior_attack[1]);
            int hold_weapon[3] = {0, 0, 0};
            hold_weapon[warrior_number%3] = 1;
            hold_weapon[(warrior_number+1)%3] = 1;
            if (hold_weapon[0] && p_ninja ->attack*2/10)
                p_ninja->p_sword = new Sword(warrior_attack[1]*2/10);
            if (hold_weapon[1])
                p_ninja->p_bomb = new Bomb();
            if (hold_weapon[2])
                p_ninja->p_arrow = new Arrow(arrow_attack);
            p_new_warrior = p_ninja;
            break;
        }
        case 2:
        {
            Iceman* p_iceman;
            p_iceman = new Iceman(color, warrior_number, warrior_lifevalue[2], warrior_attack[2]);
            switch (warrior_number%3) {
                case 0:
                {
                    if (p_iceman->attack*2/10)
                        p_iceman->p_sword = new Sword(warrior_attack[2]*2/10);
                    break;
                }
                case 1:
                {
                    p_iceman->p_bomb = new Bomb();
                    break;
                }
                case 2:
                {
                    p_iceman->p_arrow = new Arrow(arrow_attack);
                    break;
                }
            }
            p_new_warrior = p_iceman;
            break;
        }
        case 3:
        {
            p_new_warrior = new Lion(color, warrior_number, warrior_lifevalue[3], warrior_attack[3], lifevalue,
            loyalty_decrease);
            break;
        }
        case 4:
        {
            p_new_warrior = new Wolf(color, warrior_number, warrior_lifevalue[4], warrior_attack[4]);
            break;
        }
    }
    return p_new_warrior;
}

void Headquarter::lion_run_away(int hour) {
    if (p_warrior == nullptr)
        return;
    int run_away = p_warrior->run_away(hour);
    if (run_away)
        p_warrior = nullptr;
}
