// Observed superMainMenu at 0x0037f220 and raceMainMenu at 0x0037f270.
// Each is 80 bytes. The nonzero words are addresses of text that has no ELF
// symbol: "menu_returntogame", "menu_levelwarp", "menu_invulnerability",
// "Clean Up", "menu_restartrace", "menu_quitrace", and "menu_moremenu".
// The word 0x004841cc is the address of cheatInvulnerability. These arrays
// keep the observed words. They are not relocated pointers.
extern "C" unsigned superMainMenu[20] = {
    0x004638e0u, 0u, 0u, 0u, 0x00463968u, 0u, 0u, 0u, 0x00463978u, 3u,
    0x004841ccu, 0u, 0x00463990u, 0u, 0u, 0u, 0u,          0u, 0u, 0u};
extern "C" unsigned raceMainMenu[20] = {
    0x004638e0u, 0u, 0u, 0u, 0x004639a0u, 0u, 0u, 0u, 0x004639b8u, 0u,
    0u,          0u, 0x004639c8u, 0u, 0u, 0u, 0u,          0u, 0u, 0u};
