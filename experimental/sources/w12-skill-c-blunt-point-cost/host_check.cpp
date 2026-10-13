#include <climits>
#include <cstdio>
struct SkillBlunt;
extern "C" int get_skill_point_cost(const SkillBlunt*, int) asm("getSkillPointCost__10SkillBlunti");
int main() { const SkillBlunt* p = reinterpret_cast<const SkillBlunt*>(static_cast<unsigned long>(0x1234)); const int v[]={INT_MIN,-1,0,1,17,INT_MAX}; for(int n:v) if(get_skill_point_cost(p,n)!=n) return 1; return 0; }
