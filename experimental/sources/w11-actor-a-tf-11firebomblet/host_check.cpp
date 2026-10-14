// Host contract for __tf11FireBomblet; not an EE input. Callees record a call log; the base nodes owned by other sources
// are defined here. Each combination of zero and nonzero first words is checked against the sequence stated in
// typeinfo.cpp. Returns non-zero when wrong.
#include "typeinfo.cpp"
#include <cstring>
#include <string>

unsigned char __ti10GameObject[8];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "tf;"; return nullptr; }
extern "C" void __rtti_si(void* typeinfo, const char* name, const void* base) {
    auto id = [](const void* p) -> std::string {
        if (p == __ti11FireBomblet) return "FB";
        return p == __ti10GameObject ? "GO" : "?";
    };
    calls += "rtti(" + id(typeinfo) + "," + name + "," + id(base) + ");";
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}
int main() {
    int bad = 0;
    for (int on = 0; on < 2; ++on) {
        std::memset(__ti11FireBomblet, 0, 12); __ti11FireBomblet[4] = 9; __ti11FireBomblet[0] = on;
        calls.clear();
        if (__tf11FireBomblet() != __ti11FireBomblet) bad = 1;
        if (calls != (on ? "" : "tf;rtti(FB,11FireBomblet,GO);")) bad = 1;
        calls.clear();
        if (__tf11FireBomblet() != __ti11FireBomblet || !calls.empty()) bad = 1;   // now registered
    }
    return bad;
}
