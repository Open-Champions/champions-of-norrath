// Host check: compiled together with max_distance.cpp so it tests the authored definition itself.
#include "max_distance.cpp"

static_assert(sizeof maxDistance == 4, "maxDistance is a four-byte integer");

int main() {
    return maxDistance == 0 ? 0 : 1;
}
