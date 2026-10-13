// Observed: Process__9VIStation at 0x01017f90 is 1008 bytes. One call runs one frame and returns
// an int (0 unless the last step below ran). In order:
//  1. When the word at this + 0x17a30 is nonzero and the word at this + 0x15424 (the object at
//     this + 0x15408, byte 28) is nonzero: that word is cleared, the word at this + 0xb81c is
//     set to the address this + 0xb820, SetFocus__5VIWndP5VIWndb(this + 0xb798, this + 0xb820,
//     1) runs, then RemoveChild__5VIWndP5VIWnd(this + 0xb820, this + 0x15408). The word at
//     this + 0x17a30 is then always cleared.
//  2. Process__13VISoundDevice(this + 0x56a8).
//  3. Init__8VICameraR8VIRasterRC7VIVect3T2ffff(this + 0x54a0, this + 0x610, B, A, 55.0f, the
//     float with bits 0x3fb6db6d, the float with bits 0x3dcccccc, 1000.0f) where B is (0, 0, 0)
//     and A has three negative-zero components (the same call as in VISetup::Process).
//  4. BeginScene__8VIRaster(this + 0x610); SetTime__7VIScenel(this + 0x6830, VITimer::GetTime());
//     the camera is called as a function with ((0, 0, 1), (0, 0, -1)).
//  5. The 84 bytes at this + 0xb6e8 are copied to this + 0xb73c, then
//     Read__5VIPadR12VIConsolePad(this + 0xb470, this + 0xb6e8), Render__7VISceneR8VICamera(
//     this + 0x6830, this + 0x54a0), Update__15VIWindowManager(this + 0xb798) (result unused),
//     Update__7VIFader(this + 0xb448) and EndScene__8VIRaster(this + 0x610).
//  6. When the word at this + 0xb468 is zero and the float at this + 0xb450 equals 1.0f, the word
//     at this + 0xb81c is cleared and SetFocus(this + 0xb798, 0, 1) runs; the frame then counts as
//     finished (the original keeps this in a register that starts at 1 and is cleared here). When the word at this + 0xb468 is still zero afterwards, a function-local static
//     counter (mActiveTries in the image, at 0x0118e608) is incremented, and on reaching 16 or more
//     (the stored value is not reset) the same clear and SetFocus run and the frame counts as
//     finished. A nonzero word at this + 0xb468 skips this step.
//  7. A frame that did not finish returns 0. A finished frame runs strcpy(this + 0x404, this + 0xbc64) and
//     strcpy(this + 0x504, this + 0xbca4) and returns the word at this + 0xbd98.
// Inferred: the 84-byte copy is a console pad snapshot of the previous frame (as in
// VISetup::Process); the counter is a bounded wait of 16 frames before the focus is dropped.
// Unknown: the member types and what the words at 0xb468, 0x17a30 and 0xbd98 hold.

struct Vect3 {
    float x;
    float y;
    float z;
};

struct ConsolePad {
    unsigned char bytes[84];
};

extern "C" {
void Init__8VICameraR8VIRasterRC7VIVect3T2ffff(void* camera, void* raster, Vect3* a, Vect3* b, float f0, float f1,
                                               float f2, float f3);
void BeginScene__8VIRaster(void* raster);
long long GetTime__7VITimer();
void SetTime__7VIScenel(void* scene, long long time);
void __cl__8VICameraR7VIVect3T1(void* camera, Vect3* a, Vect3* b);
void Read__5VIPadR12VIConsolePad(void* pad, ConsolePad* state);
void Process__13VISoundDevice(void* sound);
void Render__7VISceneR8VICamera(void* scene, void* camera);
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag);
void RemoveChild__5VIWndP5VIWnd(void* parent, void* child);
int Update__15VIWindowManager(void* manager);
void Update__7VIFader(void* fader);
void EndScene__8VIRaster(void* raster);
char* strcpy(char* to, const char* from);
}

namespace {
float float_from_bits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
int mActiveTries;
}

extern "C" int Process__9VIStation(void* self) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    void* const camera = base + 0x54a0;
    void* const raster = base + 0x610;
    void* const scene = base + 0x6830;
    void* const manager = base + 0xb798;
    void* const fader = base + 0xb448;
    void* const window = base + 0xb820;
    void* const child = base + 0x15408;
    int* const focus = reinterpret_cast<int*>(base + 0xb81c);
    int* const idle = reinterpret_cast<int*>(base + 0xb468);
    bool finished = false;
    if (*reinterpret_cast<int*>(base + 0x17a30) != 0) {
        int* const pending = reinterpret_cast<int*>(base + 0x15408 + 28);
        if (*pending != 0) {
            *pending = 0;
            *reinterpret_cast<void**>(focus) = window;
            SetFocus__5VIWndP5VIWndb(manager, window, true);
            RemoveChild__5VIWndP5VIWnd(window, child);
        }
    }
    *reinterpret_cast<int*>(base + 0x17a30) = 0;
    Process__13VISoundDevice(base + 0x56a8);
    Vect3 first = {-0.0f, -0.0f, -0.0f};
    Vect3 second = {0.0f, 0.0f, 0.0f};
    Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera, raster, &second, &first, 55.0f, float_from_bits(0x3fb6db6du),
                                              float_from_bits(0x3dccccccu), 1000.0f);
    BeginScene__8VIRaster(raster);
    SetTime__7VIScenel(scene, GetTime__7VITimer());
    first = {0.0f, 0.0f, 1.0f};
    second = {0.0f, 0.0f, -1.0f};
    __cl__8VICameraR7VIVect3T1(camera, &first, &second);
    *reinterpret_cast<ConsolePad*>(base + 0xb73c) = *reinterpret_cast<ConsolePad*>(base + 0xb6e8);
    Read__5VIPadR12VIConsolePad(base + 0xb470, reinterpret_cast<ConsolePad*>(base + 0xb6e8));
    Render__7VISceneR8VICamera(scene, camera);
    Update__15VIWindowManager(manager);
    Update__7VIFader(fader);
    EndScene__8VIRaster(raster);
    if (*idle == 0) {
        if (*reinterpret_cast<float*>(base + 0xb450) == 1.0f) {
            *focus = 0;
            SetFocus__5VIWndP5VIWndb(manager, 0, true);
            finished = true;
        }
        if (*idle == 0) {
            mActiveTries += 1;
            if (mActiveTries >= 16) {
                *focus = 0;
                SetFocus__5VIWndP5VIWndb(manager, 0, true);
                finished = true;
            }
        }
    }
    if (!finished) {
        return 0;
    }
    strcpy(reinterpret_cast<char*>(base + 0x404), reinterpret_cast<const char*>(base + 0xbc64));
    strcpy(reinterpret_cast<char*>(base + 0x504), reinterpret_cast<const char*>(base + 0xbca4));
    return *reinterpret_cast<int*>(base + 0xb820 + 0x578);
}
