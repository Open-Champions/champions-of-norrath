// Host contract for SetHold__7Vehiclei. Not an EE plan input.
#include "set_hold.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char vehicle[2304];
    unsigned char before[2304];
    std::memset(vehicle, 0x44, sizeof vehicle);
    std::memcpy(before, vehicle, sizeof vehicle);
    SetHold__7Vehiclei(vehicle, 0x01020304);
    expect(vehicle[1024] == 4 && vehicle[1025] == 3 && vehicle[1026] == 2 && vehicle[1027] == 1,
           "whole word, little endian");
    std::memcpy(before + 1024, vehicle + 1024, 4);
    expect(std::memcmp(vehicle, before, sizeof vehicle) == 0, "nothing else changes");
    SetHold__7Vehiclei(vehicle, -1);
    expect(vehicle[1024] == 0xff && vehicle[1027] == 0xff, "negative value");
    SetHold__7Vehiclei(vehicle, 0);
    expect(vehicle[1024] == 0 && vehicle[1027] == 0, "zero clears the word");
    return failures ? 1 : 0;
}
