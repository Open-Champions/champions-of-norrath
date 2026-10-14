#define NORRATH_HOST_CONTRACT
#include "frame_start_packet.cpp"
#include <cassert>
#include <algorithm>
extern "C" { U64 frameStatePacket[256]; int engineDrawBufferStartAddress,engineYOffset,engineScreenSlide,engineScissory1,engineScissory2; void* dmaGif; }
static int calls;
extern "C" void sceDmaSend(void* c,void* p) { assert(c==dmaGif && p==frameStatePacket && calls++==0); }
extern "C" int sceGsSyncPath(int a,int b) { assert(!a&&!b&&calls++==1); return 0; }
int main() {
 for(int full: {0,1}) for(int field: {0,1}) for(unsigned color: {0u,0x87654321u}) {
  std::fill_n(frameStatePacket,256,0xdeadbeefull); calls=0;
  engineDrawBufferStartAddress=0x280000;engineYOffset=123;engineScreenSlide=-3;engineScissory1=7;engineScissory2=215;
  frameStartPacket__FUibi(color,full,field);
  unsigned pairs=12+(color?43:0)+(full?0:2);
  assert(calls==2 && frameStatePacket[0]==(0x60000000u| (pairs-1)));
  assert(frameStatePacket[1]==0 && frameStatePacket[2]==(0x1000000000008000ull | (pairs-2)) && frameStatePacket[3]==14);
  const U64 fixed[]={0x2140140,76,0x20001cc,78,0x1bf04ff0000ull,64,0x1bf04ff0000ull,65,(123ull<<32)|0x5800,24,(123ull<<32)|0x5800,25,1,69,0,73,1,70,1,26};
  assert(std::equal(fixed,fixed+20,frameStatePacket+4));
  unsigned n=24;
  if(color) {
   assert(frameStatePacket[n++]==color);assert(frameStatePacket[n++]==1);
   assert(frameStatePacket[n++]==0x387f4);assert(frameStatePacket[n++]==71);
   assert(frameStatePacket[n++]==6);assert(frameStatePacket[n++]==0);
   for(unsigned x=0;x<20;++x) {
    assert(frameStatePacket[n++]==(0x72005800ull+x*1024));assert(frameStatePacket[n++]==5);
    assert(frameStatePacket[n++]==((36304ull<<16)|(0x5c00ull+x*1024))); // replaced below
    assert(frameStatePacket[n++]==5);
   }
  }
  if(!full) for(unsigned r:{64u,65u}) {assert(frameStatePacket[n++]==((215ull<<48)|(7ull<<32)|0x4ff0000));assert(frameStatePacket[n++]==r);}
  assert(n==pairs*2 && frameStatePacket[n]==0xdeadbeef);
  assert(engineYOffset==(field?0x7208:0x7200)+(full?0:-48));
 }
}
