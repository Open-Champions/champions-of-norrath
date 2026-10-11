#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern unsigned char __ti8Creature[12];
void* __tf8Creature();
unsigned char norrath_ti_10GameObject[8];
}

static int base_calls = 0;
static int rtti_calls = 0;
static void* seen_ti = nullptr;
static std::string seen_name;
static void* seen_base = nullptr;
static int failures = 0;

extern "C" void* norrath_vt_tf_10GameObject() {
    ++base_calls;
    return norrath_ti_10GameObject;
}
extern "C" void __rtti_si(void* typeinfo, const char* name, void* base) {
    ++rtti_calls;
    seen_ti = typeinfo;
    seen_name = name;
    seen_base = base;
}

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    std::memset(__ti8Creature, 0, sizeof __ti8Creature);
    void* returned = __tf8Creature();
    expect(returned == __ti8Creature, "returns the creature type-info object");
    expect(base_calls == 1, "initializes GameObject type info first");
    expect(rtti_calls == 1, "registers single inheritance");
    expect(seen_ti == __ti8Creature, "type-info argument");
    expect(seen_name == "8Creature", "name argument");
    expect(seen_base == norrath_ti_10GameObject, "GameObject base");
    __ti8Creature[0] = 1;
    returned = __tf8Creature();
    expect(returned == __ti8Creature, "second return");
    expect(base_calls == 1 && rtti_calls == 1, "nonzero first word skips both calls");
    return failures ? 1 : 0;
}
