// Host contract for the destructor in dtor.cpp; not an EE input. The base destructor and the
// free routine are stubs that log; the contract checks the table store, the flag passed to the
// base, the order, and that only bit 0 of the incoming flags decides the free.
#include "dtor.cpp"
#include <cstring>
#include <string>
unsigned char vt_class[8];
static std::string calls; static unsigned w72_at_base;
static unsigned char* the_object;
void base_destroy(void* o, int flags) {
    std::memcpy(&w72_at_base, static_cast<unsigned char*>(o) + 72, 4);
    calls += std::string("base(") + (o == the_object ? "self" : "other") + "," + std::to_string(flags) + ");";
}
void blockFree__FPv(void* o) { calls += std::string("free(") + (o == the_object ? "self" : "other") + ");"; }
int main() {
    alignas(16) static unsigned char o[2048];
    int bad = 0;
    the_object = o;
    const unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_class));
    for (int flags : {0, 1, 2, 3, -1, -2, 0x100, 0x101}) {
        std::memset(o, 0x3c, sizeof o); calls.clear();
        destroy(o, flags);
        unsigned w72; std::memcpy(&w72, o + 72, 4);
        const std::string want = std::string("base(self,0);") + ((flags & 1) ? "free(self);" : "");
        if (calls != want || w72 != vt || w72_at_base != vt) bad = 1;
        for (int i = 0; i < 2048; ++i) if ((i < 72 || i >= 76) && o[i] != 0x3c) bad = 1;
    }
    return bad;
}
