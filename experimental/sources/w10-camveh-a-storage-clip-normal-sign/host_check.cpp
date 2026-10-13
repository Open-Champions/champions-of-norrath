// Host contract for clipFrustumNormalSign storage. Not an EE plan input.
#include "storage.cpp"

int main() {
    if (sizeof(clipFrustumNormalSign) != 18) return 1;
    for (signed char c : clipFrustumNormalSign) if (c != 0) return 2;
    return 0;
}
