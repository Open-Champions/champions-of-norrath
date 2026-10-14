// Observed class static storage of item, used by picProcess__4itemv (w8-hud-text-items-b-pic-process):
//   _4item$picsToLoad      0x00483200   1 byte, small data, 0
//   _4item$picInProgress   0x00483204   4 bytes, small data, 0xfffe7960 (-100000)
//   _4item$picLoadFinished 0x00483208   1 byte, small data, 0
//   _4item$picBuffer       0x00487080   uninitialized data, symbol size 0
//   _4item$picList         0x004d7080   uninitialized data, symbol size 0
//   _4item$picLoadBuffer   0x004da080   uninitialized data, symbol size 0
// The three arrays have no symbol size in the executable. Extents here come from the consumer and the next symbol:
// picBuffer is 256 slots of 1280 bytes (327680), exactly the gap to picList; picList is 64 records of 192 bytes
// (12288), exactly the gap to picLoadBuffer; picLoadBuffer is 32768 bytes, which is only the gap to the next named
// symbol (0x004e2080) and so an upper bound, not an observed size. The initial -100000 is a negative value that
// picProcess__4itemv treats as "no record in progress"; why that particular number is unknown. Fresh compilation is
// not an original-byte match.
extern "C" {
signed char picsToLoad __asm__("_4item$picsToLoad") = 0;
int picInProgress __asm__("_4item$picInProgress") = -100000;
signed char picLoadFinished __asm__("_4item$picLoadFinished") = 0;
unsigned char picBuffer[256 * 1280] __asm__("_4item$picBuffer");
unsigned char picList[64 * 192] __asm__("_4item$picList");
unsigned char picLoadBuffer[32768] __asm__("_4item$picLoadBuffer");
}
