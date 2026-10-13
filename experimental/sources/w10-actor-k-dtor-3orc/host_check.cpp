// Host contract for the Orc destructor; not an EE input. Callees are stubs that log.
#include "dtor.cpp"
#include <cstring>
#include <string>
unsigned char vt_class[8];
static std::string calls; static unsigned w72_at_call[2];
static unsigned char* the_object;
void listDestroy__FP8ListHead(void* h) {
    std::memcpy(&w72_at_call[0], the_object + 72, 4);
    calls += "list(" + std::to_string(static_cast<unsigned char*>(h) - the_object) + ");";
}
void base_destroy(void* o, int flags) {
    std::memcpy(&w72_at_call[1], the_object + 72, 4);
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
        const std::string want = std::string("list(1128);base(self,0);") + ((flags & 1) ? "free(self);" : "");
        if (calls != want || w72 != vt || w72_at_call[0] != vt || w72_at_call[1] != vt) bad = 1;
        for (int i = 0; i < 2048; ++i) if ((i < 72 || i >= 76) && o[i] != 0x3c) bad = 1;
    }
    return bad;
}
