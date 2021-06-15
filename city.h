// Copyright
#ifndef CITY_H  // NOLINT
#define CITY_H  // NOLINT


class Warrior;
class City{
 public:
    int flag;
    int last_combat;
    int lifevalue;
    Warrior* p_red_warrior;
    Warrior* p_blue_warrior;
    City(void);
    void lion_run_away(int);
    void produce_elements(void);
    void judge_flag(int, int, int);
};

#endif  // CITY_H  // NOLINT