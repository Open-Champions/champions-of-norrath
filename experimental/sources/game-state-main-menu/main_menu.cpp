// Observed mainMenu at 0x0037f1a0, 128 bytes. The ELF binding is local.
// Eight 16-byte records. Each record's first word is a pointer and the other
// three words are zero. The pointers address these exact bytes, including
// the NUL: menu_returntogame, menu_recall, menu_loadgame, menu_option,
// menu_changeplayers, menu_kickplayers, menu_exittomenu. The eighth record
// is zero. This definition is global because the pause-menu source imports
// the name. Nothing here was executed.
extern "C" {
struct {
    const char* label;
    unsigned word1;
    unsigned word2;
    unsigned word3;
} mainMenu[8] = {
    {"menu_returntogame", 0u, 0u, 0u},
    {"menu_recall", 0u, 0u, 0u},
    {"menu_loadgame", 0u, 0u, 0u},
    {"menu_option", 0u, 0u, 0u},
    {"menu_changeplayers", 0u, 0u, 0u},
    {"menu_kickplayers", 0u, 0u, 0u},
    {"menu_exittomenu", 0u, 0u, 0u},
    {0, 0u, 0u, 0u},
};
}

#if defined(__mips__)
static_assert(sizeof(mainMenu) == 128, "eight 16-byte menu records");
#endif
