// Observed: loadRamSlot__Fv at 0x002598f8 is 788 bytes with an 80-byte frame. With
// a cursor p that starts at ramSlot and a function-local 112-byte buffer
// newInfoHeader (symbol newInfoHeader.1280 at 0x007e0540), it:
//  1. copies 112 bytes from p to newInfoHeader and advances p by 112;
//  2. p += Load__6CameraPc(gameCam, p); p += jumpGateLoad__FPc(p);
//  3. copies 4096 bytes from p to levelSeeds and advances p by 4096;
//  4. reads an unaligned int from p into gameAutoMapMode and another into
//     nmPlayers, advancing p by 4 after each;
//  5. for n from 0 to 3 (nmPlayers is re-read each pass): if n < nmPlayers, copies
//     two signed bytes from p to playerStartYaw[2n] and playerStartYaw[2n + 1]
//     (p += 2), twelve bytes to playerStartPos + 12n (p += 12) and 23984 bytes to
//     the n-th 23984-byte element of player (p += 23984), then calls
//     convertIndiciesToPointers__Fi(n) and stores zero in the word at
//     player + 23984n + 21872; otherwise p advances by 23998 and nothing is copied;
//  6. advances p until (p - ramSlot) is a multiple of four;
//  7. scriptClearAllGameState__Fv(); then while the byte at p is nonzero calls
//     scriptSetGameState__FPci(p, the int at p + 32) and advances p by 36;
//  8. with h = newInfoHeader: keepPlayerPositions = 1, gameDifficulty = the signed
//     byte at h + 108, curPlaySeconds = the int at h + 100; if strlen(h + 20) is
//     nonzero, sprintf(levelName, "%s_%s", h, h + 20), else strcpy(levelName, h);
//  9. levelName[80] = -3; copies strlen(h) + 1 bytes of h into loadName one byte
//     at a time (strlen is called again for each byte); if strlen(h + 20) is
//     nonzero, strcat(loadName, "_") and strcat(loadName, h + 20);
// 10. when gameNetReplicateQuestItems is 1, calls inventorySyncQuestItems__Fi(0) and
//     clears it; finally saveCheckPointCount = 99.
// Inferred: ramSlot holds a saved game slot (header, camera, jump gate, level
// seeds, per-player data, script variables), and h + 0 and h + 20 are a level name
// and a sub-name; the order of the stream comes from the code only.
// Unknown: the meaning of the 112 header bytes other than offsets 0, 20, 100 and
// 108, of the zeroed word at player element + 21872, of levelName + 80, and the
// size of the playerStartYaw object (read as eight bytes).

extern "C" int sprintf(char* out, const char* format, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" int Load__6CameraPc(void* camera, unsigned char* buffer);
extern "C" int jumpGateLoad__FPc(unsigned char* buffer);
extern "C" void convertIndiciesToPointers__Fi(int n);
extern "C" void scriptClearAllGameState__Fv();
extern "C" void scriptSetGameState__FPci(unsigned char* name, int value);
extern "C" void inventorySyncQuestItems__Fi(int n);
extern "C" unsigned char ramSlot[];
extern "C" void* gameCam;
extern "C" unsigned char levelSeeds[];
extern "C" int gameAutoMapMode;
extern "C" int nmPlayers;
extern "C" signed char playerStartYaw[];
extern "C" unsigned char playerStartPos[];
extern "C" unsigned char player[];
extern "C" signed char keepPlayerPositions;
extern "C" int gameDifficulty;
extern "C" int curPlaySeconds;
extern "C" char levelName[];
extern "C" char loadName[];
extern "C" int gameNetReplicateQuestItems;
extern "C" int saveCheckPointCount;

static void copyBytes(unsigned char* to, const unsigned char* from, int n) {
    for (int i = 0; i < n; ++i) to[i] = from[i];
}

extern "C" void loadRamSlot__Fv() {
    static unsigned char newInfoHeader[112];
    unsigned char* p = ramSlot;
    copyBytes(newInfoHeader, p, 112);
    p += 112;
    p += Load__6CameraPc(gameCam, p);
    p += jumpGateLoad__FPc(p);
    copyBytes(levelSeeds, p, 4096);
    p += 4096;
    copyBytes(reinterpret_cast<unsigned char*>(&gameAutoMapMode), p, 4);
    p += 4;
    copyBytes(reinterpret_cast<unsigned char*>(&nmPlayers), p, 4);
    p += 4;
    for (int n = 0; n < 4; ++n) {
        if (n < nmPlayers) {
            playerStartYaw[2 * n] = static_cast<signed char>(p[0]);
            playerStartYaw[2 * n + 1] = static_cast<signed char>(p[1]);
            p += 2;
            copyBytes(playerStartPos + 12 * n, p, 12);
            p += 12;
            copyBytes(player + 23984 * n, p, 23984);
            p += 23984;
            convertIndiciesToPointers__Fi(n);
            *reinterpret_cast<int*>(player + 23984 * n + 21872) = 0;
        } else {
            p += 23998;
        }
    }
    while (((p - ramSlot) & 3) != 0) ++p;
    scriptClearAllGameState__Fv();
    while (*p != 0) {
        scriptSetGameState__FPci(p, *reinterpret_cast<int*>(p + 32));
        p += 36;
    }
    char* h = reinterpret_cast<char*>(newInfoHeader);
    keepPlayerPositions = 1;
    gameDifficulty = static_cast<signed char>(newInfoHeader[108]);
    curPlaySeconds = *reinterpret_cast<int*>(newInfoHeader + 100);
    if (strlen(h + 20) != 0) sprintf(levelName, "%s_%s", h, h + 20);
    else strcpy(levelName, h);
    levelName[80] = -3;
    for (unsigned long i = 0; i < strlen(h) + 1; ++i) loadName[i] = h[i];
    if (strlen(h + 20) != 0) {
        strcat(loadName, "_");
        strcat(loadName, h + 20);
    }
    if (gameNetReplicateQuestItems == 1) {
        inventorySyncQuestItems__Fi(0);
        gameNetReplicateQuestItems = 0;
    }
    saveCheckPointCount = 99;
}
