#pragma once
#include <random>
#include <ctime>

class random {

    static std::mt19937 Random;

public:

    static int randomInt(int min, int max);
};


