/* Observed makeSamplePacket__Fb, 0x0018f1f8, 1492 bytes. Two tiled
 * passes use the same texture coordinates and different destination X bases.
 * Leaves frame/color/alpha pointers into the packet for doSuperSample. */
using U64 = unsigned long long;
extern "C" U64 copyScreen[];
extern "C" unsigned char *copyScreenFrame, *copyScreenColor, *copyScreenAlpha;
extern "C" int engineDrawBufferStartAddress;
extern "C" int FlushCache(int);
extern "C" void makeSamplePacket__Fb(int full) {
    U64* p=copyScreen;
    unsigned n=4;
    p[1]=0; p[2]=0x1000000000008000ull; p[3]=14;
    auto put=[&](U64 value,U64 reg) {p[n++]=value;p[n++]=reg;};
    put(0,69);
    copyScreenFrame=reinterpret_cast<unsigned char*>(p+n);
    put(0x10a0000,76);
    U64 scissor=full?0x1bf027f0000ull:0xdf027f0000ull;
    put(scissor,64);put(scissor,65);
    put(0x790000006c00ull,24);put(97,20);put(0x31001,71);
    copyScreenColor=reinterpret_cast<unsigned char*>(p+n);
    put(0x80808080u,1);put(342,0);
    copyScreenAlpha=reinterpret_cast<unsigned char*>(p+n)+4;
    put(0x8000000064ull,66);put(0,63);
    const U64 flags=0x28000000ull|0x250000ull|(0x8000ull<<46)|(0x8000ull<<21)|(0xa000ull<<18);
    for(unsigned pass=0;pass<2;++pass) {
        const int base=(engineDrawBufferStartAddress>>8)+static_cast<int>(pass*320);
        put(static_cast<U64>(static_cast<long long>(base))|flags,6);
        const unsigned xbase=pass?0x8000:0x6c00;
        for(unsigned y=0;y<448;y+=64) for(unsigned x=0;x<640;x+=64) {
            put(((y*16+16)<<16)|(x*16+16),3);
            put((0xa000ull<<20)|((0x7900ull+y*(full?16:8))<<16)|(xbase+x*8),5);
            put((((y+64)*16+16)<<16)|((x+64)*16+16),3);
            put((0xa000ull<<20)|((0x7900ull+(y+64)*(full?16:8))<<16)|(xbase+(x+64)*8),5);
        }
    }
    put(1,69);put(0x3131757531317575ull,68);
    p[0]=0x60000000u|(n/2-1);p[2]|=n/2-2;
    FlushCache(0);
}
