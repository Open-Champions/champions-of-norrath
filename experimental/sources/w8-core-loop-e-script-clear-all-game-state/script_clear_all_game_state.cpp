// Observed scriptClearAllGameState__Fv at 0x001d3f10, 80 bytes.
// gameStateData (0x007d35e8) holds packed zero-terminated strings. If its
// first byte is zero the function returns. Otherwise, for the string at the
// cursor, it takes the length n, stores zero at cursor+n+1 (the byte after the
// terminator) and moves the cursor to cursor+n+2. The loop ends when the byte
// at the new cursor is zero. So the byte after each string's terminator is
// cleared before the cursor skips it. What the data represents is unknown.
extern "C" unsigned char norrath_script_game_state_data_view[];
extern "C" unsigned strlen(const char* text);

extern "C" void scriptClearAllGameState__Fv() {
    char* cursor = reinterpret_cast<char*>(norrath_script_game_state_data_view);
    if (*cursor == 0) return;
    do {
        char* const end = cursor + strlen(cursor);
        end[1] = 0;
        cursor = end + 2;
    } while (*cursor != 0);
}
