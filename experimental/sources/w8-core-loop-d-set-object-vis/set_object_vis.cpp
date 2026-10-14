// Observed gameSetObjectVis__Fv at 0x001cb4e8, 268 bytes. It fills the list of
// 12-byte points objectVisPoints (0x00333d28, 48 bytes in the image) and the
// signed byte objectVisCount (0x00483afe), and sets the byte
// objectVisUseCamCenter.
//  * If DLG_GetDialogCreature__Fv returns nonzero (it is called a second time
//    and that result used): objectVisUseCamCenter = 0, objectVisCount = 1 and
//    point 0 is the 12 bytes at +16 of that object.
//  * Otherwise objectVisUseCamCenter becomes (word at gameCam+52) != 1 when
//    netNetworkMode is zero; when it is nonzero, that same value if the signed
//    byte cutMode is nonzero and 0 if cutMode is zero. objectVisCount is then
//    set to 0 (also when no player is counted). For each of the first
//    nmPlayers records of player (stride 23984) whose word at +21872 is a
//    nonzero pointer, the 12 bytes at +16 of that pointee are appended.
// Unknown: what the three outputs feed, the type of gameCam beyond a word at
// +52, and what the pointer at record +21872 is. No bound is applied to the
// count beyond nmPlayers, as in the image.
extern "C" signed char netNetworkMode;
extern "C" signed char cutMode;
extern "C" int nmPlayers;
extern "C" unsigned char player[];
extern "C" unsigned char* gameCam;
extern "C" unsigned char objectVisUseCamCenter;
extern "C" signed char objectVisCount;
struct ObjectVisPoint { float x, y, z; };
extern "C" ObjectVisPoint objectVisPoints[];
extern "C" unsigned char* DLG_GetDialogCreature__Fv();

extern "C" void gameSetObjectVis__Fv() {
    if (DLG_GetDialogCreature__Fv() != 0) {
        const unsigned char* creature = DLG_GetDialogCreature__Fv();
        objectVisUseCamCenter = 0;
        objectVisCount = 1;
        objectVisPoints[0] = *reinterpret_cast<const ObjectVisPoint*>(creature + 16);
        return;
    }
    unsigned char notOne = *reinterpret_cast<const int*>(gameCam + 52) != 1;
    if (netNetworkMode == 0) objectVisUseCamCenter = notOne;
    else if (cutMode != 0) objectVisUseCamCenter = notOne;
    else objectVisUseCamCenter = 0;
    int remaining = nmPlayers;
    objectVisCount = 0;
    if (remaining <= 0) return;
    const unsigned char* slot = player + 21872;
    while (remaining-- != 0) {
        const unsigned char* object = *reinterpret_cast<const unsigned char* const*>(slot);
        slot += 23984;
        if (object != 0) {
            signed char index = objectVisCount;
            objectVisCount = static_cast<signed char>(index + 1);
            objectVisPoints[index] = *reinterpret_cast<const ObjectVisPoint*>(object + 16);
        }
    }
}
