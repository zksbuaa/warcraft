// Copyright
#ifndef WARRIOR_H  // NOLINT
#define WARRIOR_H

class Headquarter;
class Sword;
class Bomb;
class Arrow;

class Warrior{
 public:
    int color = 0;
    int number = 0;
    int lifevalue = 0;
    int attack = 0;
    Warrior(void) {}
    Warrior(int, int, int, int);
    virtual int run_away(int) = 0;
    virtual void move_forward(int, int) = 0;
    virtual void reach_headquarter(int) = 0;
    virtual void earn_elements_without_combat(int, int) = 0;
    virtual bool able_to_shot(void) = 0;
    virtual void shot_arrow(int, Warrior*) = 0;
    virtual const char* get_name(void) = 0;
    virtual int first_attack(void) = 0;
    virtual int second_attack(void) = 0;
    virtual bool judge_bomb(Warrior*, int) = 0;
    virtual void explode_bomb(int, Warrior*) = 0;
    virtual bool have_bomb(void) = 0;
    virtual void pick_up_weapon(Warrior*) = 0;
    virtual Sword* return_sword(void) = 0;
    virtual Bomb* return_bomb(void) = 0;
    virtual Arrow* return_arrow(void) = 0;
    virtual void increase_morale(void) = 0;
    virtual void yell(int, int) = 0;
    virtual int active_attack(int, int, Warrior*) = 0;
    virtual int fight_back(int, int, Warrior*) = 0;
    virtual int now_lifevalue(void) = 0;
    virtual void decrease_morale(void) = 0;
    virtual void decrease_loyalty(void) = 0;
    virtual void earn_elements(int, int) = 0;
    virtual void report_weapon(int) = 0;
};



class Dragon:public Warrior{
 private:
    const char* name;
    double morale;
    Sword* p_sword;
    Arrow* p_arrow;
    Bomb* p_bomb;

 public:
    Dragon(void) {}
    Dragon(int, int, int, int, double);
    virtual int run_away(int) {return 0;}
    virtual void move_forward(int, int);
    virtual void reach_headquarter(int);
    virtual void earn_elements_without_combat(int, int);
    virtual bool able_to_shot(void);
    virtual void shot_arrow(int, Warrior*);
    virtual const char* get_name(void);
    virtual bool judge_bomb(Warrior*, int);
    virtual int first_attack(void);
    virtual int second_attack(void);
    virtual void explode_bomb(int, Warrior*);
    virtual bool have_bomb(void);
    virtual void pick_up_weapon(Warrior*);
    virtual Sword* return_sword(void);
    virtual Bomb* return_bomb(void);
    virtual Arrow* return_arrow(void);
    virtual void increase_morale(void);
    virtual void yell(int, int);
    virtual int active_attack(int, int, Warrior*);
    virtual int fight_back(int, int, Warrior*);
    virtual int now_lifevalue(void);
    virtual void decrease_morale(void);
    virtual void decrease_loyalty(void);
    virtual void earn_elements(int, int);
    virtual void report_weapon(int);
    friend class Headquarter;
};

class Ninja:public Warrior{
 private:
    Sword* p_sword;
    Arrow* p_arrow;
    Bomb* p_bomb;

 public:
    const char* name;
    Ninja(void) {}
    Ninja(int, int, int, int);
    virtual int run_away(int) {return 0;}
    virtual void move_forward(int, int);
    virtual void reach_headquarter(int);
    virtual void earn_elements_without_combat(int, int);
    virtual bool able_to_shot(void);
    virtual void shot_arrow(int, Warrior*);
    virtual const char* get_name(void);
    virtual bool judge_bomb(Warrior*, int);
    virtual int first_attack(void);
    virtual int second_attack(void);
    virtual void explode_bomb(int, Warrior*);
    virtual bool have_bomb(void);
    virtual void pick_up_weapon(Warrior*);
    virtual Sword* return_sword(void);
    virtual Bomb* return_bomb(void);
    virtual Arrow* return_arrow(void);
    virtual void increase_morale(void);
    virtual void yell(int, int);
    virtual int active_attack(int, int, Warrior*);
    virtual int fight_back(int, int, Warrior*);
    virtual int now_lifevalue(void);
    virtual void decrease_morale(void);
    virtual void decrease_loyalty(void);
    virtual void earn_elements(int, int);
    virtual void report_weapon(int);
    friend class Headquarter;
};

