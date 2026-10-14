// Host check for statsDrawDelay. Not an EE plan input; it includes the definition file.
#include "statsDrawDelay.cpp"
int main() {
    if (sizeof statsDrawDelay != 4u) return 1;
    if (statsDrawDelay != 0) return 2;
    return 0;
}
