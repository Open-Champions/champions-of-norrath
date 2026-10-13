#include "net_get_local_object.cpp"

#include <cstdio>

signed char netNetworkMode;
int nmPartners;
unsigned char partners[17632 * 4 + 17620 + 1];
void* localPredictedPlayer;

struct Obj { alignas(8) unsigned char b[1024]; };
static Obj obj;
static int token;

static void set(unsigned flags, int index) {
    *reinterpret_cast<unsigned*>(obj.b + 12) = flags;
    *reinterpret_cast<int*>(obj.b + 960) = index;
}

int main() {
    localPredictedPlayer = &token;
    netNetworkMode = 1;
    nmPartners = 3;
    partners[17620 + 17632 * 2] = 1;
    set(0x40, 2);
    if (netGetLocalObject__FP8Creature(&obj) != &token) return 1;
    set(0x00, 2);  // flag bit clear
    if (netGetLocalObject__FP8Creature(&obj) != &obj) return 2;
    set(0xbf, 2);  // every other bit set, still clear
    if (netGetLocalObject__FP8Creature(&obj) != &obj) return 3;
    set(0x40, 1);  // partner flag byte zero
    if (netGetLocalObject__FP8Creature(&obj) != &obj) return 4;
    set(0x40, 3);  // index == nmPartners skips the byte test
    if (netGetLocalObject__FP8Creature(&obj) != &token) return 5;
    set(0x40, 100);
    if (netGetLocalObject__FP8Creature(&obj) != &token) return 6;
    nmPartners = 3;
    set(0x40, 2);
    netNetworkMode = 0;
    if (netGetLocalObject__FP8Creature(&obj) != &obj) return 8;
    netNetworkMode = 1;
    localPredictedPlayer = nullptr;
    if (netGetLocalObject__FP8Creature(&obj) != &obj) return 9;
    std::puts("net get local object passed");
    return 0;
}
