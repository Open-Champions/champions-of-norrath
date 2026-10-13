// Host contract for _$_13LooseIceChunk; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_4Prop[8];
static std::string calls;
static unsigned char* current;
static int failures;
static const char* table_name(unsigned word) {
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_4Prop))) return "4Prop";
    return word == 0x3c3c3c3cu ? "-" : "?";
}
static unsigned word_at(int off) { unsigned w; std::memcpy(&w, current + off, 4); return w; }
static void snap(const char* what, int off, long long arg) {
    char b[96]; std::snprintf(b, sizeof b, "%s@%s(%lld);", what, table_name(word_at(off)), arg); calls += b;
}
void game_object_destructor(void*, int f) { snap("go", 72, f); }
void blockFree__FPv(void* s) { snap("free", 72, s == current); }
int main() {
    alignas(16) unsigned char object[160];
    current = object;
    const int cases[] = {0, 1, 2, 3, 4, 5, static_cast<int>(0x80000001u), 0x7ffffffe};
    const char* want[] = {
        "go@4Prop(0);",
        "go@4Prop(0);free@4Prop(1);",
        "go@4Prop(0);",
        "go@4Prop(0);free@4Prop(1);",
        "go@4Prop(0);",
        "go@4Prop(0);free@4Prop(1);",
        "go@4Prop(0);free@4Prop(1);",
        "go@4Prop(0);",
    };
    for (int i = 0; i < 8; ++i) {
        std::memset(object, 0x3c, sizeof object);
        calls.clear();
        destructor_13LooseIceChunk(object, cases[i]);
        if (calls != want[i]) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", cases[i], calls.c_str(), want[i]); ++failures; }
    }
    if (!failures) std::puts("dtor13LooseIceChunk ok");
    return failures ? 1 : 0;
}
