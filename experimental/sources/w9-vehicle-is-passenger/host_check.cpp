// Host contract for isPassenger__7Vehiclei. Not an EE plan input.
#include "is_passenger.cpp"

#include <cstdio>
#include <cstring>

int main() {
    int failures = 0;
    unsigned char o[2304];
    std::memset(o, 0, sizeof o);
    int* slots = reinterpret_cast<int*>(o + 960);
    int* count = reinterpret_cast<int*>(o + 976);
    slots[0] = 7; slots[1] = -1; slots[2] = 9; slots[3] = 7;
    for (int n = -1; n <= 4; ++n) {
        *count = n;
        for (int v = -2; v <= 10; ++v) {
            int want = 0;
            for (int i = 0; i < n; ++i) if (slots[i] == v) want = 1;
            if (isPassenger__7Vehiclei(o, v) != want) { std::fprintf(stderr, "fail: n %d v %d\n", n, v); ++failures; }
        }
    }
    // The slot at index 4 is the count word itself and must not be read as a passenger.
    *count = 4; slots[4] = 4;
    if (isPassenger__7Vehiclei(o, 4) != 0) { std::fprintf(stderr, "fail: read past count\n"); ++failures; }
    return failures ? 1 : 0;
}
