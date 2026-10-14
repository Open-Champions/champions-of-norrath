// Host check for p1 and p2. Not an EE plan input.
#include "hermite_points.cpp"
int main() {
    static_assert(sizeof(Slot) == 16 && alignof(Slot) == 16, "slot");
    if (p1.x != 0.0f || p1.y != 0.0f || p1.z != 0.0f || p2.x != 0.0f || p2.y != 0.0f || p2.z != 0.0f) return 1;
    if (reinterpret_cast<__UINTPTR_TYPE__>(&p1) % 16 != 0 || reinterpret_cast<__UINTPTR_TYPE__>(&p2) % 16 != 0) return 2;
    return &p1 == &p2 ? 3 : 0;
}
