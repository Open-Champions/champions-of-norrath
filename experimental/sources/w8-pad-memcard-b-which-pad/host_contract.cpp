#include "which_pad.cpp"

int main() {
    if (sizeof whichPad != 1u) return 1;
    if (whichPad != 0) return 2;
    whichPad = static_cast<signed char>(0x80);
    if (whichPad >= 0) return 3;
    return 0;
}
