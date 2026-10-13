// Observed in the pinned SLUS_205.65 image (sizes are the ELF symbol sizes):
//   playerMenu               0x004287f0  80 bytes   five 16-byte records; the first word of records
//                                       0 to 3 points at the strings menu_oneplayer, menu_twoplayer,
//                                       menu_threeplayer, menu_fourplayer; every other word is zero
//   playerSelectNextName     0x004389b8  64 bytes   zero
//   playerSelectPrevName     0x004389f8  64 bytes   zero (directly after the previous one)
//   playerRecalled           0x00483f3c  1 byte     zero
//   playerSelectCount        0x00486002  1 byte     zero
//   playerSelectStartPlayer  0x00486003  1 byte     zero
//   playerSelectEnforce      0x00486004  1 byte     zero
//   playerSelectExitMessage  0x00486008  4 bytes    zero
// Inferred: the playerMenu record shape, from the 16-byte stride used by runFrontEnd.
// Unknown: the meaning of the three zero words of a menu record; what values the byte flags take.
// The earlier unit w8-net-play-c-player-select-state defines these names plus others that other units
// own, and is not selected; this unit defines only the names that were still unresolved.
struct norrath_player_menu_record {
    const char* label;
    unsigned second;
    unsigned third;
    unsigned fourth;
};

extern "C" {
norrath_player_menu_record playerMenu[5] = {
    {"menu_oneplayer", 0u, 0u, 0u},
    {"menu_twoplayer", 0u, 0u, 0u},
    {"menu_threeplayer", 0u, 0u, 0u},
    {"menu_fourplayer", 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
};
unsigned char playerSelectNextName[64];
unsigned char playerSelectPrevName[64];
signed char playerRecalled;
signed char playerSelectCount;
signed char playerSelectStartPlayer;
signed char playerSelectEnforce;
int playerSelectExitMessage;
}
