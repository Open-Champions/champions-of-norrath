// Observed: loadPlayerInfo__FiPiP12playerStruct at 0x00256398 is 304 bytes with
// a 64-byte frame. Arguments are (count, ids, players). It issues memory-card
// commands through MCA_Cmd__Fiiii(command, argument, size, 0) only:
//  1. If ids is null: MCA_Cmd(2, 0, 0, 0), then MCA_Cmd(3, importInfoHeader,
//     112, 0).
//  2. Adds the result of GetSaveFootPrint__6Camera(gameCam) to the result of
//     jumpGateSaveFootPrint__Fv() (the first result is saved across the second
//     call) and calls MCA_Cmd(2, sum + 4216, 0, 0).
//  3. For each of count entries, with skip starting at 98 and growing by 23896
//     per entry: if ids is null, MCA_Cmd(2, 14, 1, 0) and MCA_Cmd(3, players,
//     23984, 0); otherwise MCA_Cmd(2, skip, 1, 0) and MCA_Cmd(3, ids, 4, 0).
//     Then ids advances by one int and players by 23984 bytes (both advance
//     even when unused).
// Inferred: command 2 is a seek and command 3 a read, from the argument shapes
// (offset, whence-like third value 0 or 1) and (buffer, length); the third value
// of the seek 1 reads as "relative". These readings are not verified against
// MCA_Cmd__Fiiii itself.
// Unknown: why the skip grows by 23896 per entry (copied as the code does it),
// the layout of importInfoHeader (112 bytes), and the layout of playerStruct
// beyond its 23984-byte size.

extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth);
extern "C" int GetSaveFootPrint__6Camera(void* camera);
extern "C" int jumpGateSaveFootPrint__Fv();
extern "C" void* gameCam;
extern "C" char importInfoHeader[];

static inline const void* asArg(int value) {
    return reinterpret_cast<const void*>(static_cast<unsigned long>(value));
}

extern "C" void loadPlayerInfo__FiPiP12playerStruct(int count, int* ids, void* players) {
    if (ids == 0) {
        MCA_Cmd__Fiiii(2, asArg(0), 0, 0);
        MCA_Cmd__Fiiii(3, importInfoHeader, 112, 0);
    }
    int position = GetSaveFootPrint__6Camera(gameCam);
    position += jumpGateSaveFootPrint__Fv();
    MCA_Cmd__Fiiii(2, asArg(position + 4216), 0, 0);
    int skip = 98;
    int* cursor = ids;
    unsigned char* record = static_cast<unsigned char*>(players);
    for (int i = 0; i < count; ++i) {
        if (ids == 0) {
            MCA_Cmd__Fiiii(2, asArg(14), 1, 0);
            MCA_Cmd__Fiiii(3, record, 23984, 0);
        } else {
            MCA_Cmd__Fiiii(2, asArg(skip), 1, 0);
            MCA_Cmd__Fiiii(3, cursor, 4, 0);
        }
        skip += 23900 - 4;
        ++cursor;
        record += 23984;
    }
}
