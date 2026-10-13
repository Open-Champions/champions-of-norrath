#include <cstdio>
extern "C" void init_ramps(void*) asm("initRamps__5Skill");
int main() {
    alignas(8) unsigned char bytes[80];
    for (unsigned i=0;i<80;++i) bytes[i]=0xa5;
    init_ramps(bytes);
    for (unsigned i=0;i<80;++i) {
        unsigned char expected=0xa5;
        for(unsigned j=0;j<5;++j) {
            if(i>=14+2*j && i<=15+2*j) expected=0;
            if(i>=24+2*j && i<=25+2*j) expected=(i%2==0)?1:0;
            if(i>=36+4*j && i<=39+4*j) { const unsigned char two[4]={0,0,0,0x40}; expected=two[i-(36+4*j)]; }
        }
        if(bytes[i]!=expected) { std::fprintf(stderr,"bad byte %u\n",i); return 1; }
    }
    return 0;
}
