// Host contract for _$_19SpellSummonSkeleton; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_SummonSkeleton[8];
unsigned char vt_Spell[8];
static std::string calls;
static unsigned char* current;
static int failures;

static const char* tagOf(const unsigned char* object) {
    unsigned table; std::memcpy(&table, object + 20, 4);
    if (table == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_SummonSkeleton))) return "19Summon";
    if (table == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Spell))) return "5Spell";
    return "?";
}

void listDestroy__FP8ListHead(void* head) {
    char b[64];
    std::snprintf(b, sizeof b, "list(%ld)@%s;", static_cast<long>(static_cast<unsigned char*>(head) - current), tagOf(current));
    calls += b;
}
void blockFree__FPv(void* block) { calls += block == current ? "free;" : "free-other;"; }

int main() {
    alignas(16) unsigned char object[64];
    current = object;
    const int cases[] = {0, 1, 2, 3, static_cast<int>(0x80000001u), 0x7ffffffe, -1};
    for (int flags : cases) {
        std::memset(object, 0xee, sizeof object);
        calls.clear();
        destructor_SummonSkeleton(object, flags);
        const bool wantFree = (flags & 1) != 0;
        const std::string want = std::string("list(24)@19Summon;list(4)@5Spell;") + (wantFree ? "free;" : "");
        if (tagOf(object) != std::string("5Spell")) { std::printf("fail: final table flags %d\n", flags); ++failures; }
        if (calls != want) { std::printf("fail: flags %d calls %s\n", flags, calls.c_str()); ++failures; }
        if (object[0] != 0xee || object[16] != 0xee || object[28] != 0xee) { std::printf("fail: bytes touched flags %d\n", flags); ++failures; }
    }
    if (failures == 0) std::puts("ok");
    return failures;
}
