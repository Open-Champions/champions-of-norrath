// Observed menu record arrays in the pinned SLUS-20565 image. A record is
// four words; the first is a label pointer and the list ends at the first
// record with a null label (the same shape frontMenu uses and runFrontEnd
// walks with a 16-byte stride).
//   gameTypeMenu   0x00428730, 48 bytes: "menu_normalgame", "menu_netgame",
//                  then one zero record. All other words are zero.
//   difficultyMenu 0x00428840, 64 bytes: "menu_easy2", "menu_normal2",
//                  "menu_hard2", then one zero record. Other words are zero.
//   kickMenu       0x0037f2c0, 96 bytes: "menu_choosetokick" with the value 5
//                  in the second word, then five zero records. Pause-menu
//                  code stores label pointers into records 1 to 4 at run
//                  time and clears them first, so the shipped zeros are
//                  only the initial state.
// Inferred: the 16-byte record stride, from the sizes and the walkers.
// Unknown: what the value 5 in kickMenu's first record means, and the roles
// of the second to fourth words in every other record.
struct norrath_menu_record {
    const char* label;
    unsigned second;
    unsigned third;
    unsigned fourth;
};

extern "C" {
norrath_menu_record gameTypeMenu[3] = {
    {"menu_normalgame", 0u, 0u, 0u},
    {"menu_netgame", 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
};
norrath_menu_record difficultyMenu[4] = {
    {"menu_easy2", 0u, 0u, 0u},
    {"menu_normal2", 0u, 0u, 0u},
    {"menu_hard2", 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
};
norrath_menu_record kickMenu[6] = {
    {"menu_choosetokick", 5u, 0u, 0u},
    {0, 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
};
}
