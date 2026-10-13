// Host contract for the Innoruuk destructor; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned char vt_Innoruuk[8];
alignas(16) static unsigned char object[1100];
static std::string order;
static std::vector<unsigned> wordSeen;
static int frees, failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (order %s)\n", what, order.c_str()); ++failures; } }
void light_destructor(void* a, int f) { order += "light_destructor@" + std::to_string(static_cast<unsigned char*>(a) - object) + "/" + std::to_string(f) + ";"; wordSeen.push_back(*reinterpret_cast<unsigned*>(object + 72)); }
void list_destroy(void* a) { order += "list_destroy@" + std::to_string(static_cast<unsigned char*>(a) - object); }
void creature_destructor(void* a, int f) { order += "creature_destructor@" + std::to_string(static_cast<unsigned char*>(a) - object) + "/" + std::to_string(f) + ";"; wordSeen.push_back(*reinterpret_cast<unsigned*>(object + 72)); }
void blockFree__FPv(void* a) { ++frees; order += "F"; expect(a == object, "frees the same object"); }
int main() {
    std::memset(object, 0x3c, sizeof object);
    const unsigned wanted = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_Innoruuk));
    innoruuk_destructor(object, 0);
    expect(order == "light_destructor@1016/2;list_destroy@988creature_destructor@0/0;" && frees == 0, "flag 0: parts in order, no free");
    for (unsigned w : wordSeen) expect(w == wanted, "table pointer stored before any part destructor");
    expect(!wordSeen.empty(), "part destructors ran");
    order.clear();
    innoruuk_destructor(object, 2);
    expect(order == "light_destructor@1016/2;list_destroy@988creature_destructor@0/0;" && frees == 0, "flag 2 does not free");
    order.clear();
    innoruuk_destructor(object, 1);
    expect(order == "light_destructor@1016/2;list_destroy@988creature_destructor@0/0;F" && frees == 1, "flag 1 frees last");
    order.clear();
    innoruuk_destructor(object, 5);
    expect(order == "light_destructor@1016/2;list_destroy@988creature_destructor@0/0;F" && frees == 2, "any odd flag frees; part flags stay constant");
    if (!failures) std::puts("Innoruuk dtor ok");
    return failures ? 1 : 0;
}
