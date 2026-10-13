// Host contract for initType__5Mummyb; not an EE input. 32-bit words are offsets from a host base and the callees
// log. The authored function was also compared with the original in a private R5900 run (72 scenarios over every
// type and mode, flag values and skill results; 270 of 299 original words reached - the rest are alignment padding
// and the record-search branches that are dead because the four records were cleared a few steps earlier).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static int skillAmount;
unsigned w12_spell_c_host_call(unsigned, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2) {
    calls += "skill(" + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) + "," + std::to_string(argument_count) + "," +
             std::to_string(a1) + "," + std::to_string(a2) + ");";
    return static_cast<unsigned>(skillAmount);
}
extern "C" { unsigned skills[32]; float worldDifficulty = 2.9f; }
#include "init_type.cpp"

static int roll;
static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
int eRand__10GameObject(void*) { calls += "r;"; return roll; }
void setParameters__8Creaturefffffi(void*, float a, float b, float c, float d, float e, int flag) {
    calls += "set(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + std::to_string(flag) + ");";
}
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const mummy = arena + 0x100;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static short get16(unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
static void reset(unsigned type, int mode) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    std::memset(skills, 0, sizeof skills);
    calls.clear(); roll = 32768; skillAmount = 100;
    put32(mummy + 960, type); put32(mummy + 208, static_cast<unsigned>(mode));
    for (int slot : {13, 14, 25}) {
        unsigned char* skill = arena + 0x1000 + 0x200 * slot;
        skills[slot] = static_cast<unsigned>(skill - arena);
        put32(skill + 96, static_cast<unsigned>(arena + 0x3000 + 0x40 * slot - arena));
        short adjust = 8; std::memcpy(arena + 0x3000 + 0x40 * slot + 96, &adjust, 2);
        put32(arena + 0x3000 + 0x40 * slot + 100, 0x99);
    }
}

int main() {
    int bad = 0;
    struct Row { unsigned type; const char* call; bool second; };
    const Row rows[] = {
        {0, "set(1,1,1,1,1,0);", false}, {1, "set(1,1,1,1,1,0);", true}, {2, "set(1.2,1.2,1.2,1.2,1.2,0);", false},
        {3, "set(0.8,1,1,1,1,0);", true}, {4, "set(1.5,1.3,1.4,1.2,1.3,0);", false}, {5, "set(1.3,1,1,1.2,1.2,0);", true},
    };
    for (const Row& row : rows) {
        reset(row.type, 0);
        mummy[156] = 99;
        initType__5Mummyb(mummy, true);
        const std::string call = row.call;   // the flag true gives the last argument 0
        if (calls != call) { std::puts(calls.c_str()); std::printf("fail: type %u calls\n", row.type); bad = 1; }
        if (getf(mummy + 88) != -0.2f && std::memcmp(mummy + 88, "\xcc\xcc\x4c\xbe", 4) != 0) { std::puts("fail: +88"); bad = 1; }
        if (getf(mummy + 100) != 0.5f) { std::puts("fail: +100"); bad = 1; }
        if (row.second && (mummy[156] != 6 || getf(mummy + 104) < 0.19f || getf(mummy + 104) > 0.21f)) { std::puts("fail: second group"); bad = 1; }
        if (!row.second && (mummy[156] != 0 || get32(mummy + 104) != 0)) { std::puts("fail: first group"); bad = 1; }
        for (int k = 0; k < 4; ++k) if (get16(mummy + 1068 + 8 * k) != -1) { std::puts("fail: records not cleared"); bad = 1; }
    }
    reset(7, 0);
    initType__5Mummyb(mummy, false);
    if (!calls.empty() || get32(mummy + 88) != 0) { std::puts("fail: large type"); bad = 1; }
    reset(0, 0);
    initType__5Mummyb(mummy, false);
    if (calls.find(",1);") == std::string::npos) { std::puts("fail: flag false -> 1"); bad = 1; }
    // mode 1: skill slot 25, id 38; amount 100, bonus (int)(100 * 0.2 * 0.5) = 9 (floats), byte 7
    reset(0, 1);
    initType__5Mummyb(mummy, true);
    {
        const std::string want = "set(1,1,1,1,1,0);skill(" + std::to_string(skills[25] + 8) + ",2,0,2);r;";
        if (calls != want) { std::puts(calls.c_str()); std::puts("fail: mode 1 calls"); bad = 1; }
        unsigned char* rec = mummy + 1068;
        if (get16(rec) != 38 || get16(rec + 2) != 100 || get16(rec + 4) != 109 || get16(rec + 6) != 0 || mummy[156] != 7) { std::puts("fail: mode 1 record"); bad = 1; }
    }
    reset(2, 2);
    initType__5Mummyb(mummy, true);
    if (calls.find("skill(" + std::to_string(skills[14] + 8)) == std::string::npos || get16(mummy + 1068) != 22 || mummy[156] != 3) { std::puts("fail: mode 2 type 2"); bad = 1; }
    reset(1, 2);
    initType__5Mummyb(mummy, true);
    if (calls.find("skill(" + std::to_string(skills[13] + 8)) == std::string::npos || get16(mummy + 1068) != 21 || mummy[156] != 2) { std::puts("fail: mode 2 other type"); bad = 1; }
    reset(9, 3);
    initType__5Mummyb(mummy, true);
    if (mummy[156] != 8 || get32(mummy + 212) != 0) { std::puts("fail: mode 3"); bad = 1; }
    reset(9, 4);
    initType__5Mummyb(mummy, true);
    if (mummy[156] != 8 || get32(mummy + 212) != 1) { std::puts("fail: mode 4"); bad = 1; }
    reset(9, 5);
    initType__5Mummyb(mummy, true);
    if (mummy[156] != 0 || get32(mummy + 212) != 0) { std::puts("fail: mode 5"); bad = 1; }
    return bad;
}
