// Host check for MC_CheckPlayerImportOrChange__Fv (source: ../w9-frontend-a-mc-check-player-import-or-change).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// All callees are stubs that append to a call log and apply a small recognisable effect to the
// player data (convertPointersToIndicies copies player i into the temporary array and flips
// byte 1; convertIndiciesToPointers flips byte 0 of player i; removal shifts the inventory down;
// addItem appends; playerInit marks byte 0). An independent model written from the notes in the
// source file computes the expected arrays, call log and counters for each scenario.
// Not exercised: the reads at index -1 of playerChangeOldPos / playerChangeOldYaw that happen
// when a joining pad has no earlier player (the original reads those out of range too).
#include "../w9-frontend-a-mc-check-player-import-or-change/check_player_import_or_change.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

extern "C" {
int nmPlayers;
signed char killPlayer, netNetworkMode, MC_FrontEndImport, particlesPaused;
int asyncMode, syncState, playerStatus[4], gamePause, saveCheckPointCount;
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char tempPlayerStruct[4 * 23984];
unsigned char playerSelectInfo[4 * 41 + 8];
float playerChangeOldPos[4][3];
unsigned short playerChangeOldYaw[4];
float playerStartPos[8][3];
unsigned short playerStartYaw[8];
int camTag;
void* gameCam = &camTag;
}

namespace {
constexpr int kRec = 23984;
std::string log_;

std::string num(long v) { return std::to_string(v); }
unsigned char* rec(unsigned char* base, int i) { return base + i * kRec; }

void add(std::string& l, const std::string& s) { l += s + ";"; }
}

extern "C" {
void convertPointersToIndicies__Fi(int index) {
    add(log_, "cp" + num(index));
    std::memcpy(rec(tempPlayerStruct, index), rec(player, index), kRec);
    rec(tempPlayerStruct, index)[1] ^= 0xff;
}
void convertIndiciesToPointers__Fi(int index) {
    add(log_, "ci" + num(index));
    rec(player, index)[0] ^= 0xff;
}
void removeItemFromInventory__Fiiii(int p, int index, int count, int mode) {
    add(log_, "rm" + num(p) + "," + num(index) + "," + num(count) + "," + num(mode));
    unsigned char* inv = rec(player, p) + 9560;
    int n;
    std::memcpy(&n, inv + 4608, 4);
    std::memmove(inv + index * 72, inv + (index + 1) * 72, static_cast<unsigned>(n - index - 1) * 72);
    --n;
    std::memcpy(inv + 4608, &n, 4);
}
int addItemToInventory__FiPQ24item11descriptioni(int p, void* item, int count) {
    unsigned h = 0;
    for (int i = 0; i < 72; ++i) h = h * 31u + static_cast<const unsigned char*>(item)[i];
    add(log_, "add" + num(p) + "," + num(h) + "," + num(count));
    unsigned char* inv = rec(player, p) + 9560;
    int n;
    std::memcpy(&n, inv + 4608, 4);
    std::memcpy(inv + n * 72, item, 72);
    ++n;
    std::memcpy(inv + 4608, &n, 4);
    return 1;
}
void objectFindEmptySpot__FG6Point3iiPc(void* result, const void* point, int a, int b, char* c) {
    float p[3];
    std::memcpy(p, point, 12);
    add(log_, "spot" + num(static_cast<long>(p[0] * 8)) + "," + num(static_cast<long>(p[1] * 8)) + "," +
                  num(static_cast<long>(p[2] * 8)) + "," + num(a) + "," + num(b) + "," + num(c != nullptr));
    float r[3] = {p[0] + 100.f, p[1] + 200.f, p[2] + 300.f};
    std::memcpy(result, r, 12);
}
void padChangeIndex__Fii(int pad, int index) { add(log_, "pad" + num(pad) + "," + num(index)); }
void playerInit__FiiPcScScScScT2(int index, int second, char* name, signed char b1, signed char b2, signed char b3,
                                 signed char b4, char* tint) {
    add(log_, "init" + num(index) + "," + num(second) + "," + name + "," + num(b1) + "," + num(b2) + "," + num(b3) + "," +
                  num(b4) + "," + tint);
    rec(player, index)[0] = 0xa5;
}
void debugClearStartMenu__Fv() { add(log_, "dbg"); }
void Reset__6Cameraii(void* camera, int a, int b) { add(log_, std::string("reset") + (camera == &camTag ? "ok" : "?") + num(a) + num(b)); }
}

