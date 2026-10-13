#include <cstdio>
extern "C" void init_ramps(void*) asm("initRamps__18SkillPoisonWeapons");
int main() { alignas(8) unsigned char bytes[80]; for(unsigned i=0;i<80;i++) bytes[i]=0xa5; init_ramps(bytes); for(unsigned i=0;i<80;i++) { unsigned char want=0xa5; if(i==56) want=1; if(i==57) want=0; if(i==66) want=2; if(i==67) want=0; if(i>=76&&i<=79) { const unsigned char v[4]={0,0,0,0x40}; want=v[i-76]; } if(bytes[i]!=want) { std::fprintf(stderr,"offset %u: got %u want %u\n",i,bytes[i],want); return 1; } } return 0; }
