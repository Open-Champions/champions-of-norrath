// Observed scriptGetGameState__FPc at 0x001d39c0, 236 bytes. The name is
// copied into a 256-byte stack buffer (unbounded) and lower-cased in place
// with the newlib character table: a byte whose table entry has bit 0 set gets
// 32 added. The game-state table (norrath_script_game_state_data_view,
// gameStateData at 0x007d35e8) is a run of records, each a NUL-terminated name
// followed by one signed value byte, ended by a record whose first byte is
// zero. A matching name returns its value byte, sign-extended. Without a match
// the key is appended at the end, its value byte is cleared, and 0 is
// returned; the byte after that new record is not written and relies on the
// table already being zero. The table's 8192-byte size is inferred from the
// next symbol; the function does not check it.
extern "C" {
char* strcpy(char* destination, const char* source);
int strcmp(const char* left, const char* right);
__SIZE_TYPE__ strlen(const char* text);
extern unsigned char _ctype_[];
extern unsigned char norrath_script_game_state_data_view[8192];

int scriptGetGameState__FPc(char* name) {
    char key[256];
    strcpy(key, name);
    for (char* p = key; *p != 0; ++p) {
        const int c = *p;
        if (_ctype_[c + 1] & 1) *p = static_cast<char>(c + 32);
    }
    char* entry = reinterpret_cast<char*>(norrath_script_game_state_data_view);
    while (*entry != 0) {
        if (strcmp(entry, key) == 0) return entry[strlen(entry) + 1];
        entry += strlen(entry) + 2;
    }
    strcpy(entry, key);
    entry[strlen(entry) + 1] = 0;
    return 0;
}
}