namespace {
struct State {
    std::vector<unsigned char> player, temp;
    float startPos[8][3];
    unsigned short startYaw[8];
    int nm, kill, net, gamePause, ckpt, particles;
    std::string log;
};

unsigned rngState = 12345;
unsigned rnd() { rngState = rngState * 1664525u + 1013904223u; return rngState >> 8; }

bool modelFlagged(const unsigned char* item) {
    unsigned long long v;
    std::memcpy(&v, item + 8, 8);
    return (v >> 39) & 1;
}

void modelRemoveFlagged(State& s, int p) {
    unsigned char* inv = rec(s.player.data(), p) + 9560;
    int n;
    std::memcpy(&n, inv + 4608, 4);
    for (int i = 0; i < n; ++i) {
        if (modelFlagged(inv + i * 72)) {
            add(s.log, "rm" + num(p) + "," + num(i) + ",2,0");
            std::memmove(inv + i * 72, inv + (i + 1) * 72, static_cast<unsigned>(n - i - 1) * 72);
            --n;
            std::memcpy(inv + 4608, &n, 4);
            --i;
        }
    }
}

void setupState(int nm, int fillInventories) {
    for (unsigned i = 0; i < sizeof player; ++i) player[i] = static_cast<unsigned char>(rnd());
    for (unsigned i = 0; i < sizeof tempPlayerStruct; ++i) tempPlayerStruct[i] = static_cast<unsigned char>(rnd());
    for (int p = 0; p < 4; ++p)
        for (unsigned char* base : {player, tempPlayerStruct}) {
            unsigned char* inv = rec(base, p) + 9560;
            int n = fillInventories ? static_cast<int>(rnd() % 9) : 0;
            std::memcpy(inv + 4608, &n, 4);
            for (int i = 0; i < n; ++i) {
                unsigned long long v;
                std::memcpy(&v, inv + i * 72 + 8, 8);
                v = (rnd() & 1) ? (v | 0x8000000000ull) : (v & ~0x8000000000ull);
                std::memcpy(inv + i * 72 + 8, &v, 8);
            }
            std::memcpy(rec(base, p) + 36, "Name", 5);
            std::memcpy(rec(base, p) + 36, ("N" + num(p) + "x").c_str(), 4);
            rec(base, p)[39] = 0;
        }
    for (int i = 0; i < 4; ++i) {
        unsigned char* e = playerSelectInfo + i * 41;
        for (int b = 0; b < 41; ++b) e[b] = static_cast<unsigned char>(rnd());
        std::memset(e, 0, 32);
        e[0] = 'P'; e[1] = static_cast<unsigned char>('0' + i);
        e[37] = 't'; e[38] = static_cast<unsigned char>('0' + i); e[39] = 0;
        e[32] = static_cast<unsigned char>(0x80 | i);  // negative signed value
    }
    for (int i = 0; i < 4; ++i) {
        for (int c = 0; c < 3; ++c) playerChangeOldPos[i][c] = static_cast<float>(10 * i + c) + 0.25f;
        playerChangeOldYaw[i] = static_cast<unsigned short>(60000 + 3000 * i);
    }
    for (int i = 0; i < 8; ++i) {
        for (int c = 0; c < 3; ++c) playerStartPos[i][c] = -1.f - static_cast<float>(i);
        playerStartYaw[i] = static_cast<unsigned short>(0xdead);
    }
    nmPlayers = nm;
    gamePause = 5; saveCheckPointCount = 9; particlesPaused = 4; netNetworkMode = 6; killPlayer = 0; MC_FrontEndImport = 0;
}

State snapshot() {
    State s;
    s.player.assign(player, player + sizeof player);
    s.temp.assign(tempPlayerStruct, tempPlayerStruct + sizeof tempPlayerStruct);
    std::memcpy(s.startPos, playerStartPos, sizeof s.startPos);
    std::memcpy(s.startYaw, playerStartYaw, sizeof s.startYaw);
    s.nm = nmPlayers; s.kill = killPlayer; s.net = netNetworkMode; s.gamePause = gamePause; s.ckpt = saveCheckPointCount;
    s.particles = particlesPaused;
    return s;
}

void modelFinish(State& s) { s.gamePause = 0; s.particles = 0; s.ckpt = 1; }

void model(State& s) {
    const int count = s.nm;
    unsigned char* P = s.player.data();
    unsigned char* T = s.temp.data();
    auto flip0 = [&](int i) { rec(P, i)[0] ^= 0xff; add(s.log, "ci" + num(i)); };
    if (s.kill != 0) {
        for (int k = s.kill - 1; k < count - 1; ++k) {
            add(s.log, "cp" + num(k + 1));
            std::memcpy(rec(T, k + 1), rec(P, k + 1), kRec);
            rec(T, k + 1)[1] ^= 0xff;
            std::memcpy(rec(P, k), rec(T, k + 1), kRec);
            flip0(k);
        }
        s.nm = count - 1;
        if (s.nm == 1) s.net = 0;
        s.kill = 0;
        return;
    }
    if (asyncMode != 7) return;
    if (syncState == 7) {
        for (int i = 0; i < count; ++i) {
            std::memcpy(s.startPos[i], playerChangeOldPos[i], 12);
            s.startYaw[i] = playerChangeOldYaw[i];
        }
        return;
    }
    if (syncState != 12) return;
    if (MC_FrontEndImport) {
        for (int i = 0; i < count; ++i) {
            std::memcpy(rec(P, i), rec(T, i), kRec);
            int zero = 0;
            std::memcpy(rec(P, i) + 21872, &zero, 4);
            flip0(i);
            std::memcpy(rec(P, i) + 36, "None", 5);
            modelRemoveFlagged(s, i);
        }
        modelFinish(s);
        return;
    }
    s.nm = 0;
    int first = -1;
    for (int k = 0; k < 4; ++k)
        if (playerStatus[k] != 0 && k < count) { first = k; break; }
    for (int k = 0; k < 4; ++k) {
        int st = playerStatus[k];
        if (!st) continue;
        if (k < count && st == 1) {
            std::memcpy(s.startPos[s.nm], playerChangeOldPos[k], 12);
            s.startYaw[s.nm] = playerChangeOldYaw[k];
        } else {
            float p[3];
            std::memcpy(p, playerChangeOldPos[first], 12);
            add(s.log, "spot" + num(static_cast<long>(p[0] * 8)) + "," + num(static_cast<long>(p[1] * 8)) + "," +
                           num(static_cast<long>(p[2] * 8)) + ",20,24,0");
            for (int c = 0; c < 3; ++c) s.startPos[s.nm][c] = p[c] + 100.f * (c + 1);
            s.startYaw[s.nm] = static_cast<unsigned short>(playerChangeOldYaw[first] + 8000);
        }
        add(s.log, "pad" + num(k) + "," + num(s.nm));
        ++s.nm;
    }
    std::vector<std::vector<unsigned char>> collected;
    {
        unsigned char* inv = rec(P, 0) + 9560;
        int n;
        std::memcpy(&n, inv + 4608, 4);
        for (int i = 0; i < n; ++i)
            if (modelFlagged(inv + i * 72)) collected.emplace_back(inv + i * 72, inv + i * 72 + 72);
    }
    std::vector<unsigned char> block(rec(P, 0) + 17768, rec(P, 0) + 17768 + 4100);
    std::string name(reinterpret_cast<char*>(rec(P, 0) + 36));
    unsigned char savedPoint[12];
    std::memcpy(savedPoint, rec(P, 0) + 68, 12);
    for (int k = 0; k < 4; ++k) {
        int st = playerStatus[k];
        if (!st) continue;
        int d = 0;
        for (int j = 0; j < k; ++j) d += playerStatus[j] != 0;
        unsigned char pt[12];
        std::memcpy(pt, rec(P, k) + 68, 12);
        if (st == 3) {
            std::memcpy(rec(P, d), rec(T, k), kRec);
            int zero = 0;
            std::memcpy(rec(P, d) + 21872, &zero, 4);
            flip0(d);
            std::memcpy(rec(P, d) + 68, savedPoint, 12);
        } else if (st == 2) {
            unsigned char* e = playerSelectInfo + k * 41;
            add(s.log, "init" + num(d) + "," + num(static_cast<signed char>(e[32])) + "," + reinterpret_cast<char*>(e) + "," +
                           num(static_cast<signed char>(e[33])) + "," + num(static_cast<signed char>(e[34])) + "," +
                           num(static_cast<signed char>(e[36])) + "," + num(static_cast<signed char>(e[35])) + "," +
                           reinterpret_cast<char*>(e + 37));
            rec(P, d)[0] = 0xa5;
            std::memcpy(rec(P, d) + 68, savedPoint, 12);
        } else if (st == 1) {
            add(s.log, "cp" + num(k));
            std::memcpy(rec(T, k), rec(P, k), kRec);
            rec(T, k)[1] ^= 0xff;
            std::memcpy(rec(P, d), rec(T, k), kRec);
            flip0(d);
            int zero = 0;
            std::memcpy(rec(P, d) + 21872, &zero, 4);
            std::memcpy(rec(P, d) + 68, pt, 12);
        }
        std::memcpy(rec(P, d) + 17768, block.data(), 4100);
        std::memcpy(rec(P, d) + 36, name.c_str(), name.size() + 1);
    }
    for (int p = 0; p < s.nm; ++p) {
        modelRemoveFlagged(s, p);
        for (auto& item : collected) {
            unsigned h = 0;
            for (unsigned char b : item) h = h * 31u + b;
            add(s.log, "add" + num(p) + "," + num(h) + ",1");
            unsigned char* inv = rec(P, p) + 9560;
            int n;
            std::memcpy(&n, inv + 4608, 4);
            std::memcpy(inv + n * 72, item.data(), 72);
            ++n;
            std::memcpy(inv + 4608, &n, 4);
        }
    }
    add(s.log, "dbg");
    add(s.log, "resetok10");
    modelFinish(s);
}

int compare(const State& want, const char* what) {
    if (log_ != want.log) { std::printf("%s: log\n got  %s\n want %s\n", what, log_.c_str(), want.log.c_str()); return 1; }
    if (std::memcmp(player, want.player.data(), sizeof player)) { std::printf("%s: player\n", what); return 2; }
    if (std::memcmp(tempPlayerStruct, want.temp.data(), sizeof tempPlayerStruct)) { std::printf("%s: temp\n", what); return 3; }
    if (std::memcmp(playerStartPos, want.startPos, sizeof want.startPos) || std::memcmp(playerStartYaw, want.startYaw, sizeof want.startYaw)) {
        std::printf("%s: start\n", what);
        return 4;
    }
    if (nmPlayers != want.nm || killPlayer != want.kill || netNetworkMode != want.net || gamePause != want.gamePause ||
        saveCheckPointCount != want.ckpt || particlesPaused != want.particles) {
        std::printf("%s: counters nm %d/%d net %d/%d gp %d/%d ck %d/%d pp %d/%d\n", what, nmPlayers, want.nm, netNetworkMode, want.net,
                    gamePause, want.gamePause, saveCheckPointCount, want.ckpt, particlesPaused, want.particles);
        return 5;
    }
    return 0;
}

int run(const char* what, int nm, int kill, int async, int sync, int import, std::initializer_list<int> status) {
    setupState(nm, 1);
    killPlayer = static_cast<signed char>(kill);
    asyncMode = async; syncState = sync; MC_FrontEndImport = static_cast<signed char>(import);
    int k = 0;
    for (int v : status) playerStatus[k++] = v;
    for (; k < 4; ++k) playerStatus[k] = 0;
    State want = snapshot();
    model(want);
    log_.clear();
    MC_CheckPlayerImportOrChange__Fv();
    return compare(want, what);
}
}

