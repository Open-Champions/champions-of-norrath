// Observed gameLineAdd__FG6Point3sPPc at 0x279940, extent 164.
// gameLineCount is the 4-byte file zero at 0x485ea8. The next symbol,
// needEnforce, begins at 0x485ec0. gameLines begins at 0x8d57a8 and the next
// symbol, p1, begins at 0x8d5938: 400 bytes, 100 pointer slots. The add has
// no capacity test. The count store is in the blockAlloc delay slot, so the
// incremented word is visible to that call. A null allocation is still stored
// and does not call __4LineG6Point3sPPc. The 12-byte point is copied first;
// the halfword type is sign-extended before the constructor.

extern "C" unsigned char* norrath_block_alloc(int requested_size);
extern "C" void norrath_construct_Line(unsigned char* line, const void* point, short type, char** arguments);

// Zero initial values. Under the reviewed -G0 profile these land in BSS.
// The original count is a file-backed .sdata zero; the table is BSS.
extern "C" {
unsigned norrath_game_line_count;
unsigned char* norrath_game_lines[100];
}

extern "C" void norrath_game_line_add(const void* point, short type, char** arguments) {
    unsigned char copy[12];
    const unsigned char* source = static_cast<const unsigned char*>(point);
    for (unsigned i = 0; i < 12u; ++i) copy[i] = source[i];
    const unsigned index = norrath_game_line_count;
    const unsigned low = static_cast<unsigned>(static_cast<unsigned short>(type));
    const short signed_type = static_cast<short>(low < 0x8000u ? static_cast<int>(low) : static_cast<int>(low) - 65536);
    norrath_game_line_count = index + 1u;
    unsigned char* line = norrath_block_alloc(24);
    if (line != nullptr) norrath_construct_Line(line, copy, signed_type, arguments);
    norrath_game_lines[index] = line;
}
