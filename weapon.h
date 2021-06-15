// Copyright
#ifndef WEAPON_H  // NOLINT
#define WEAPON_H


class Sword{
 public:
    int attack;
    Sword(void) {}
    explicit Sword(int);
};

class Bomb{
};

class Arrow{
 public:
    int attack;
    int left_arrow;
    Arrow(void) {}
    explicit Arrow(int);
};


#endif  // WEAPON_H  // NOLINT
