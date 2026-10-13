// Observed: SaveAndExit__12VIWndOptions at 0x010c45b0 is 176 bytes. With this it calls VIMemCardStatus__FPi(0).
//  - When that result is nonzero: it stores 1 in the word at +1184, copies the two words at +52 and +56 to a
//    local pair, calls McErr__FP5VIWndRC8VIVect2Ii(this, &pair, 4), then McErrSetMode__Fi(1), and returns -1.
//  - Otherwise it calls Save__13VIGameOptions(VIGetGameOptions__Fv()). If that result is negative it stores 1
//    in the word at +1192 and returns -1. If not, it clears the word at +28, calls
//    SetFocus__5VIWndP5VIWndb(this, the word at +20, 1) and returns 0.
// Inferred: the memory-card check gates the save; +1184 and +1192 are "card problem" and "save failed" latches
// that the key handler consults; the pair at +52 is a 2D size or position handed to the error window.
// Unknown: the meaning of McErr's mode 4 and of McErrSetMode(1), VIMemCardStatus's argument, the field names,
// and the exact types of the Save and VIMemCardStatus results beyond int.
extern "C" {
int VIMemCardStatus__FPi(int* value);
void McErr__FP5VIWndRC8VIVect2Ii(void* window, const int* size, int mode);
void McErrSetMode__Fi(int mode);
void* VIGetGameOptions__Fv();
int Save__13VIGameOptions(void* options);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);

int SaveAndExit__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    if (VIMemCardStatus__FPi(0) != 0) {
        words[296] = 1;
        int size[2];
        size[0] = words[13];
        size[1] = words[14];
        McErr__FP5VIWndRC8VIVect2Ii(self, size, 4);
        McErrSetMode__Fi(1);
        return -1;
    }
    if (Save__13VIGameOptions(VIGetGameOptions__Fv()) < 0) {
        words[298] = 1;
        return -1;
    }
    words[7] = 0;
    SetFocus__5VIWndP5VIWndb(self, reinterpret_cast<void*>(static_cast<unsigned long>(words[5])), true);
    return 0;
}
}
