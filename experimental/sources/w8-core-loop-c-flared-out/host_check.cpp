// Host contract for flaredOut. Not an EE plan input.
#include "flared_out.cpp"

int main() {
    if (flaredOut != 0) return 1;
    if (sizeof flaredOut != 4u) return 2;
    flaredOut = flaredOut + 1;
    if (flaredOut != 0 + 1) return 3;
    return 0;
}
