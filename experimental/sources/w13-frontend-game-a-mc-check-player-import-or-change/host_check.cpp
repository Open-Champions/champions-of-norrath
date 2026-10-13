// Host check for MC_CheckPlayerImportOrChange__Fv (source: ../w9-frontend-a-mc-check-player-import-or-change).
// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// Every callee is a recording stand-in. The expected event order and the memory results are written
// out by hand from the source's header comment for five scenarios: removing a player, the
// position/yaw refresh, the import branch, the change branch with statuses 1, 0, 2, 3, and the
// change branch with a different status mix. Not covered: the `first` = -1 case, which reads
// before playerChangeOldPos in the original and cannot be run safely on the host.
#include "../w9-frontend-a-mc-check-player-import-or-change/check_player_import_or_change.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
int nmPlayers;
signed char killPlayer;
signed char netNetworkMode;
int asyncMode;
int syncState;
signed char MC_FrontEndImport;
int playerStatus[4];
alignas(16) unsigned char player[4 * 23984];
alignas(16) unsigned char tempPlayerStruct[4 * 23984];
unsigned char playerSelectInfo[4 * 41 + 8];
float playerChangeOldPos[4][3];
unsigned short playerChangeOldYaw[4];
float playerStartPos[5][3];
unsigned short playerStartYaw[5];
void* gameCam = reinterpret_cast<void*>(0x4321);
int gamePause;
int saveCheckPointCount;
signed char particlesPaused;
}

static const int kSize = 23984;
static std::string events;

static void ev(const char* fmt, long a = 0, long b = 0, long c = 0, long d = 0, long e = 0, long f = 0, long g = 0) {
    char buf[160];
    std::snprintf(buf, sizeof buf, fmt, a, b, c, d, e, f, g);
    events += buf;
    events += ";";
}

static unsigned hashBytes(const unsigned char* p, int n) {
    unsigned h = 2166136261u;
    for (int i = 0; i < n; ++i) h = (h ^ p[i]) * 16777619u;
    return h;
}
static unsigned char* rec(int i) { return player + i * kSize; }
static unsigned char* trec(int i) { return tempPlayerStruct + i * kSize; }
static int& invCount(unsigned char* r) { return *reinterpret_cast<int*>(r + 9560 + 4608); }
static unsigned char* item(unsigned char* r, int k) { return r + 9560 + 72 * k; }

extern "C" void convertPointersToIndicies__Fi(int index) {
    ev("p2i %ld", index);
    std::memcpy(trec(index), rec(index), kSize);  // stand-in: leaves a copy of the record for the next copy
    trec(index)[0] ^= 0xff;
}
extern "C" void convertIndiciesToPointers__Fi(int index) {
    ev("i2p %ld", index);
    rec(index)[100] ^= 0x5a;
}
extern "C" void removeItemFromInventory__Fiiii(int pl, int index, int count, int mode) {
    ev("remove %ld %ld %ld %ld", pl, index, count, mode);
    unsigned char* r = rec(pl);
    int n = invCount(r);
    std::memmove(item(r, index), item(r, index + 1), 72 * (n - index - 1));
    invCount(r) = n - 1;
}
extern "C" int addItemToInventory__FiPQ24item11descriptioni(int pl, void* it, int count) {
    ev("add %ld %lx %ld", pl, static_cast<long>(hashBytes(static_cast<unsigned char*>(it), 72)), count);
    return 0;
}
extern "C" void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int first, int second, char* third) {
    float v[3];
    std::memcpy(v, point, 12);
    char buf[120];
    std::snprintf(buf, sizeof buf, "find %g %g %g %d %d %d;", v[0], v[1], v[2], first, second, third != nullptr);
    events += buf;
    v[0] += 1; v[1] += 2; v[2] += 3;
    std::memcpy(result, v, 12);
}
extern "C" void padChangeIndex__Fii(int pad, int index) { ev("pad %ld %ld", pad, index); }
extern "C" void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char a, signed char b, signed char c, signed char d, char* tint) {
    char buf[200];
    std::snprintf(buf, sizeof buf, "init %d %d %s %d %d %d %d %s;", index, second, name, a, b, c, d, tint);
    events += buf;
    std::strcpy(reinterpret_cast<char*>(rec(index)) + 36, "Init");
    rec(index)[90] = 0x77;
}
extern "C" void debugClearStartMenu__Fv() { ev("clear"); }
extern "C" void Reset__6Cameraii(void* camera, int a, int b) { ev("reset %ld %ld %ld", camera == gameCam, a, b); }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    events: %s\n", __LINE__, #c, events.c_str()); return 1; } } while (0)

