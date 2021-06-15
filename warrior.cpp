// Copyright
#include <cstdio>
#include "warrior.h"  // NOLINT
#include "weapon.h"  // NOLINT 
extern FILE* fp;
Warrior::Warrior(int _color, int _number, int _lifevalue, int _attack):
    color(_color), number(_number), lifevalue(_lifevalue), attack(_attack) {}

Dragon::Dragon(int _color, int _number, int _lifevalue, int _attack, double _morale):
    Warrior(_color, _number, _lifevalue, _attack), morale(_morale), p_sword(nullptr),
    p_arrow(nullptr), p_bomb(nullptr), name("dragon") {}

void Dragon::move_forward(int hour, int destination) {
    fprintf(fp, "%03d:10 %s dragon %d marched to city %d with %d elements and force %d\n",
    hour, (color == 1)?"red":"blue", number, destination, lifevalue, attack);
}
void Dragon::reach_headquarter(int hour) {
    fprintf(fp, "%03d:10 %s dragon %d reached %s headquarter with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", number, (color == 1)?"blue":"red", lifevalue, attack);
}
void Dragon::earn_elements_without_combat(int hour, int elements) {
    fprintf(fp, "%03d:30 %s dragon %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue",
    number, elements);
}
bool Dragon::able_to_shot(void) {
    if (p_arrow && p_arrow->left_arrow)
        return true;
    return false;
}
void Dragon::shot_arrow(int hour, Warrior* p_shooted) {
    if (p_arrow->attack >= p_shooted->lifevalue) {
        fprintf(fp, "%03d:35 %s dragon %d shot and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
        (color == 1)?"blue":"red", p_shooted->get_name(), p_shooted->number);
        p_shooted->lifevalue = 0;
    } else {
        fprintf(fp, "%03d:35 %s dragon %d shot\n", hour, (color == 1)?"red":"blue", number);
        p_shooted->lifevalue -= p_arrow->attack;
    }
    --p_arrow->left_arrow;
    if (p_arrow->left_arrow == 0)
        p_arrow = nullptr;
}
const char* Dragon::get_name(void) {
    return name;
}
bool Dragon::judge_bomb(Warrior* p_enemy, int order) {
    if (color == order) {  // 先手
        if (first_attack() < p_enemy->lifevalue && p_enemy->second_attack() >= lifevalue) {
            return true;
        }
        return false;
    } else {
        if (p_enemy->first_attack() >= lifevalue)
            return true;
        return false;
    }
}
int Dragon::first_attack(void) {
    if (p_sword)
        return attack+p_sword->attack;
    return attack;
}
int Dragon::second_attack(void) {
    if (p_sword)
        return attack/2+p_sword->attack;
    return attack/2;
}
void Dragon::explode_bomb(int hour, Warrior* p_enemy) {
    fprintf(fp, "%03d:38 %s dragon %d used a bomb and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
    (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number);
}
bool Dragon::have_bomb(void) {
    if (p_bomb)
        return true;
    return false;
}
void Dragon::pick_up_weapon(Warrior* p_enemy) {
    return;
}
Sword* Dragon::return_sword(void) {
    return p_sword;
}
Bomb* Dragon::return_bomb(void) {
    return p_bomb;
}
Arrow* Dragon::return_arrow(void) {
    return p_arrow;
}
void Dragon::increase_morale(void) {
    morale += 0.2;
}
void Dragon::yell(int hour, int city) {
    if (morale > 0.8)
        fprintf(fp, "%03d:40 %s dragon %d yelled in city %d\n", hour, (color == 1)?"red":"blue", number, city);
}
int Dragon::active_attack(int hour, int city, Warrior* p_enemy) {
    int now_lifevalue = p_enemy->now_lifevalue();
    fprintf(fp, "%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(),
    p_enemy->number, city, lifevalue, attack);
    // 对方掉血
    if (first_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= first_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        increase_morale();
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        yell(hour, city);
        return color;
    } else {
        decrease_morale();
        int res = 0;
        res = p_enemy->fight_back(hour, city, this);
        if (res == 0)
            yell(hour, city);
        return res;
    }
}
int Dragon::fight_back(int hour, int city, Warrior* p_enemy) {
    fprintf(fp, "%03d:40 %s %s %d fought back against %s %s %d in city %d\n", hour, (color == 1)?"red":"blue",
    get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number, city);
    int now_lifevalue = p_enemy->now_lifevalue();
    if (second_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= second_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        increase_morale();
        return color;
    } else {
        decrease_morale();
        return 0;
    }
}
int Dragon::now_lifevalue(void) {
    return 0;
}
void Dragon::decrease_morale(void) {
    morale -= 0.2;
}
void Dragon::decrease_loyalty(void) {
    return;
}
void Dragon::earn_elements(int hour, int elements) {
    fprintf(fp, "%03d:40 %s dragon %d earned %d elements for his headquarter\n", hour,
    (color == 1)?"red":"blue", number, elements);
}
void Dragon::report_weapon(int hour) {
    fprintf(fp, "%03d:55 %s dragon %d has ", hour, (color == 1)?"red":"blue", number);
    if (p_sword == nullptr && p_bomb == nullptr && p_arrow == nullptr)
        fprintf(fp, "no weapon\n");
    if (p_arrow && !p_bomb && !p_sword)
        fprintf(fp, "arrow(%d)\n", p_arrow->left_arrow);
    if (!p_arrow && p_bomb && !p_sword)
        fprintf(fp, "bomb\n");
    if (!p_arrow && !p_bomb && p_sword)
        fprintf(fp, "sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && !p_sword)
        fprintf(fp, "arrow(%d),bomb\n", p_arrow->left_arrow);
    if (p_arrow && !p_bomb && p_sword)
        fprintf(fp, "arrow(%d),sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
    if (!p_arrow && p_bomb && p_sword)
        fprintf(fp, "bomb,sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && p_sword)
        fprintf(fp, "arrow(%d),bomb,sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
}

Ninja::Ninja(int _color, int _number, int _lifevalue, int _attack):
    Warrior(_color, _number, _lifevalue, _attack), p_arrow(nullptr), p_bomb(nullptr), p_sword(nullptr), name("ninja") {}

void Ninja::move_forward(int hour, int destination) {
    fprintf(fp, "%03d:10 %s ninja %d marched to city %d with %d elements and force %d\n",
    hour, (color == 1)?"red":"blue", number, destination, lifevalue, attack);
}
void Ninja::reach_headquarter(int hour) {
    fprintf(fp, "%03d:10 %s ninja %d reached %s headquarter with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", number, (color == 1)?"blue":"red", lifevalue, attack);
}
void Ninja::earn_elements_without_combat(int hour, int elements) {
    fprintf(fp, "%03d:30 %s ninja %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue",
    number, elements);
}
bool Ninja::able_to_shot(void) {
    if (p_arrow && p_arrow->left_arrow)
        return true;
    return false;
}
const char* Ninja::get_name(void) {
    return name;
}
void Ninja::shot_arrow(int hour, Warrior* p_shooted) {
    if (p_arrow->attack >= p_shooted->lifevalue) {
        fprintf(fp, "%03d:35 %s ninja %d shot and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
        (color == 1)?"blue":"red", p_shooted->get_name(), p_shooted->number);
        p_shooted->lifevalue = 0;
    } else {
        fprintf(fp, "%03d:35 %s ninja %d shot\n", hour, (color == 1)?"red":"blue", number);
        p_shooted->lifevalue -= p_arrow->attack;
    }
    --p_arrow->left_arrow;
    if (p_arrow->left_arrow == 0)
        p_arrow = nullptr;
}
int Ninja::first_attack(void) {
    if (p_sword)
        return attack+p_sword->attack;
    return attack;
}
int Ninja::second_attack(void) {
    return 0;
}
bool Ninja::judge_bomb(Warrior* p_enemy, int order) {
    if (color == order) {  // 先手
        if (first_attack() < p_enemy->lifevalue && p_enemy->second_attack() >= lifevalue) {
            return true;
        }
        return false;
    } else {
        if (p_enemy->first_attack() >= lifevalue)
            return true;
        return false;
    }
}
bool Ninja::have_bomb(void) {
    if (p_bomb)
        return true;
    return false;
}
void Ninja::explode_bomb(int hour, Warrior* p_enemy) {
    fprintf(fp, "%03d:38 %s ninja %d used a bomb and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
    (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number);
}
Sword* Ninja::return_sword(void) {
    return p_sword;
}
Bomb* Ninja::return_bomb(void) {
    return p_bomb;
}
Arrow* Ninja::return_arrow(void) {
    return p_arrow;
}
void Ninja::increase_morale(void) {
    return;
}
void Ninja::yell(int hour, int city) {
    return;
}
int Ninja::now_lifevalue(void) {
    return 0;
}
void Ninja::decrease_morale(void) {
    return;
}
int Ninja::active_attack(int hour, int city, Warrior* p_enemy) {
    int now_lifevalue = p_enemy->now_lifevalue();
    fprintf(fp, "%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(),
    p_enemy->number, city, lifevalue, attack);
    // 对方掉血
    if (first_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= first_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        increase_morale();
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        yell(hour, city);
        return color;
    } else {
        decrease_morale();
        int res = p_enemy->fight_back(hour, city, this);
        return res;
    }
}
int Ninja::fight_back(int hour, int city, Warrior* p_enemy) {
    return 0;
}
void Ninja::decrease_loyalty(void) {
    return;
}
void Ninja::earn_elements(int hour, int elements) {
    fprintf(fp, "%03d:40 %s ninja %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue", number,
    elements);
}
void Ninja::report_weapon(int hour) {
    fprintf(fp, "%03d:55 %s ninja %d has ", hour, (color == 1)?"red":"blue", number);
    if (p_sword == nullptr && p_bomb == nullptr && p_arrow == nullptr)
        fprintf(fp, "no weapon\n");
    if (p_arrow && !p_bomb && !p_sword)
        fprintf(fp, "arrow(%d)\n", p_arrow->left_arrow);
    if (!p_arrow && p_bomb && !p_sword)
        fprintf(fp, "bomb\n");
    if (!p_arrow && !p_bomb && p_sword)
        fprintf(fp, "sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && !p_sword)
        fprintf(fp, "arrow(%d),bomb\n", p_arrow->left_arrow);
    if (p_arrow && !p_bomb && p_sword)
        fprintf(fp, "arrow(%d),sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
    if (!p_arrow && p_bomb && p_sword)
        fprintf(fp, "bomb,sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && p_sword)
        fprintf(fp, "arrow(%d),bomb,sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
}
void Ninja::pick_up_weapon(Warrior* p_enemy) {
    return;
}
Iceman::Iceman(int _color, int _number, int _lifevalue, int _attack):
    Warrior(_color, _number, _lifevalue, _attack), p_arrow(nullptr), p_bomb(nullptr), p_sword(nullptr),
    step(0), name("iceman") {}

void Iceman::move_forward(int hour, int destination) {
    ++step;
    if (step == 2) {
        step = 0;
        attack += 20;
        (lifevalue > 9)?lifevalue -= 9:lifevalue = 1;
    }
    fprintf(fp, "%03d:10 %s iceman %d marched to city %d with %d elements and force %d\n",
    hour, (color == 1)?"red":"blue", number, destination, lifevalue, attack);
}
void Iceman::reach_headquarter(int hour) {
    ++step;
    if (step == 2) {
        step = 0;
        attack += 20;
        (lifevalue > 9)?lifevalue -= 9:lifevalue = 1;
    }
    fprintf(fp, "%03d:10 %s iceman %d reached %s headquarter with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", number, (color == 1)?"blue":"red", lifevalue, attack);
}
void Iceman::earn_elements_without_combat(int hour, int elements) {
    fprintf(fp, "%03d:30 %s iceman %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue",
    number, elements);
}
bool Iceman::able_to_shot(void) {
    if (p_arrow && p_arrow->left_arrow)
        return true;
    return false;
}
const char* Iceman::get_name(void) {
    return name;
}
void Iceman::shot_arrow(int hour, Warrior* p_shooted) {
    if (p_arrow->attack >= p_shooted->lifevalue) {
        fprintf(fp, "%03d:35 %s iceman %d shot and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
        (color == 1)?"blue":"red", p_shooted->get_name(), p_shooted->number);
        p_shooted->lifevalue = 0;
    } else {
        fprintf(fp, "%03d:35 %s iceman %d shot\n", hour, (color == 1)?"red":"blue", number);
        p_shooted->lifevalue -= p_arrow->attack;
    }
    --p_arrow->left_arrow;
    if (p_arrow->left_arrow == 0)
        p_arrow = nullptr;
}
int Iceman::first_attack(void) {
    if (p_sword)
        return attack+p_sword->attack;
    return attack;
}
int Iceman::second_attack(void) {
    if (p_sword)
        return attack/2+p_sword->attack;
    return attack/2;
}
bool Iceman::judge_bomb(Warrior* p_enemy, int order) {
    if (color == order) {  // 先手
        if (first_attack() < p_enemy->lifevalue && p_enemy->second_attack() >= lifevalue) {
            return true;
        }
        return false;
    } else {
        if (p_enemy->first_attack() >= lifevalue)
            return true;
        return false;
    }
}
bool Iceman::have_bomb(void) {
    if (p_bomb)
        return true;
    return false;
}
void Iceman::explode_bomb(int hour, Warrior* p_enemy) {
    fprintf(fp, "%03d:38 %s iceman %d used a bomb and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
    (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number);
}
void Iceman::pick_up_weapon(Warrior* p_enemy) {
    return;
}
Sword* Iceman::return_sword(void) {
    return p_sword;
}
Bomb* Iceman::return_bomb(void) {
    return p_bomb;
}
Arrow* Iceman::return_arrow(void) {
    return p_arrow;
}
void Iceman::increase_morale(void) {
    return;
}
void Iceman::yell(int hour, int city) {
    return;
}
int Iceman::now_lifevalue(void) {
    return 0;
}
void Iceman::decrease_morale(void) {
    return;
}
int Iceman::active_attack(int hour, int city, Warrior* p_enemy) {
    int now_lifevalue = p_enemy->now_lifevalue();
    fprintf(fp, "%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(),
    p_enemy->number, city, lifevalue, attack);
    // 对方掉血
    if (first_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= first_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        increase_morale();
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        yell(hour, city);
        return color;
    } else {
        decrease_morale();
        int res = p_enemy->fight_back(hour, city, this);
        return res;
    }
}
int Iceman::fight_back(int hour, int city, Warrior* p_enemy) {
    fprintf(fp, "%03d:40 %s %s %d fought back against %s %s %d in city %d\n", hour, (color == 1)?"red":"blue",
    get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number, city);
    int now_lifevalue = p_enemy->now_lifevalue();
    if (second_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= second_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        return color;
    } else {
        return 0;
    }
}
void Iceman::decrease_loyalty(void) {
    return;
}
void Iceman::earn_elements(int hour, int elements) {
    fprintf(fp, "%03d:40 %s iceman %d earned %d elements for his headquarter\n", hour,
    (color == 1)?"red":"blue", number, elements);
}
void Iceman::report_weapon(int hour) {
    fprintf(fp, "%03d:55 %s iceman %d has ", hour, (color == 1)?"red":"blue", number);
    if (p_sword == nullptr && p_bomb == nullptr && p_arrow == nullptr)
        fprintf(fp, "no weapon\n");
    if (p_arrow && !p_bomb && !p_sword)
        fprintf(fp, "arrow(%d)\n", p_arrow->left_arrow);
    if (!p_arrow && p_bomb && !p_sword)
        fprintf(fp, "bomb\n");
    if (!p_arrow && !p_bomb && p_sword)
        fprintf(fp, "sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && !p_sword)
        fprintf(fp, "arrow(%d),bomb\n", p_arrow->left_arrow);
    if (p_arrow && !p_bomb && p_sword)
        fprintf(fp, "arrow(%d),sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
    if (!p_arrow && p_bomb && p_sword)
        fprintf(fp, "bomb,sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && p_sword)
        fprintf(fp, "arrow(%d),bomb,sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
}
Lion::Lion(int _color, int _number, int _lifevalue, int _attack, int _loyalty, int _loyalty_decrease):
    Warrior(_color, _number, _lifevalue, _attack), loyalty(_loyalty), name("lion"),
    loyalty_decrease(_loyalty_decrease) {}

int Lion::run_away(int hour) {
    if (loyalty <= 0) {
        fprintf(fp, "%03d:05 %s lion %d ran away\n", hour, (color == 1)?"red":"blue", number);
        return 1;
    }
    return 0;
}

void Lion::move_forward(int hour, int destination) {
    fprintf(fp, "%03d:10 %s lion %d marched to city %d with %d elements and force %d\n",
    hour, (color == 1)?"red":"blue", number, destination, lifevalue, attack);
}
void Lion::reach_headquarter(int hour) {
    fprintf(fp, "%03d:10 %s lion %d reached %s headquarter with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", number, (color == 1)?"blue":"red", lifevalue, attack);
}
void Lion::earn_elements_without_combat(int hour, int elements) {
    fprintf(fp, "%03d:30 %s lion %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue",
    number, elements);
}
bool Lion::able_to_shot(void) {
    return false;
}
const char* Lion::get_name(void) {
    return name;
}
void Lion::shot_arrow(int hour, Warrior* p_shooted) {
    return;
}
int Lion::first_attack(void) {
    return attack;
}
int Lion::second_attack(void) {
    return attack/2;
}
bool Lion::have_bomb(void) {
    return false;
}
bool Lion::judge_bomb(Warrior* p_enemy, int order) {
    return false;
}
void Lion::explode_bomb(int hour, Warrior* p_enemy) {
    return;
}
void Lion::pick_up_weapon(Warrior* p_enemy) {
    return;
}
Sword* Lion::return_sword(void) {
    return nullptr;
}
Bomb* Lion::return_bomb(void) {
    return nullptr;
}
Arrow* Lion::return_arrow(void) {
    return nullptr;
}
void Lion::increase_morale(void) {
    return;
}
void Lion::yell(int hour, int city) {
    return;
}
int Lion::now_lifevalue(void) {
    return lifevalue;
}
void Lion::decrease_morale(void) {
    return;
}
int Lion::active_attack(int hour, int city, Warrior* p_enemy) {
    int now_lifevalue = p_enemy->now_lifevalue();
    fprintf(fp, "%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(),
    p_enemy->number, city, lifevalue, attack);
    // 对方掉血
    if (first_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= first_attack();
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        increase_morale();
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        yell(hour, city);
        return color;
    } else {
        decrease_loyalty();
        int res = p_enemy->fight_back(hour, city, this);
        return res;
    }
}
int Lion::fight_back(int hour, int city, Warrior* p_enemy) {
    fprintf(fp, "%03d:40 %s %s %d fought back against %s %s %d in city %d\n", hour, (color == 1)?"red":"blue",
    get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number, city);
    int now_lifevalue = p_enemy->now_lifevalue();
    if (second_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= second_attack();
    if (p_enemy->lifevalue == 0) {
        lifevalue += now_lifevalue;
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        return color;
    } else {
        decrease_loyalty();
        return 0;
    }
}
void Lion::decrease_loyalty(void) {
    loyalty -= loyalty_decrease;
}
void Lion::earn_elements(int hour, int elements) {
    fprintf(fp, "%03d:40 %s lion %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue", number,
    elements);
}
void Lion::report_weapon(int hour) {
    fprintf(fp, "%03d:55 %s lion %d has no weapon\n", hour, (color == 1)?"red":"blue", number);
}

Wolf::Wolf(int _color, int _number, int _lifevalue, int _attack):
    Warrior(_color, _number, _lifevalue, _attack), p_arrow(nullptr), p_bomb(nullptr), p_sword(nullptr), name("wolf") {}

void Wolf::move_forward(int hour, int destination) {
    fprintf(fp, "%03d:10 %s wolf %d marched to city %d with %d elements and force %d\n",
    hour, (color == 1)?"red":"blue", number, destination, lifevalue, attack);
}
void Wolf::reach_headquarter(int hour) {
    fprintf(fp, "%03d:10 %s wolf %d reached %s headquarter with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", number, (color == 1)?"blue":"red", lifevalue, attack);
}

void Wolf::earn_elements_without_combat(int hour, int elements) {
    fprintf(fp, "%03d:30 %s wolf %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue",
    number, elements);
}
bool Wolf::able_to_shot(void) {
    if (p_arrow && p_arrow->left_arrow)
        return true;
    return false;
}
const char* Wolf::get_name(void) {
    return name;
}
void Wolf::shot_arrow(int hour, Warrior* p_shooted) {
    if (p_arrow->attack >= p_shooted->lifevalue) {
        fprintf(fp, "%03d:35 %s wolf %d shot and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
        (color == 1)?"blue":"red", p_shooted->get_name(), p_shooted->number);
        p_shooted->lifevalue = 0;
    } else {
        fprintf(fp, "%03d:35 %s wolf %d shot\n", hour, (color == 1)?"red":"blue", number);
        p_shooted->lifevalue -= p_arrow->attack;
    }
    --p_arrow->left_arrow;
    if (p_arrow->left_arrow == 0)
        p_arrow = nullptr;
}
int Wolf::first_attack(void) {
    if (p_sword)
        return attack+p_sword->attack;
    return attack;
}
int Wolf::second_attack(void) {
    if (p_sword)
        return attack/2+p_sword->attack;
    return attack/2;
}
bool Wolf::have_bomb(void) {
    if (p_bomb)
        return true;
    return false;
}
bool Wolf::judge_bomb(Warrior* p_enemy, int order) {
    if (color == order) {  // 先手
        if (first_attack() < p_enemy->lifevalue && p_enemy->second_attack() >= lifevalue) {
            return true;
        }
        return false;
    } else {
        if (p_enemy->first_attack() >= lifevalue)
            return true;
        return false;
    }
}
void Wolf::explode_bomb(int hour, Warrior* p_enemy) {
    fprintf(fp, "%03d:38 %s wolf %d used a bomb and killed %s %s %d\n", hour, (color == 1)?"red":"blue", number,
    (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number);
}

void Wolf::pick_up_weapon(Warrior* p_enemy) {
    if (p_sword == nullptr && p_enemy->return_sword())
        p_sword = p_enemy->return_sword();
    if (p_bomb == nullptr && p_enemy->return_bomb())
        p_bomb = p_enemy->return_bomb();
    if (p_arrow == nullptr && p_enemy->return_arrow())
        p_arrow = p_enemy->return_arrow();
}
Sword* Wolf::return_sword(void) {
    return p_sword;
}
Bomb* Wolf::return_bomb(void) {
    return p_bomb;
}
Arrow* Wolf::return_arrow(void) {
    return p_arrow;
}
void Wolf::increase_morale(void) {
    return;
}
void Wolf::yell(int hour, int city) {
    return;
}
int Wolf::now_lifevalue(void) {
    return 0;
}
void Wolf::decrease_morale(void) {
    return;
}
int Wolf::active_attack(int hour, int city, Warrior* p_enemy) {
    int now_lifevalue = p_enemy->now_lifevalue();
    fprintf(fp, "%03d:40 %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n", hour,
    (color == 1)?"red":"blue", get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(),
    p_enemy->number, city, lifevalue, attack);
    // 对方掉血
    if (first_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= first_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        pick_up_weapon(p_enemy);
        lifevalue += now_lifevalue;
        increase_morale();
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        yell(hour, city);
        return color;
    } else {
        decrease_morale();
        int res = p_enemy->fight_back(hour, city, this);
        return res;
    }
}
int Wolf::fight_back(int hour, int city, Warrior* p_enemy) {
    fprintf(fp, "%03d:40 %s %s %d fought back against %s %s %d in city %d\n", hour, (color == 1)?"red":"blue",
    get_name(), number, (color == 1)?"blue":"red", p_enemy->get_name(), p_enemy->number, city);
    int now_lifevalue = p_enemy->now_lifevalue();
    if (second_attack() >= p_enemy->lifevalue)
        p_enemy->lifevalue = 0;
    else
        p_enemy->lifevalue -= second_attack();
    if (p_sword) {
        p_sword->attack = p_sword->attack*8/10;
        if (p_sword->attack == 0)
            p_sword = nullptr;
    }
    if (p_enemy->lifevalue == 0) {
        pick_up_weapon(p_enemy);
        lifevalue += now_lifevalue;
        fprintf(fp, "%03d:40 %s %s %d was killed in city %d\n", hour, (color == 1)?"blue":"red", p_enemy->get_name(),
        p_enemy->number, city);
        return color;
    } else {
        return 0;
    }
}
void Wolf::decrease_loyalty(void) {
    return;
}
void Wolf::earn_elements(int hour, int elements) {
    fprintf(fp, "%03d:40 %s wolf %d earned %d elements for his headquarter\n", hour, (color == 1)?"red":"blue", number,
    elements);
}
void Wolf::report_weapon(int hour) {
    fprintf(fp, "%03d:55 %s wolf %d has ", hour, (color == 1)?"red":"blue", number);
    if (p_sword == nullptr && p_bomb == nullptr && p_arrow == nullptr)
        fprintf(fp, "no weapon\n");
    if (p_arrow && !p_bomb && !p_sword)
        fprintf(fp, "arrow(%d)\n", p_arrow->left_arrow);
    if (!p_arrow && p_bomb && !p_sword)
        fprintf(fp, "bomb\n");
    if (!p_arrow && !p_bomb && p_sword)
        fprintf(fp, "sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && !p_sword)
        fprintf(fp, "arrow(%d),bomb\n", p_arrow->left_arrow);
    if (p_arrow && !p_bomb && p_sword)
        fprintf(fp, "arrow(%d),sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
    if (!p_arrow && p_bomb && p_sword)
        fprintf(fp, "bomb,sword(%d)\n", p_sword->attack);
    if (p_arrow && p_bomb && p_sword)
        fprintf(fp, "arrow(%d),bomb,sword(%d)\n", p_arrow->left_arrow, p_sword->attack);
}
