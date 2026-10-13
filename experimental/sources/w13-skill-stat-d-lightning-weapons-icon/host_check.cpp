#include <cstring>
struct SkillLightningWeapons;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillLightningWeapons*) asm("getIcon__21SkillLightningWeapons");
static int calls; static char lump[32], name[48]; static void* ret = reinterpret_cast<void*>(0x7788);
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { ++calls; std::strncpy(lump, l, 31); std::strncpy(name, n, 47); return ret; }
int main() {
    const SkillLightningWeapons* p = reinterpret_cast<const SkillLightningWeapons*>(static_cast<unsigned long>(0x1234));
    void* r = get_icon(p);
    if (r != ret || calls != 1) return 1;
    if (std::strcmp(lump, "icons.lmp") || std::strcmp(name, "iclightningweapons.tex")) return 2;
    ret = nullptr; if (get_icon(p) != nullptr) return 3;
    return 0;
}
