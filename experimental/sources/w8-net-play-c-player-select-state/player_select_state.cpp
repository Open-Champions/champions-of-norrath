// Observed storage in the pinned SLUS_205.65 image. Sizes are the ELF symbol
// sizes. Only playerMenu and playerSelectSuccessful start non-zero; every other
// object here is zero in the file. Access widths come from the load/store
// instructions that touch them. Nothing about the meaning of the values is
// established except where stated.
//   playerMenu                0x004287f0  80 bytes   5 records of 16 bytes
//   playerSelectNextName      0x004389b8  64 bytes   all zero
//   playerSelectPrevName      0x004389f8  64 bytes   all zero
//   worldCharViewMatrix       0x0037efb0  64 bytes   all zero, 16-byte aligned
//   playerRecalled            0x00483f3c  1 byte     accessed as signed byte
//   recallEnter               0x00483f3e  1 byte     signed and unsigned byte loads
//   playerSelectSuccessful    0x00485ec8  1 byte     initial value 1
//   playerSelectCount         0x00486002  1 byte
//   playerSelectStartPlayer   0x00486003  1 byte
//   playerSelectEnforce       0x00486004  1 byte
//   playerSelectExitMessage   0x00486008  4 bytes    word
//   syncState                 0x00486c3c  4 bytes    word (symbol has no size; every
//                                                    access is a word load or store)
// playerMenu: records 0..3 hold a pointer to the C string "menu_oneplayer",
// "menu_twoplayer", "menu_threeplayer", "menu_fourplayer" in the first word and
// zero in the other three words; record 4 is all zero. The pointer is 32 bits
// on the target.
struct PlayerMenuRecord {
    const char* name;
    unsigned pad[3];
};

extern "C" {
PlayerMenuRecord playerMenu[5] = {
    {"menu_oneplayer", {0, 0, 0}},
    {"menu_twoplayer", {0, 0, 0}},
    {"menu_threeplayer", {0, 0, 0}},
    {"menu_fourplayer", {0, 0, 0}},
    {0, {0, 0, 0}},
};
unsigned char playerSelectNextName[64];
unsigned char playerSelectPrevName[64];
alignas(16) unsigned char worldCharViewMatrix[64];
signed char playerRecalled;
signed char recallEnter;
signed char playerSelectSuccessful = 1;
signed char playerSelectCount;
signed char playerSelectStartPlayer;
signed char playerSelectEnforce;
int playerSelectExitMessage;
int syncState;
}
