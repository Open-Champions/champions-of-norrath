// Host contract for getClosestLivingPlayer__FG6Point3f. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// A host pointer is 8 bytes; each record's object pointer at +21872 is a host pointer.
#include <cstdio>
#include <cstring>
extern "C" unsigned char player[23984 * 4];
unsigned char player[23984 * 4];
int nmPlayers;
#include "get_closest_living_player.cpp"
static unsigned char objs[4][64];
static void place(int i, float x, float y, float z, unsigned flags) {
    std::memset(objs[i], 0, 64);
    std::memcpy(objs[i] + 16, &x, 4); std::memcpy(objs[i] + 20, &y, 4); std::memcpy(objs[i] + 24, &z, 4);
    std::memcpy(objs[i] + 12, &flags, 4);
    unsigned char* p = objs[i];
    std::memcpy(player + 23984 * i + 21872, &p, sizeof p);
}
int main() {
    Point3Words at = {0, 0, 0};
    nmPlayers = 0;
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != -1) return 1;
    nmPlayers = 3;
    place(0, 10, 0, 0, 0); place(1, 0, 3, 4, 0); place(2, 0, 0, 2, 0);
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != 2) return 2;
    if (getClosestLivingPlayer__FG6Point3f(&at, 2.f) != -1) return 3;     // tie with the limit is rejected
    if (getClosestLivingPlayer__FG6Point3f(&at, 2.0001f) != 2) return 4;
    if (getClosestLivingPlayer__FG6Point3f(&at, 5.f) != 2) return 5;
    // A flagged (0x8) player is skipped; other flag bits do not matter.
    place(2, 0, 0, 2, 0x8);
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != 1) return 6;   // distance 5
    place(2, 0, 0, 2, 0xfffffff7u);
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != 2) return 7;
    // A null object pointer is skipped.
    std::memset(player + 23984 * 2 + 21872, 0, 8);
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != 1) return 8;
    // Equal distances: the earlier player wins (strict comparison).
    place(2, 0, 5, 0, 0);
    place(1, 0, 0, 5, 0);
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != 1) return 9;
    // Only the first nmPlayers records count.
    nmPlayers = 1;
    place(0, 100, 0, 0, 0);
    if (getClosestLivingPlayer__FG6Point3f(&at, 1000.f) != 0) return 10;
    if (getClosestLivingPlayer__FG6Point3f(&at, 50.f) != -1) return 11;
    // Every axis counts, with a moved origin and negative offsets.
    nmPlayers = 2;
    place(0, 1, 1, 1, 0); place(1, -2, 2, -2, 0);
    Point3Words o = {-2, 2, -1};
    if (getClosestLivingPlayer__FG6Point3f(&o, 100.f) != 1) return 12;
    Point3Words o2 = {1, 1, 3};
    if (getClosestLivingPlayer__FG6Point3f(&o2, 100.f) != 0) return 13;
    // Position is read as a copy (the argument is not modified).
    if (o2.x != 1 || o2.y != 1 || o2.z != 3) return 14;
    std::puts("getClosestLivingPlayer check passed");
    return 0;
}
