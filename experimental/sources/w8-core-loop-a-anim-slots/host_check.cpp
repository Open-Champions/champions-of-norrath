// Host check for anim. Not an EE plan input.
#include "anim_slots.cpp"

int main() {
    if (sizeof(anim) / sizeof(anim[0]) != 8) return 1;
    for (void* p : anim) if (p) return 2;
    anim[0] = anim;
    anim[4] = anim + 4;
    return anim[0] == anim && anim[4] == anim + 4 ? 0 : 3;
}
