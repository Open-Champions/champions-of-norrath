// Host contract for doMyUniqueForces__7Vehicle. Not an EE plan input.
#include "unique_forces.cpp"

#include <cstdio>
#include <cstring>

int main() {
    unsigned char vehicle[64];
    unsigned char before[64];
    std::memset(vehicle, 0x6b, sizeof vehicle);
    std::memcpy(before, vehicle, sizeof vehicle);
    int failures = 0;
    if (doMyUniqueForces__7Vehicle(vehicle) != 0) {
        std::fprintf(stderr, "fail: result is zero\n");
        ++failures;
    }
    if (std::memcmp(vehicle, before, sizeof vehicle) != 0) {
        std::fprintf(stderr, "fail: nothing is written\n");
        ++failures;
    }
    return failures ? 1 : 0;
}