int main() {
    int rc = 0;
    rc |= run("kill first of 3", 3, 1, 7, 12, 0, {});
    rc |= run("kill middle of 4", 4, 2, 7, 12, 0, {});
    rc |= run("kill last of 4", 4, 4, 0, 0, 0, {});
    rc |= run("kill leaves one (net off)", 2, 1, 0, 0, 0, {});
    rc |= run("kill leaves two", 3, 3, 0, 0, 0, {});
    rc |= run("not async", 3, 0, 6, 12, 1, {});
    rc |= run("sync 7 copies positions", 3, 0, 7, 7, 1, {});
    rc |= run("sync other", 3, 0, 7, 5, 1, {1, 1, 1});
    rc |= run("import 3", 3, 0, 7, 12, 1, {});
    rc |= run("import 1", 1, 0, 7, 12, 3, {});
    rc |= run("import none", 0, 0, 7, 12, 1, {});
    rc |= run("change: all kept", 3, 0, 7, 12, 0, {1, 1, 1});
    rc |= run("change: pad skipped", 3, 0, 7, 12, 0, {1, 0, 1});
    rc |= run("change: join as status 3", 2, 0, 7, 12, 0, {1, 1, 3});
    rc |= run("change: join as status 2", 2, 0, 7, 12, 0, {1, 1, 2, 2});
    rc |= run("change: status 3 in slot 0", 2, 0, 7, 12, 0, {3, 1});
    rc |= run("change: first pad absent", 3, 0, 7, 12, 0, {0, 1, 2, 3});
    rc |= run("change: status 2 beyond count", 1, 0, 7, 12, 0, {1, 2});
    rc |= run("change: one player drops", 4, 0, 7, 12, 0, {1, 0, 1, 1});
    rc |= run("change: status 1 beyond count", 1, 0, 7, 12, 0, {1, 1, 1, 1});
    // random sweep over statuses with at least one earlier player present
    for (int i = 0; i < 400 && !rc; ++i) {
        int nm = 1 + static_cast<int>(rnd() % 4);
        int st[4];
        bool ok = false;
        for (int k = 0; k < 4; ++k) {
            st[k] = static_cast<int>(rnd() % 4);
            if (st[k] != 0 && k < nm) ok = true;
        }
        if (!ok) continue;
        char what[32];
        std::snprintf(what, sizeof what, "random %d", i);
        rc |= run(what, nm, 0, 7, 12, 0, {st[0], st[1], st[2], st[3]});
    }
    if (rc) return rc;
    std::puts("check player import or change passed");
    return 0;
}
