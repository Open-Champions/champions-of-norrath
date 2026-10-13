#include <cstdint>
struct SkillResistPoison;
extern "C" std::uint32_t get_level_requirement(const SkillResistPoison*, std::uint32_t) asm("getLevelRequirement__17SkillResistPoisoni");
int main() { const SkillResistPoison* p = reinterpret_cast<const SkillResistPoison*>(static_cast<unsigned long>(0x1234)); const std::uint32_t v[]={0u,1u,2u,0x3fffffffu,0x40000000u,0x80000000u,0xffffffffu}; for(auto n:v) if(get_level_requirement(p,n)!=(n<<2)) return 1; return 0; }