static unsigned char itemByte(int r, int k, int j) { return static_cast<unsigned char>(r * 13 + k * 7 + j * 3 + 1); }

// Fill both record arrays. Items per record: `n` of them, flagged where mask has the bit.
static void reset(const int* counts, const int* masks) {
    events.clear();
    for (int r = 0; r < 4; ++r)
        for (int i = 0; i < kSize; ++i) {
            rec(r)[i] = static_cast<unsigned char>(r * 29 + i * 3 + (i >> 7));
            trec(r)[i] = static_cast<unsigned char>(r * 31 + i * 5 + 7 + (i >> 9));
        }
    for (int r = 0; r < 4; ++r) {
        invCount(rec(r)) = counts[r];
        invCount(trec(r)) = 0;
        for (int k = 0; k < 8; ++k) {
            for (int j = 0; j < 72; ++j) item(rec(r), k)[j] = itemByte(r, k, j);
            item(rec(r), k)[12] = static_cast<unsigned char>(item(rec(r), k)[12] & 0x7f);
            if (masks[r] & (1 << k)) item(rec(r), k)[12] |= 0x80;
        }
        std::strcpy(reinterpret_cast<char*>(rec(r)) + 36, r == 0 ? "Alpha" : r == 1 ? "Beta" : r == 2 ? "Gamma" : "Delta");
    }
    for (int i = 0; i < 4 * 41 + 8; ++i) playerSelectInfo[i] = static_cast<unsigned char>(i + 1);
    for (int s = 0; s < 4; ++s) {
        std::snprintf(reinterpret_cast<char*>(playerSelectInfo) + 41 * s, 32, "sel%d", s);
        std::snprintf(reinterpret_cast<char*>(playerSelectInfo) + 41 * s + 37, 4, "t%d", s);
        playerSelectInfo[41 * s + 32] = static_cast<unsigned char>(10 + s);
        playerSelectInfo[41 * s + 33] = static_cast<unsigned char>(20 + s);
        playerSelectInfo[41 * s + 34] = static_cast<unsigned char>(30 + s);
        playerSelectInfo[41 * s + 35] = static_cast<unsigned char>(40 + s);
        playerSelectInfo[41 * s + 36] = static_cast<unsigned char>(0xf0 + s);  // reads back as a negative signed char
    }
    for (int i = 0; i < 4; ++i) {
        for (int c = 0; c < 3; ++c) playerChangeOldPos[i][c] = static_cast<float>(100 * i + 10 * c + 1);
        playerChangeOldYaw[i] = static_cast<unsigned short>(1000 * (i + 1));
    }
    for (int i = 0; i < 5; ++i) {
        for (int c = 0; c < 3; ++c) playerStartPos[i][c] = -static_cast<float>(i + 1);
        playerStartYaw[i] = 0xabcd;
    }
    for (int i = 0; i < 4; ++i) playerStatus[i] = 0;
    for (int r = 0; r < 4; ++r) *reinterpret_cast<float*>(rec(r) + 68) = static_cast<float>(500 + r);
    nmPlayers = 0; killPlayer = 0; netNetworkMode = 5; asyncMode = 7; syncState = 12; MC_FrontEndImport = 0;
    gamePause = 9; saveCheckPointCount = 9; particlesPaused = 9;
}

static const int noCounts[4] = {0, 0, 0, 0};
static const int noMasks[4] = {0, 0, 0, 0};

