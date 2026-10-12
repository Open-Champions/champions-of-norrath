// Observed: keyboardGetNextKey__Fv at 0x002ca440 is 272 bytes with a 32-byte
// frame. If lastkey is nonzero it first adds 1 to lasttime. It then loops on
// Read__10VIKeyboardR12VIConsoleKey(vik, &key), where key is a local record
// whose byte 0 is a code and byte 1 is a flag:
//  - Read returns zero (no event): the function returns -1 if lasttime is below
//    31, if lastkey is zero, or if framedone equals drawFrameNm; otherwise it
//    stores drawFrameNm in framedone and returns lastkey (a repeat).
//  - Read returns nonzero: the code is remapped; codes 19 to 25 become -1, 91
//    becomes 138, 93 becomes 139, 96 becomes 39, 123 becomes 91, 124 becomes
//    140, 125 becomes 93, 126 becomes 96; every other code, including all codes
//    outside 19..126, is unchanged. If byte 1 is nonzero it stores 0 in lasttime
//    and the remapped code in lastkey and returns the code (so -1 can be
//    returned and stored). If byte 1 is zero (a release) it clears lastkey when
//    it equals the remapped code, and in every release case reads again.
// Inferred: byte 1 of the record is a pressed/released flag, from the repeat
// logic that follows; lasttime counts calls since the last press, with 31 the
// repeat delay.
// Unknown: the type and size of the keyboard object vik, the full layout of
// VIConsoleKey beyond bytes 0 and 1 (the frame leaves 16 bytes), and what the
// remapped values 138 to 140 stand for.

extern "C" int Read__10VIKeyboardR12VIConsoleKey(void* keyboard, void* key);
extern "C" char vik[];
extern "C" int lastkey;
extern "C" int lasttime;
extern "C" int framedone;
extern "C" int drawFrameNm;

extern "C" int keyboardGetNextKey__Fv() {
    unsigned char key[16];
    if (lastkey != 0) ++lasttime;
    for (;;) {
        if (Read__10VIKeyboardR12VIConsoleKey(vik, key) == 0) {
            if (lasttime < 31) return -1;
            if (lastkey == 0) return -1;
            if (framedone == drawFrameNm) return -1;
            framedone = drawFrameNm;
            return lastkey;
        }
        int code = key[0];
        switch (code) {
        case 19: case 20: case 21: case 22: case 23: case 24: case 25: code = -1; break;
        case 91: code = 138; break;
        case 93: code = 139; break;
        case 96: code = 39; break;
        case 123: code = 91; break;
        case 124: code = 140; break;
        case 125: code = 93; break;
        case 126: code = 96; break;
        default: break;
        }
        if (key[1] != 0) {
            lasttime = 0;
            lastkey = code;
            return code;
        }
        if (code == lastkey) lastkey = 0;
    }
}
