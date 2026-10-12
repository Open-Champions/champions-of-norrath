// Host contract for flaredIn. Not an EE plan input.
#include "flared_in.cpp"

int main() {
    if (flaredIn != 0) return 1;
    if (sizeof flaredIn != 4u) return 2;
    flaredIn = flaredIn + 1;
    if (flaredIn != 0 + 1) return 3;
    return 0;
}
