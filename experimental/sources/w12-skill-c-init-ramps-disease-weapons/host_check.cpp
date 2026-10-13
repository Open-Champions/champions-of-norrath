extern "C" void init_ramps(void*) asm("initRamps__19SkillDiseaseWeapons");
int main() { alignas(8) unsigned char b[80]; for(unsigned i=0;i<80;i++) b[i]=0xa5; init_ramps(b); for(unsigned i=0;i<80;i++) { unsigned char e=0xa5; if(i==56) e=1; if(i==57) e=0; if(i==66) e=4; if(i==67) e=0; if(i>=76&&i<=79) { const unsigned char x[4]={0,0,0,0x40}; e=x[i-76]; } if(b[i]!=e) return 1; } return 0; }
