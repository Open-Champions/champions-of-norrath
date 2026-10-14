#include "make_sample_packet.cpp"
#include <cassert>
#include <algorithm>
extern "C" { U64 copyScreen[1200]; unsigned char *copyScreenFrame,*copyScreenColor,*copyScreenAlpha; int engineDrawBufferStartAddress; }
static int flushed;
extern "C" int FlushCache(int n) {assert(n==0);++flushed;return 0;}
int main() {
 for(int full: {0,1}) {
  std::fill_n(copyScreen,1200,0xdeadbeefull);flushed=0;engineDrawBufferStartAddress=0x280000;
  makeSamplePacket__Fb(full);
  assert(flushed==1 && copyScreen[0]==0x60000240 && copyScreen[1]==0);
  assert(copyScreen[2]==0x100000000000823full && copyScreen[3]==14);
  assert(copyScreenFrame==reinterpret_cast<unsigned char*>(copyScreen+6));
  assert(copyScreenColor==reinterpret_cast<unsigned char*>(copyScreen+18));
  assert(copyScreenAlpha==reinterpret_cast<unsigned char*>(copyScreen+22)+4);
  const U64 prefix[]={0,69,0x10a0000,76,full?0x1bf027f0000ull:0xdf027f0000ull,64,full?0x1bf027f0000ull:0xdf027f0000ull,65,0x790000006c00ull,24,97,20,0x31001,71,0x80808080u,1,342,0,0x8000000064ull,66,0,63};
  assert(std::equal(prefix,prefix+22,copyScreen+4));
  unsigned n=26;
  for(unsigned pass=0;pass<2;++pass) {
   assert(copyScreen[n++]==(0x20000012a8250000ull | (0x2800u+pass*320))); assert(copyScreen[n++]==6);
   for(unsigned tile=0;tile<70;++tile) {
    unsigned x=tile%10*64,y=tile/10*64;
    for(unsigned corner=0;corner<2;++corner) {
     unsigned u=x+corner*64,v=y+corner*64;
     assert(copyScreen[n++]==((v*16+16)*65536ull+u*16+16));assert(copyScreen[n++]==3);
     U64 xyz=(0xa000ull<<20)|((30976ull+v*(full?16:8))*65536)|(pass?32768:27648)+u*8;
     assert(copyScreen[n++]==xyz);assert(copyScreen[n++]==5);
    }
   }
  }
  assert(copyScreen[n++]==1);assert(copyScreen[n++]==69);assert(copyScreen[n++]==0x3131757531317575ull);assert(copyScreen[n++]==68);
  assert(n==1154 && copyScreen[n]==0xdeadbeef);
 }
}
