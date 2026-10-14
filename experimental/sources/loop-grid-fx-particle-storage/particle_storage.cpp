// Storage that P_Clear, P_AddParticle and P_Update address by absolute value.
//   pushLevel     0x484304  4 bytes (.sdata), zero.
//   currentPS     0x48430c  4 bytes (.sdata), initial value the address of
//                           globalSystem (the file word is 0x0037f730).
//   freeBlocks    0x4869f0  4 bytes (.sbss), zero. Head of the free block list.
//   globalSystem  0x37f730  256 bytes (.data), all file bytes zero: 16 entries
//                           of 16 bytes, the system used until another is
//                           pushed.
//   blockList     0x780080  512 bytes (BSS) up to descListMark: 128 words, one
//                           per 4096-byte page of block. P_Clear links them.
// The reviewed -G0 profile places the zero objects in BSS.
extern "C" {
unsigned char pushLevel[4];
unsigned char freeBlocks[4];
unsigned char globalSystem[256];
unsigned char blockList[512];
unsigned char* currentPS = globalSystem;
}
static_assert(sizeof globalSystem == 256u && sizeof blockList == 512u, "extents");