class Iceman:public Warrior{
 private:
    Sword* p_sword;
    Arrow* p_arrow;
    Bomb* p_bomb;
    int step;

 public:
    const char* name;
    Iceman(void) {}
    Iceman(int, int, int, int);
    virtual int run_away(int) {return 0;}
    virtual void move_forward(int, int);
    virtual void reach_headquarter(int);
    virtual void earn_elements_without_combat(int, int);
    virtual bool able_to_shot(void);
    virtual void shot_arrow(int, Warrior*);
    virtual const char* get_name(void);
    virtual bool judge_bomb(Warrior*, int);
    virtual int first_attack(void);
    virtual int second_attack(void);
    virtual void explode_bomb(int, Warrior*);
    virtual bool have_bomb(void);
    virtual void pick_up_weapon(Warrior*);
    virtual Sword* return_sword(void);
    virtual Bomb* return_bomb(void);
    virtual Arrow* return_arrow(void);
    virtual void increase_morale(void);
    virtual void yell(int, int);
    virtual int active_attack(int, int, Warrior*);
    virtual int fight_back(int, int, Warrior*);
    virtual int now_lifevalue(void);
    virtual void decrease_morale(void);
    virtual void decrease_loyalty(void);
    virtual void earn_elements(int, int);
    virtual void report_weapon(int);
    friend class Headquarter;
};

class Lion:public Warrior{
 private:
    int loyalty;
    int loyalty_decrease;

 public:
    const char* name;
    Lion(void) {}
    Lion(int, int, int, int, int, int);
    virtual int run_away(int);
    virtual void move_forward(int, int);
    virtual void reach_headquarter(int);
    virtual void earn_elements_without_combat(int, int);
    virtual bool able_to_shot(void);
    virtual void shot_arrow(int, Warrior*);
    virtual const char* get_name(void);
    virtual bool judge_bomb(Warrior*, int);
    virtual int first_attack(void);
    virtual int second_attack(void);
    virtual void explode_bomb(int, Warrior*);
    virtual bool have_bomb(void);
    virtual void pick_up_weapon(Warrior*);
    virtual Sword* return_sword(void);
    virtual Bomb* return_bomb(void);
    virtual Arrow* return_arrow(void);
    virtual void increase_morale(void);
    virtual void yell(int, int);
    virtual int active_attack(int, int, Warrior*);
    virtual int fight_back(int, int, Warrior*);
    virtual int now_lifevalue(void);
    virtual void decrease_morale(void);
    virtual void decrease_loyalty(void);
    virtual void earn_elements(int, int);
    virtual void report_weapon(int);
    friend class Headquarter;
};

class Wolf:public Warrior{
 private:
    Sword* p_sword;
    Arrow* p_arrow;
    Bomb* p_bomb;

 public:
    const char* name;
    Wolf(void) {}
    Wolf(int, int, int, int);
    virtual int run_away(int) {return 0;}
    virtual void move_forward(int, int);
    virtual void reach_headquarter(int);
    virtual void earn_elements_without_combat(int, int);
    virtual bool able_to_shot(void);
    virtual void shot_arrow(int, Warrior*);
    virtual const char* get_name(void);
    virtual bool judge_bomb(Warrior*, int);
    virtual int first_attack(void);
    virtual int second_attack(void);
    virtual void explode_bomb(int, Warrior*);
    virtual bool have_bomb(void);
    virtual void pick_up_weapon(Warrior*);
    virtual Sword* return_sword(void);
    virtual Bomb* return_bomb(void);
    virtual Arrow* return_arrow(void);
    virtual void increase_morale(void);
    virtual void yell(int, int);
    virtual int active_attack(int, int, Warrior*);
    virtual int fight_back(int, int, Warrior*);
    virtual int now_lifevalue(void);
    virtual void decrease_morale(void);
    virtual void decrease_loyalty(void);
    virtual void earn_elements(int, int);
    virtual void report_weapon(int);
    friend class Headquarter;
};


#endif  // WARRIOR_H  // NOLINT
