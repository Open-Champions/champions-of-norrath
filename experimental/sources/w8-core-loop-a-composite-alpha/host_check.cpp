// Host check for compositeAlpha. Not an EE plan input.
#include "composite_alpha.cpp"

int main() {
    if (sizeof(compositeAlpha) != 4 || compositeAlpha != 128) return 1;
    return 0;
}
