#include <climits>
#include <cstdio>
struct SkillResistPoison;
extern "C" int get_skill_point_cost(const SkillResistPoison*, int) asm("getSkillPointCost__17SkillResistPoisoni");
int main() { const SkillResistPoison* p = reinterpret_cast<const SkillResistPoison*>(static_cast<unsigned long>(0x1234)); const int v[]={INT_MIN,-1,0,1,17,INT_MAX}; for(int n:v) if(get_skill_point_cost(p,n)!=n) return 1; return 0; }
