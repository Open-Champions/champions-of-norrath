// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_icon.cpp
#include <cstring>
struct SkillDiseaseTrail;
extern "C" void* get_icon(const SkillDiseaseTrail*) asm("getIcon__17SkillDiseaseTrail");
static int calls = 0;
static char lump[32], name[32];
static int sentinel;
extern "C" void* lump_find_resource(const char* a, const char* b) asm("lumpFindResource__FPcT0");
extern "C" void* lump_find_resource(const char* a, const char* b) {
    ++calls;
    std::strncpy(lump, a, sizeof lump - 1);
    std::strncpy(name, b, sizeof name - 1);
    return &sentinel;
}
int main() {
    const SkillDiseaseTrail* receiver = reinterpret_cast<const SkillDiseaseTrail*>(static_cast<unsigned long>(0x1234));
    if (get_icon(receiver) != &sentinel) return 1;
    if (calls != 1) return 2;
    if (std::strcmp(lump, "icons.lmp") != 0) return 3;
    if (std::strcmp(name, "icdiseasetrail.tex") != 0) return 4;
    return 0;
}
