#include <cstdint>
struct SkillBlunt;
extern "C" std::uint32_t get_level_requirement(const SkillBlunt*, std::uint32_t) asm("getLevelRequirement__10SkillBlunti");
int main() { const SkillBlunt* p = reinterpret_cast<const SkillBlunt*>(static_cast<unsigned long>(0x1234)); const std::uint32_t v[]={0u,1u,2u,0x3fffffffu,0x40000000u,0x80000000u,0xffffffffu}; for(auto n:v) if(get_level_requirement(p,n)!=(n<<2)) return 1; return 0; }
