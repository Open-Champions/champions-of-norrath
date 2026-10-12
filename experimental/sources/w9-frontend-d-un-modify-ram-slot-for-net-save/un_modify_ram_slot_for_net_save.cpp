// Observed: ramSlot starts with a 112-byte header, followed by a region whose
// start is ramSlot + 0x70 + GetSaveFootPrint(gameCam) + jumpGateSaveFootPrint().
// Both footprint calls are repeated three times, before each of the three
// stages below, and their results are added anew each time.
//   1. 23984 bytes of tempPlayerStruct are copied to region + 4118.
//   2. The low halfword of nmPlayers is stored at ramSlot + 106. The four
//      bytes of nmPlayers, low byte first, are then stored at region + 4100.
//   3. Starting at region + 4104, four entries of 23998 bytes each receive
//      two bytes of originalYaw (index 2 * n and 2 * n + 1) and, directly after
//      them, the 12 bytes of originalPos at 12 * n. The 23984 bytes after
//      those 14 bytes are not touched by this stage.
// The entry of stage 1 is the first of these four entries.
// Inferred: stage 1 fills the first entry's 23984-byte part.
// Unknown: the meaning of the header halfword, the types behind originalYaw
// and originalPos, and the layout of the region before offset 4100.

extern "C" int GetSaveFootPrint__6Camera(void* camera);
extern "C" int jumpGateSaveFootPrint__Fv();
extern "C" void* gameCam;
extern "C" int nmPlayers;
extern "C" unsigned char ramSlot[];
extern "C" unsigned char tempPlayerStruct[23984];
extern "C" unsigned char originalYaw[8];
extern "C" unsigned char originalPos[48];

extern "C" void unModifyRamSlotForNetSave__Fv() {
    int camera = GetSaveFootPrint__6Camera(gameCam);
    unsigned char* region = ramSlot + 0x70 + camera;
    region += jumpGateSaveFootPrint__Fv();
    for (int i = 0; i < 23984; ++i) {
        region[4118 + i] = tempPlayerStruct[i];
    }

    int players = nmPlayers;
    *(unsigned short*)(ramSlot + 106) = (unsigned short)players;
    camera = GetSaveFootPrint__6Camera(gameCam);
    region = ramSlot + 0x70 + camera;
    region += jumpGateSaveFootPrint__Fv();
    for (int i = 0; i < 4; ++i) {
        region[4100 + i] = ((unsigned char*)&players)[i];
    }

    camera = GetSaveFootPrint__6Camera(gameCam);
    region = ramSlot + 0x70 + camera;
    region += jumpGateSaveFootPrint__Fv();
    unsigned char* entry = region + 4104;
    for (int n = 0; n < 4; ++n) {
        entry[0] = originalYaw[2 * n];
        entry[1] = originalYaw[2 * n + 1];
        for (int k = 0; k < 12; ++k) {
            entry[2 + k] = originalPos[12 * n + k];
        }
        entry += 23998;
    }
}
