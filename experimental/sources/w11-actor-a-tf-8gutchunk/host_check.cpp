// Host contract for __tf8GutChunk; not an EE input. Callees record a call log; the base nodes owned by other sources
// are defined here. Each combination of zero and nonzero first words is checked against the sequence stated in
// typeinfo.cpp. Returns non-zero when wrong.
#include "typeinfo.cpp"
#include <cstring>
#include <string>
unsigned char __ti6PhProp[12];
unsigned char __ti4Prop[12];
unsigned char __ti10GameObject[8];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "tf;"; return nullptr; }
extern "C" void __rtti_si(void* typeinfo, const char* name, const void* base) {
    auto id = [](const void* p) -> std::string {
        if (p == __ti8GutChunk) return "GC";
        if (p == __ti6PhProp) return "PP";
        if (p == __ti4Prop) return "P";
        return p == __ti10GameObject ? "GO" : "?";
    };
    calls += "rtti(" + id(typeinfo) + "," + name + "," + id(base) + ");";
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}
int main() {
    int bad = 0;
    for (int gc = 0; gc < 2; ++gc) for (int pp = 0; pp < 2; ++pp) for (int pr = 0; pr < 2; ++pr) {
        std::memset(__ti8GutChunk, 0, 12); __ti8GutChunk[0] = gc; __ti6PhProp[0] = pp; __ti4Prop[0] = pr;
        calls.clear();
        if (__tf8GutChunk() != __ti8GutChunk) bad = 1;
        std::string want;
        if (!gc) {
            if (!pp) {
                if (!pr) want += "tf;rtti(P,4Prop,GO);";
                want += "rtti(PP,6PhProp,P);";
            }
            want += "rtti(GC,8GutChunk,PP);";
        }
        if (calls != want) bad = 1;
    }
    return bad;
}
