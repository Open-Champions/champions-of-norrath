#include <cstring>
struct SkillDiseaseShield;
extern "C" void* lumpFindResource__FPcT0(char*, char*);
extern "C" void* get_icon(const SkillDiseaseShield*) asm("getIcon__18SkillDiseaseShield");
static int calls; static char lump[32], name[48]; static void* ret = reinterpret_cast<void*>(0x7788);
extern "C" void* lumpFindResource__FPcT0(char* l, char* n) { ++calls; std::strncpy(lump, l, 31); std::strncpy(name, n, 47); return ret; }
int main() {
    const SkillDiseaseShield* p = reinterpret_cast<const SkillDiseaseShield*>(static_cast<unsigned long>(0x1234));
    void* r = get_icon(p);
    if (r != ret || calls != 1) return 1;
    if (std::strcmp(lump, "icons.lmp") || std::strcmp(name, "icdiseaseshield.tex")) return 2;
    ret = nullptr; if (get_icon(p) != nullptr) return 3;
    return 0;
}
