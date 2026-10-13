// Host contract for _$_7OniBall; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_7OniBall[8];
unsigned char vt_10Projectile[8];
static std::string calls;
static unsigned char* current;
static int failures;
static const char* table_name(unsigned word) {
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_7OniBall))) return "7OniBall";
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10Projectile))) return "10Projectile";
    return word == 0x3c3c3c3cu ? "-" : "?";
}
static unsigned word_at(int off) { unsigned w; std::memcpy(&w, current + off, 4); return w; }
static void snap(const char* what, int off, long long arg) {
    char b[96]; std::snprintf(b, sizeof b, "%s@%s(%lld);", what, table_name(word_at(off)), arg); calls += b;
}
void game_object_destructor(void*, int f) { snap("go", 72, f); }
void light_destructor(void* s, int f) { snap("light", 72, (static_cast<unsigned char*>(s) - current) * 16 + f); }
void listDestroy__FP8ListHead(void* s) { snap("list", 72, static_cast<unsigned char*>(s) - current); }
void blockFree__FPv(void* s) { snap("free", 72, s == current); }
int main() {
    alignas(16) unsigned char object[160];
    current = object;
    const int cases[] = {0, 1, 2, 3, 4, 5, static_cast<int>(0x80000001u), 0x7ffffffe};
    const char* want[] = {
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);free@10Projectile(1);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);free@10Projectile(1);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);free@10Projectile(1);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);free@10Projectile(1);",
        "light@7OniBall(1922);list@10Projectile(76);go@10Projectile(0);",
    };
    for (int i = 0; i < 8; ++i) {
        std::memset(object, 0x3c, sizeof object);
        calls.clear();
        destructor_7OniBall(object, cases[i]);
        if (calls != want[i]) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", cases[i], calls.c_str(), want[i]); ++failures; }
    }
    if (!failures) std::puts("dtor7OniBall ok");
    return failures ? 1 : 0;
}
