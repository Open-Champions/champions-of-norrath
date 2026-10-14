#include "fr_deadkey.cpp"

int main() {
    if (sizeof(sFrDeadkey) != 16) return 1;
    return sFrDeadkey[0] == 94 && sFrDeadkey[1] == 96 && sFrDeadkey[2] == 126 && sFrDeadkey[3] == 168 ? 0 : 2;
}