static unsigned stateHash() {
    return hashBytes(player, sizeof player) ^ hashBytes(tempPlayerStruct, sizeof tempPlayerStruct) * 3u;
}

int main() {
    // 1. removing a player (killPlayer = index + 1): later records slide down, in order
    reset(noCounts, noMasks);
    nmPlayers = 4; killPlayer = 2; syncState = 99; asyncMode = 3;  // other state must not matter
    unsigned char t1[kSize]; unsigned char t2[kSize];
    {
        std::memcpy(t1, rec(2), kSize); t1[0] ^= 0xff;  // what the p2i stand-in leaves in the temp record
        std::memcpy(t2, rec(3), kSize); t2[0] ^= 0xff;
    }
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events == "p2i 2;i2p 1;p2i 3;i2p 2;");
    CHECK(nmPlayers == 3 && killPlayer == 0 && netNetworkMode == 5);
    {
        unsigned char expect1[kSize], expect2[kSize];
        std::memcpy(expect1, t1, kSize); expect1[100] ^= 0x5a;
        std::memcpy(expect2, t2, kSize); expect2[100] ^= 0x5a;
        CHECK(std::memcmp(rec(1), expect1, kSize) == 0);
        CHECK(std::memcmp(rec(2), expect2, kSize) == 0);
    }
    CHECK(gamePause == 9 && saveCheckPointCount == 9 && particlesPaused == 9);
    // killing the last player copies nothing; going from two players to one clears netNetworkMode
    reset(noCounts, noMasks);
    nmPlayers = 4; killPlayer = 4;
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events.empty() && nmPlayers == 3 && killPlayer == 0 && netNetworkMode == 5);
    reset(noCounts, noMasks);
    nmPlayers = 2; killPlayer = 1;
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events == "p2i 1;i2p 0;" && nmPlayers == 1 && netNetworkMode == 0 && killPlayer == 0);
    reset(noCounts, noMasks);
    nmPlayers = 3; killPlayer = 3;
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events.empty() && nmPlayers == 2 && netNetworkMode == 5);

    // 2. nothing happens unless asyncMode is 7; syncState 7 refreshes the start positions only
    reset(noCounts, noMasks);
    nmPlayers = 3; asyncMode = 6; syncState = 7;
    unsigned before = stateHash();
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events.empty() && before == stateHash() && playerStartYaw[0] == 0xabcd && nmPlayers == 3);
    reset(noCounts, noMasks);
    nmPlayers = 3; syncState = 5;
    before = stateHash();
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events.empty() && before == stateHash() && playerStartYaw[0] == 0xabcd && gamePause == 9);
    reset(noCounts, noMasks);
    nmPlayers = 3; syncState = 7;
    before = stateHash();
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events.empty() && before == stateHash());
    for (int i = 0; i < 3; ++i) {
        CHECK(playerStartYaw[i] == playerChangeOldYaw[i]);
        for (int c = 0; c < 3; ++c) CHECK(playerStartPos[i][c] == playerChangeOldPos[i][c]);
    }
    CHECK(playerStartYaw[3] == 0xabcd && playerStartPos[3][0] == -4.0f && nmPlayers == 3 && gamePause == 9);

    // 3. import (syncState 12, MC_FrontEndImport set): records come from tempPlayerStruct
    {
        const int counts[4] = {4, 3, 0, 0};
        const int masks[4] = {0, 0, 0, 0};
        reset(counts, masks);
        // temp record 0: three items, the second flagged; temp record 1: none
        for (int r = 0; r < 2; ++r) {
            for (int k = 0; k < 8; ++k)
                for (int j = 0; j < 72; ++j) item(trec(r), k)[j] = itemByte(r + 4, k, j);
        }
        invCount(trec(0)) = 3;
        item(trec(0), 1)[12] = static_cast<unsigned char>(item(trec(0), 1)[12] | 0x80);
        item(trec(0), 0)[12] = static_cast<unsigned char>(item(trec(0), 0)[12] & 0x7f);
        item(trec(0), 2)[12] = static_cast<unsigned char>(item(trec(0), 2)[12] & 0x7f);
        invCount(trec(1)) = 2;
        item(trec(1), 0)[12] = static_cast<unsigned char>(item(trec(1), 0)[12] | 0x80);
        item(trec(1), 1)[12] = static_cast<unsigned char>(item(trec(1), 1)[12] | 0x80);
        nmPlayers = 2; MC_FrontEndImport = 1;
        unsigned char keep0[72], keep2[72];
        std::memcpy(keep0, item(trec(0), 0), 72);
        std::memcpy(keep2, item(trec(0), 2), 72);
        static unsigned char r2copy[kSize], r3copy[kSize];
        std::memcpy(r2copy, rec(2), kSize); std::memcpy(r3copy, rec(3), kSize);
        float p0 = *reinterpret_cast<float*>(trec(0) + 68);
        MC_CheckPlayerImportOrChange__Fv();
        CHECK(events == "i2p 0;remove 0 1 2 0;i2p 1;remove 1 0 2 0;remove 1 0 2 0;");
        CHECK(nmPlayers == 2 && gamePause == 0 && saveCheckPointCount == 1 && particlesPaused == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(0)) + 36, "None") == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(1)) + 36, "None") == 0);
        CHECK(*reinterpret_cast<int*>(rec(0) + 21872) == 0 && *reinterpret_cast<int*>(rec(1) + 21872) == 0);
        CHECK(*reinterpret_cast<float*>(rec(0) + 68) == p0);
        CHECK(invCount(rec(0)) == 2 && invCount(rec(1)) == 0);
        CHECK(std::memcmp(item(rec(0), 0), keep0, 72) == 0 && std::memcmp(item(rec(0), 1), keep2, 72) == 0);
        CHECK(std::memcmp(rec(2), r2copy, kSize) == 0 && std::memcmp(rec(3), r3copy, kSize) == 0);
        CHECK(rec(0)[100] == static_cast<unsigned char>(trec(0)[100] ^ 0x5a));
    }
    // import with no players: only the pause/checkpoint words change
    reset(noCounts, noMasks);
    MC_FrontEndImport = 1;
    before = stateHash();
    MC_CheckPlayerImportOrChange__Fv();
    CHECK(events.empty() && before == stateHash() && gamePause == 0 && saveCheckPointCount == 1 && particlesPaused == 0 && nmPlayers == 0);

    // 4. change: old count 3, statuses {1, 0, 2, 3}
    {
        const int counts[4] = {4, 1, 2, 1};
        const int masks[4] = {0x0a, 0, 0, 0};  // player 0: items 1 and 3 flagged
        reset(counts, masks);
        for (int r = 0; r < 4; ++r) {
            for (int k = 0; k < 8; ++k)
                for (int j = 0; j < 72; ++j) item(trec(r), k)[j] = itemByte(r + 8, k, j);
            invCount(trec(r)) = 0;
        }
        nmPlayers = 3; playerStatus[0] = 1; playerStatus[1] = 0; playerStatus[2] = 2; playerStatus[3] = 3;
        unsigned char flaggedA[72], flaggedB[72], keepA[72], keepB[72];
        std::memcpy(keepA, item(rec(0), 0), 72); std::memcpy(flaggedA, item(rec(0), 1), 72);
        std::memcpy(keepB, item(rec(0), 2), 72); std::memcpy(flaggedB, item(rec(0), 3), 72);
        unsigned char block0[4100]; std::memcpy(block0, rec(0) + 17768, 4100);
        float savedPoint = *reinterpret_cast<float*>(rec(0) + 68);
        float point0 = *reinterpret_cast<float*>(rec(0) + 68);
        unsigned char rec3Before[kSize]; std::memcpy(rec3Before, rec(3), kSize);
        unsigned char temp3[kSize]; std::memcpy(temp3, trec(3), kSize);
        MC_CheckPlayerImportOrChange__Fv();
        // The expected rebuild text is spelled out by hand from the status table of the header comment.
        std::string want = "pad 0 0;find 1 11 21 20 24 0;pad 2 1;find 1 11 21 20 24 0;pad 3 2;";
        want += "p2i 0;i2p 0;";
        char init[120];
        std::snprintf(init, sizeof init, "init 1 %d sel2 %d %d %d %d t2;", 10 + 2, 20 + 2, 30 + 2, static_cast<signed char>(0xf2), 40 + 2);
        want += init;
        want += "i2p 2;";
        // removal of flagged items from every player below the new count (3), then re-adding the
        // two collected items to each of them in collection order
        want += "remove 0 1 2 0;remove 0 2 2 0;";
        char add[400];
        unsigned hA = hashBytes(flaggedA, 72), hB = hashBytes(flaggedB, 72);
        std::snprintf(add, sizeof add, "add 0 %x 1;add 0 %x 1;add 1 %x 1;add 1 %x 1;add 2 %x 1;add 2 %x 1;", hA, hB, hA, hB, hA, hB);
        want += add;
        want += "clear;reset 1 1 0;";
        CHECK(events == want);
        CHECK(nmPlayers == 3 && gamePause == 0 && saveCheckPointCount == 1 && particlesPaused == 0);
        // start positions: slot 0 from pad 0, slots 1 and 2 near pad 0's old position with yaw + 8000
        for (int c = 0; c < 3; ++c) {
            CHECK(playerStartPos[0][c] == playerChangeOldPos[0][c]);
            CHECK(playerStartPos[1][c] == playerChangeOldPos[0][c] + 1 + c);
            CHECK(playerStartPos[2][c] == playerChangeOldPos[0][c] + 1 + c);
        }
        CHECK(playerStartYaw[0] == 1000 && playerStartYaw[1] == 9000 && playerStartYaw[2] == 9000);
        CHECK(playerStartYaw[3] == 0xabcd && playerStartPos[3][0] == -4.0f);
        // slot 0: status 1, copy of the temp record that p2i left, pointer word cleared, own point kept
        CHECK(*reinterpret_cast<int*>(rec(0) + 21872) == 0);
        CHECK(*reinterpret_cast<float*>(rec(0) + 68) == point0);
        CHECK(std::memcmp(rec(0) + 17768, block0, 4100) == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(0)) + 36, "Alpha") == 0);
        // slot 1: initialised by playerInit, then takes the saved point, block and name of player 0
        CHECK(rec(1)[90] == 0x77);
        CHECK(*reinterpret_cast<float*>(rec(1) + 68) == savedPoint);
        CHECK(std::memcmp(rec(1) + 17768, block0, 4100) == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(1)) + 36, "Alpha") == 0);
        // slot 2: status 3, copy of temp record 3 with the pointer word cleared
        CHECK(*reinterpret_cast<int*>(rec(2) + 21872) == 0);
        CHECK(*reinterpret_cast<float*>(rec(2) + 68) == savedPoint);
        CHECK(std::memcmp(rec(2) + 17768, block0, 4100) == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(2)) + 36, "Alpha") == 0);
        CHECK(std::memcmp(rec(2) + 200, temp3 + 200, 8000) == 0);  // the rest is the temp record
        // record 3 is no longer a player slot and stays as it was
        CHECK(std::memcmp(rec(3), rec3Before, kSize) == 0);
        // player 0 lost both flagged items (unflagged ones remain, in order)
        CHECK(invCount(rec(0)) == 2);
        CHECK(std::memcmp(item(rec(0), 0), keepA, 72) == 0 && std::memcmp(item(rec(0), 1), keepB, 72) == 0);
    }

    // 5. change: old count 2, statuses {0, 1, 1, 0}: pad 1 is first; slot 0 comes from record 1
    {
        const int counts[4] = {2, 3, 0, 0};
        const int masks[4] = {0, 0x4, 0, 0};  // player 1: its third item is flagged
        reset(counts, masks);
        nmPlayers = 2; playerStatus[1] = 1; playerStatus[2] = 1;
        float p1 = *reinterpret_cast<float*>(rec(1) + 68);
        float p2 = *reinterpret_cast<float*>(rec(2) + 68);
        unsigned char block0[4100]; std::memcpy(block0, rec(0) + 17768, 4100);
        unsigned char rec1Before[kSize]; std::memcpy(rec1Before, rec(1), kSize);
        MC_CheckPlayerImportOrChange__Fv();
        // pad 1 (< count, status 1) keeps its position; pad 2 (>= count) is placed near pad 1 (first)
        std::string want = "pad 1 0;find 101 111 121 20 24 0;pad 2 1;";
        // no flagged items in player 0, so nothing is collected; slot 0 <- pad 1, slot 1 <- pad 2
        want += "p2i 1;i2p 0;p2i 2;i2p 1;";
        CHECK(events.substr(0, want.size()) == want);
        std::string rest = events.substr(want.size());
        // slot 0 now holds record 1's items, of which the third is flagged: it is removed again
        CHECK(rest == "remove 0 2 2 0;clear;reset 1 1 0;");
        CHECK(invCount(rec(0)) == 2);
        CHECK(std::memcmp(item(rec(0), 0), rec1Before + 9560, 72) == 0);
        CHECK(std::memcmp(item(rec(0), 1), rec1Before + 9560 + 72, 72) == 0);
        CHECK(nmPlayers == 2);
        for (int c = 0; c < 3; ++c) {
            CHECK(playerStartPos[0][c] == playerChangeOldPos[1][c]);
            CHECK(playerStartPos[1][c] == playerChangeOldPos[1][c] + 1 + c);
        }
        CHECK(playerStartYaw[0] == 2000 && playerStartYaw[1] == 10000);
        CHECK(*reinterpret_cast<float*>(rec(0) + 68) == p1 && *reinterpret_cast<float*>(rec(1) + 68) == p2);
        CHECK(*reinterpret_cast<int*>(rec(0) + 21872) == 0 && *reinterpret_cast<int*>(rec(1) + 21872) == 0);
        CHECK(std::memcmp(rec(0) + 17768, block0, 4100) == 0 && std::memcmp(rec(1) + 17768, block0, 4100) == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(0)) + 36, "Alpha") == 0);
        CHECK(std::strcmp(reinterpret_cast<char*>(rec(1)) + 36, "Alpha") == 0);
        // slot 0 is the temp copy that p2i made of record 1 (its first byte flipped), then i2p's mark
        CHECK(rec(0)[0] == static_cast<unsigned char>(rec1Before[0] ^ 0xff));
        CHECK(rec(0)[100] == static_cast<unsigned char>(rec1Before[100] ^ 0x5a));
        CHECK(std::memcmp(rec(0) + 200, rec1Before + 200, 8000) == 0);
    }

    // 6. change: old count 1, statuses {2, 0, 0, 1}: a new player is created in slot 0, pad 3 (>= count)
    //    is placed next to pad 0
    {
        const int counts[4] = {0, 0, 0, 0};
        reset(counts, noMasks);
        nmPlayers = 1; playerStatus[0] = 2; playerStatus[3] = 1;
        float p0 = *reinterpret_cast<float*>(rec(0) + 68);
        float p3 = *reinterpret_cast<float*>(rec(3) + 68);
        MC_CheckPlayerImportOrChange__Fv();
        char init[120];
        std::snprintf(init, sizeof init, "init 0 10 sel0 20 30 %d 40 t0;", static_cast<signed char>(0xf0));
        std::string order = std::string("find 1 11 21 20 24 0;pad 0 0;find 1 11 21 20 24 0;pad 3 1;") + init +
                            "p2i 3;i2p 1;clear;reset 1 1 0;";
        CHECK(events == order);
        CHECK(nmPlayers == 2);
        CHECK(playerStartYaw[0] == 9000 && playerStartYaw[1] == 9000);
        CHECK(*reinterpret_cast<float*>(rec(0) + 68) == p0);
        CHECK(*reinterpret_cast<float*>(rec(1) + 68) == p3);
    }
    std::puts("check player import or change passed");
    return 0;
}
