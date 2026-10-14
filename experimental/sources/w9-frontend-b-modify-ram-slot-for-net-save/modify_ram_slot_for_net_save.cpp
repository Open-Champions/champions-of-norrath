// Observed: modifyRamSlotForNetSave__Fv at 0x00259e10 is 756 bytes with a
// 64-byte frame. Let base be ramSlot + 112 + GetSaveFootPrint__6Camera(gameCam)
// + jumpGateSaveFootPrint__Fv() (the two calls are made in that order, and
// again each time base is needed). Player entries begin at base + 4104; entry n
// is 23998 bytes apart: 14 header bytes (two bytes, then twelve bytes) followed
// by a 23984-byte player record.
//  1. For n = 0 to 3: copies the two header bytes of entry n to
//     originalYaw[2 * n .. 2 * n + 1] and the following twelve bytes to
//     originalPos + 12 * n.
//  2. Calls netGetLocalPlayerNm__Fi(0), copies originalYaw[2 * r] and the byte
//     after it into the two header bytes of entry 0; calls
//     netGetLocalPlayerNm__Fi(0) again and copies the twelve bytes at
//     originalPos + 12 * r2 to entry 0 + 2.
//  3. Recomputes base and copies the 23984-byte record of entry 0 to
//     tempPlayerStruct, byte by byte.
//  4. Recomputes base, calls netGetLocalPlayerNm__Fi(0) (result r3), recomputes
//     base again and copies the 23984-byte record of entry r3 over the record of
//     entry 0 (entry r3 is taken as a null pointer when r3 is outside 0..3).
//  5. Stores 1 in the halfword at ramSlot + 106, recomputes base and copies the
//     four bytes of the int 1 to base + 4100.
//  6. If the byte gameOver is nonzero, copies the 13 bytes "gameComplete" and
//     its terminator to the start of ramSlot.
// Inferred: the routine rearranges a save slot so the local network player's
// data sits in slot 0 (the swap source being entry r3), and keeps the old slot-0
// record in tempPlayerStruct for a matching un-modify; the pairing with
// unModifyRamSlotForNetSave__Fv comes from the names only.
// Unknown: the layout of ramSlot beyond the offsets used here, the meaning of
// the two header values, the size of originalYaw (8 bytes in the symbol table)
// and what "gameComplete" selects when copied to the front of the slot.

extern "C" int GetSaveFootPrint__6Camera(void* camera);
extern "C" int jumpGateSaveFootPrint__Fv();
extern "C" int netGetLocalPlayerNm__Fi(int player);
extern "C" void* gameCam;
extern "C" unsigned char ramSlot[];
extern "C" unsigned char originalYaw[];
extern "C" unsigned char originalPos[];
extern "C" unsigned char tempPlayerStruct[];
extern "C" unsigned char gameOver;

static inline unsigned char* slotBase() {
    int a = GetSaveFootPrint__6Camera(gameCam);
    int b = jumpGateSaveFootPrint__Fv();
    return ramSlot + 112 + a + b;
}

static inline unsigned char* entryRecord(int n) {
    unsigned char* p = slotBase() + 4104 + 14;
    if (n < 0 || n > 3) return 0;
    return p + 23998 * n;
}

extern "C" void modifyRamSlotForNetSave__Fv() {
    unsigned char* header = slotBase() + 4104;
    unsigned char* first = header;
    for (int n = 0; n < 4; ++n) {
        originalYaw[2 * n] = header[0];
        originalYaw[2 * n + 1] = header[1];
        for (int k = 0; k < 12; ++k) originalPos[12 * n + k] = header[2 + k];
        header += 23998;
    }
    int r = netGetLocalPlayerNm__Fi(0);
    first[0] = originalYaw[2 * r];
    first[1] = originalYaw[2 * r + 1];
    int r2 = netGetLocalPlayerNm__Fi(0);
    for (int k = 0; k < 12; ++k) first[2 + k] = originalPos[12 * r2 + k];
    unsigned char* source = entryRecord(0);
    for (int k = 0; k < 23984; ++k) tempPlayerStruct[k] = source[k];
    unsigned char* target = entryRecord(0);
    int r3 = netGetLocalPlayerNm__Fi(0);
    source = entryRecord(r3);
    for (int k = 0; k < 23984; ++k) target[k] = source[k];
    int one = 1;
    *reinterpret_cast<unsigned short*>(ramSlot + 106) = static_cast<unsigned short>(one);
    unsigned char* word = slotBase() + 4100;
    for (int k = 0; k < 4; ++k) word[k] = reinterpret_cast<unsigned char*>(&one)[k];
    if (gameOver != 0) {
        const char* name = "gameComplete";
        for (int k = 0; k < 13; ++k) ramSlot[k] = static_cast<unsigned char>(name[k]);
    }
}
