// Host contract for __tf13MissileWeapon; not an EE input. Callees record a call log; the base nodes owned by other sources
// are defined here. Each combination of zero and nonzero first words is checked against the sequence stated in
// typeinfo.cpp. Returns non-zero when wrong.
#include "typeinfo.cpp"
#include <cstring>
#include <string>
unsigned char __ti10Projectile[12];
unsigned char __ti10GameObject[8];
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "tf;"; return nullptr; }
extern "C" void __rtti_si(void* typeinfo, const char* name, const void* base) {
    auto id = [](const void* p) -> std::string {
        if (p == __ti13MissileWeapon) return "MW";
        if (p == __ti10Projectile) return "PR";
        return p == __ti10GameObject ? "GO" : "?";
    };
    calls += "rtti(" + id(typeinfo) + "," + name + "," + id(base) + ");";
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}
int main() {
    int bad = 0;
    for (int mw = 0; mw < 2; ++mw) for (int pr = 0; pr < 2; ++pr) {
        std::memset(__ti13MissileWeapon, 0, 12); __ti13MissileWeapon[0] = mw; __ti10Projectile[0] = pr;
        calls.clear();
        if (__tf13MissileWeapon() != __ti13MissileWeapon) bad = 1;
        std::string want;
        if (!mw) {
            if (!pr) want += "tf;rtti(PR,10Projectile,GO);";
            want += "rtti(MW,13MissileWeapon,PR);";
        }
        if (calls != want) bad = 1;
    }
    return bad;
}
