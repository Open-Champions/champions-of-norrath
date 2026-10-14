// Host contract for the Vehicle globals. Not an EE plan input.
#include "vehicle_globals.cpp"

#include <cstdio>

int main() {
    int failures = 0;
    if (vehicleCount != 0) { std::fprintf(stderr, "fail: count starts at zero\n"); ++failures; }
    if (sizeof(vehicleList) != 32 * sizeof(void*)) { std::fprintf(stderr, "fail: 32 entries\n"); ++failures; }
    for (unsigned char* p : vehicleList) if (p != nullptr) { std::fprintf(stderr, "fail: list starts empty\n"); ++failures; }
    return failures ? 1 : 0;
}
