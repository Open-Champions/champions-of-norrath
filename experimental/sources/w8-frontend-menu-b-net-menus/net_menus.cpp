// Observed: three menu tables in the pinned SLUS-20565 image, each 48 bytes:
// menuNetClientServer at 0x00428760, menuNetImportOrNewPlayer at 0x00428790
// and menuNetImportOrNewGame at 0x004287c0. Each holds two 16-byte records
// followed by one all-zero record. Only the first word of a live record is
// nonzero; it points at a text key. Words +4, +8 and +12 of every record are
// zero, which menuRun reads as kind 0, no value and not hidden.
// The keys are menu_net_client and menu_net_server; menu_net_newplayer and
// menu_net_importplayer; menu_net_newgame and menu_net_importgame.
// Inferred: the record shape from menuRun's 16-byte stride. Unknown: what
// the four words are called in the original source.
struct norrath_menu_record {
    const char* label;
    unsigned kind;
    void* value;
    unsigned flags;
};
static_assert(__SIZEOF_POINTER__ != 4 || sizeof(norrath_menu_record) == 16, "records are 16 bytes");

extern "C" {
norrath_menu_record menuNetClientServer[3] = {
    {"menu_net_client", 0u, 0, 0u},
    {"menu_net_server", 0u, 0, 0u},
    {0, 0u, 0, 0u},
};
norrath_menu_record menuNetImportOrNewPlayer[3] = {
    {"menu_net_newplayer", 0u, 0, 0u},
    {"menu_net_importplayer", 0u, 0, 0u},
    {0, 0u, 0, 0u},
};
norrath_menu_record menuNetImportOrNewGame[3] = {
    {"menu_net_newgame", 0u, 0, 0u},
    {"menu_net_importgame", 0u, 0, 0u},
    {0, 0u, 0, 0u},
};
}
