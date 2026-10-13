// Observed: OnKeyDown__12VIWndOptionsi at 0x010c4310 is 668 bytes. With (this, key) it first calls
// VIGetGameOptions__Fv() (g). Then, with mUI the word _5VIWnd$mUI (0x01192c70):
//  - nothing more when the 64-bit value at +1184 is nonzero;
//  - when the word at +1192 is nonzero: for key 13 or 134 the words at +1192 and +1188 are cleared; any other
//    key does nothing;
//  - otherwise: keys 17 and 136 call Decrement__12VIWndOptions(this); 18 and 137 call Increment__12VIWndOptions;
//    19 and 138 call Prev__12VIWndOptions; 20 and 139 call Next__12VIWndOptions; every other key except the
//    two cases below does nothing;
//  - keys 13 and 134: r = Select__12VIWndOptions(this); when r is 1: Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPT
//    RC8VIVect2Ii(this + 1232, GetString(mUI, 1633, the word at mUI + 48), GetString(mUI, 1634, the same
//    column), 2, &{the words at +52 and +56}, 300) is called, the word at +1260 is set to r, and
//    SetFocus__5VIWndP5VIWndb(this, this + 1232, 1);
//  - keys 27 and 132: when Back__12VIWndOptions(this) returns 1: g[6] = low byte of the word at +1156, g[7],
//    g[4], g[5] = the bytes at +1160, +1161, +1162, the floats at g+8 and g+12 = the floats at +1164 and +1168,
//    g[145] = the byte at +1172, g[146] = (the byte at +1173 == 0); then
//    SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(the word at (the word mUI) + 76, 0, float g+8) and
//    (..., 1, float g+12); the word at +28 is cleared; SetFocus(this, the word at +20, 1).
// Inferred: this window edits a copy of the game options and 27/132 confirm and close it; Select with a result
// of 1 opens the message window at +1232 with a two-text prompt.
// Unknown: key names, the meaning of the prompt type 2 and 300, of the 64-bit word at +1184, the type of g
// and of the first argument of SetSoundClassVolume (a pointer is assumed); the return type (void).
extern "C" {
unsigned char* VIGetGameOptions__Fv();
void Decrement__12VIWndOptions(void* self);
void Increment__12VIWndOptions(void* self);
void Prev__12VIWndOptions(void* self);
void Next__12VIWndOptions(void* self);
int Select__12VIWndOptions(void* self);
int Back__12VIWndOptions(void* self);
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* window, const unsigned short* first,
                                                              const unsigned short* second, int prompt,
                                                              const int* size, int a);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(void* device, int sound_class, float volume);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);
extern unsigned _5VIWnd$mUI;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}

static inline void* sound_device() {
    return pointer_of(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(pointer_of(_5VIWnd$mUI)) + 76));
}

void OnKeyDown__12VIWndOptionsi(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    unsigned char* g = VIGetGameOptions__Fv();
    if (*reinterpret_cast<unsigned long long*>(bytes + 1184) != 0) return;
    if (words[298] != 0) {
        if (key == 13 || key == 134) {
            words[298] = 0;
            words[297] = 0;
        }
        return;
    }
    switch (key) {
    case 17: case 136:
        Decrement__12VIWndOptions(self);
        break;
    case 18: case 137:
        Increment__12VIWndOptions(self);
        break;
    case 19: case 138:
        Prev__12VIWndOptions(self);
        break;
    case 20: case 139:
        Next__12VIWndOptions(self);
        break;
    case 13: case 134: {
        const int r = Select__12VIWndOptions(self);
        if (r != 1) break;
        const void* table = pointer_of(_5VIWnd$mUI);
        void* window = bytes + 1232;
        const unsigned short* first = GetString__13VIStringTableii(table, 1633, static_cast<const int*>(table)[12]);
        const unsigned short* second = GetString__13VIStringTableii(table, 1634, static_cast<const int*>(table)[12]);
        const int size[2] = {words[13], words[14]};
        Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(window, first, second, 2, size, 300);
        *reinterpret_cast<int*>(bytes + 1260) = r;
        SetFocus__5VIWndP5VIWndb(self, window, true);
        break;
    }
    case 27: case 132: {
        if (Back__12VIWndOptions(self) != 1) break;
        g[6] = static_cast<unsigned char>(words[289]);
        g[7] = bytes[1160];
        g[4] = bytes[1161];
        g[5] = bytes[1162];
        *reinterpret_cast<float*>(g + 8) = *reinterpret_cast<const float*>(bytes + 1164);
        *reinterpret_cast<float*>(g + 12) = *reinterpret_cast<const float*>(bytes + 1168);
        g[145] = bytes[1172];
        g[146] = bytes[1173] == 0;
        SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(sound_device(), 0, *reinterpret_cast<const float*>(g + 8));
        SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(sound_device(), 1, *reinterpret_cast<const float*>(g + 12));
        words[7] = 0;
        SetFocus__5VIWndP5VIWndb(self, pointer_of(static_cast<unsigned>(words[5])), true);
        break;
    }
    default:
        break;
    }
}
}
