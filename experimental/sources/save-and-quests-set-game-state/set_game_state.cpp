// Observed scriptSetGameState__FPci at 0x001d3ab0, 248 bytes. The value is
// clamped to -128..127. The name is copied (unbounded) into a 256-byte stack
// buffer and lower-cased with the newlib table, as in scriptGetGameState. The
// game-state records (NUL-terminated name, one value byte, ended by a record
// whose first byte is zero) are searched for that key. A match, or the
// appended key when there is none, gets the clamped value stored in the byte
// after its terminator. The return is void.
extern "C" {
char* strcpy(char* destination, const char* source);
int strcmp(const char* left, const char* right);
__SIZE_TYPE__ strlen(const char* text);
extern unsigned char _ctype_[];
extern unsigned char norrath_script_game_state_data_view[8192];

void scriptSetGameState__FPci(char* name, int value) {
    if (value < -128) value = -128;
    if (value > 127) value = 127;
    char key[256];
    strcpy(key, name);
    for (char* p = key; *p != 0; ++p) {
        const int c = *p;
        if (_ctype_[c + 1] & 1) *p = static_cast<char>(c + 32);
    }
    char* entry = reinterpret_cast<char*>(norrath_script_game_state_data_view);
    while (*entry != 0) {
        if (strcmp(entry, key) == 0) {
            entry[strlen(entry) + 1] = static_cast<char>(value);
            return;
        }
        entry += strlen(entry) + 2;
    }
    strcpy(entry, key);
    entry[strlen(entry) + 1] = static_cast<char>(value);
}
}
