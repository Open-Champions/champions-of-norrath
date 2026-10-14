#include <cstdio>
#include <cstring>

extern "C" void dtor_15FxCreatureState(void* object, int flags) asm("_$_15FxCreatureState");

static int failures = 0;
static int deletes = 0;
static void* deleted = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void __builtin_delete(void* pointer) {
    ++deletes;
    deleted = pointer;
}

int main() {
    unsigned char object[32];
    unsigned char before[32];
    std::memset(object, 0x6b, sizeof object);
    std::memcpy(before, object, sizeof object);
    dtor_15FxCreatureState(object, 0);
    expect(deletes == 0, "flag zero does not delete");
    dtor_15FxCreatureState(object, 2);
    expect(deletes == 0, "flag two (the embedded-member call) does not delete");
    dtor_15FxCreatureState(object, 1);
    expect(deletes == 1 && deleted == object, "flag one deletes the object");
    dtor_15FxCreatureState(object, 3);
    expect(deletes == 2 && deleted == object, "any odd flag deletes");
    dtor_15FxCreatureState(object, -1);
    expect(deletes == 3, "negative flag has bit 0 set");
    dtor_15FxCreatureState(object, -2);
    expect(deletes == 3, "-2 has bit 0 clear");
    expect(std::memcmp(object, before, sizeof object) == 0, "object bytes are never written");
    return failures == 0 ? 0 : 1;
}
