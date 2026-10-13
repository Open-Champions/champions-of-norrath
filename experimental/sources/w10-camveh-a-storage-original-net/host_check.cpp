// Host contract for the original-position storage. Not an EE plan input.
#include "storage.cpp"

int main() {
    if (sizeof(originalPos) != 48 || sizeof(originalYaw) != 8) return 1;
    for (unsigned char b : originalPos) if (b != 0) return 2;
    for (unsigned char b : originalYaw) if (b != 0) return 3;
    return 0;
}
