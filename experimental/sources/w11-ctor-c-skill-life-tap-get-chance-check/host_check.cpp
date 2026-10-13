// Host contract for getChance__12SkillLifeTapi, whose source is
// w10-item-a-skill-life-tap-get-chance/get_chance.cpp (written without a committed check). Not an EE input.
// The two table entries are planted at table + 120 and table + 88 with different adjustments, so a swapped
// order, a wrong adjustment or a wrong argument fails.
#include "../w10-item-a-skill-life-tap-get-chance/get_chance.cpp"
#include <cstdio>
#include <cstring>
#include <string>

struct Entry { short adjust; short unused; int (*function)(void*, int, int); };
static std::string calls; static void* selfSeen[2]; static int argA[2], argB[2];
static int ret120 = 0, ret88 = 0;
static int fn120(void* s, int a, int b) { calls += "120;"; selfSeen[0] = s; argA[0] = a; argB[0] = b; return ret120; }
static int fn88(void* s, int a, int b) { calls += "88;"; selfSeen[1] = s; argA[1] = a; argB[1] = b; return ret88; }

static int run(int rank, int r120, int r88, float want) {
    int bad = 0;
    alignas(16) unsigned char skill[128], table[256];
    std::memset(skill, 0, sizeof skill); std::memset(table, 0, sizeof table);
    Entry e120{24, 0, fn120}, e88{-8, 0, fn88};
    std::memcpy(table + 120, &e120, sizeof e120); std::memcpy(table + 88, &e88, sizeof e88);
    char* t = reinterpret_cast<char*>(table);
    std::memcpy(skill + 96, &t, sizeof t);
    calls.clear(); ret120 = r120; ret88 = r88;
    const float got = getChance__12SkillLifeTapi(skill, rank);
    if (calls != "120;88;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    if (selfSeen[0] != skill + 24 || argA[0] != rank || argB[0] != 1) { std::puts("fail: first entry arguments"); bad = 1; }
    if (selfSeen[1] != skill - 8 || argA[1] != 0 || argB[1] != r120) { std::puts("fail: second entry arguments"); bad = 1; }
    if (got != want) { std::printf("fail: result %g want %g\n", got, want); bad = 1; }
    return bad;
}
int main() {
    int bad = 0;
    bad |= run(3, 77, 250, 250.0f * 0.01f);
    bad |= run(-1, -5, -40, -40.0f * 0.01f);
    bad |= run(0, 0, 0, 0.0f);
    return bad;
}
