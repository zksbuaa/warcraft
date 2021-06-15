// Copyright
#ifndef FUNCTION_H  // NOLINT
#define FUNCTION_H  // NOLINT

#include <vector>
#include "city.h"  // NOLINT
class Headquarter;  // NOLINT
using namespace std;  // NOLINT

void shot_arrow(int, int, vector<City>&);

void bomb_explode(int, vector<City>&, int);

void fight(int, vector<City>&, int, Headquarter&, Headquarter&);
#endif  // FUNCTION_H  // NOLINT
