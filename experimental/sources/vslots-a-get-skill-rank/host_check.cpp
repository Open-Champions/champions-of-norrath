#include <cstdio>
#include <cstring>

extern "C" int getSkillRank__8Creaturei(void* self, int rank);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char object[8];
    std::memset(object, 0x5a, sizeof object);
    expect(getSkillRank__8Creaturei(object, 7) == 0, "rank 7 returns zero");
    expect(getSkillRank__8Creaturei(object, -1) == 0, "negative rank returns zero");
    expect(getSkillRank__8Creaturei(nullptr, 0) == 0, "null creature returns zero");
    bool intact = true;
    for (unsigned char byte : object) if (byte != 0x5a) intact = false;
    expect(intact, "creature bytes stay put");
    return failures ? 1 : 0;
}
