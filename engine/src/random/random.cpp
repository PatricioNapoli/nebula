#include "random/random.h"

namespace nebula {
    Random::Random() : _mersenneEngine(_randomDevice()), _alphaDist(0, sizeof(alphabet)/sizeof(*alphabet)-2) {}

    int Random::randomInt(int min, int max) {
        std::uniform_int_distribution dist(min, max);
        return dist(_mersenneEngine);
    }

    double Random::randomDouble(double min, double max) {
        std::uniform_real_distribution dist(min, max);
        return dist(_mersenneEngine);
    }

    string Random::randomAlphaString(int length) {
        string str;
        str.reserve(length);
        std::generate_n(std::back_inserter(str), length, [&]() {
            return alphabet[_alphaDist(_mersenneEngine)];
        });

        return str;
    }

    string Random::randomAlphaStringG(int length) {
        static std::uniform_int_distribution<> AlphaDist{0, sizeof(alphabet)/sizeof(*alphabet)-2};
        static std::random_device randomDevice;
        static std::mt19937 Mersenne{randomDevice()};

        string str;
        str.reserve(length);
        std::generate_n(std::back_inserter(str), length, [&]() {
            return alphabet[AlphaDist(Mersenne)];
        });

        return str;
    }
}
