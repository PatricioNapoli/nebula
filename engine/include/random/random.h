#pragma once

#include <random>
#include <algorithm>

#include <injector/injectable.h>

namespace nebula {
    class Random : public Injectable {

        static constexpr char alphabet[] =
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "0123456789";

        std::random_device _randomDevice;
        std::mt19937 _mersenneEngine;

        std::uniform_int_distribution<> _alphaDist;

    public:
        Random();

        int randomInt(int min, int max);
        double randomDouble(double min, double max);
        string randomAlphaString(int length);
        static string randomAlphaStringG(int length);

        NEBULA_INJECTABLE_E(Random);
    };
}
