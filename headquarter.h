// Copyright
#ifndef HEADQUARTER_H  // NOLINT
#define HEADQUARTER_H

class Warrior;

const char name_list[5][10] = {"dragon", "ninja", "iceman", "lion", "wolf"};
const char weapon_list[3][10] = {"sword", "bomb", "arrow"};

class Headquarter{
 private:
    int color;
    int warrior_number;
    int next_create;
    int create_order[5];
    int warrior_lifevalue[5];
    int warrior_attack[5];
    int arrow_attack;
    int loyalty_decrease;
 public:
    int lifevalue;
    Warrior* p_warrior;
    Warrior* p_enemy;
    int enemy_number;
    Headquarter(void) {}
    Headquarter(int, int, int*, int*, int, int);
    void create_warrior(int);
    Warrior* new_warrior(int);
    void lion_run_away(int);
};


#endif  // HEADQUARTER_H // NOLINT
