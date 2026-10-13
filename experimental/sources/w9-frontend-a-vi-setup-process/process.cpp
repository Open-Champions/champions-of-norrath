// Observed: Process__7VISetup at 0x0114fc48 is 1116 bytes. One call runs one frame and returns a
// result code (0 when nothing special happens). In order:
//  1. Init__8VICameraR8VIRasterRC7VIVect3T2ffff(this + 0x4e90, this, B, A, 55.0f, 0x3fb6db6d as a
//     float, 0x3dcccccc as a float, 1000.0f) where B is the vector (0, 0, 0) and A the vector
//     whose three components are negative zero.
//  2. BeginScene__8VIRaster(this); SetTime__7VIScenel(this + 0x6220, VITimer::GetTime()).
//  3. The camera is called as a function with (A, B) = ((0, 0, 1), (0, 0, -1)), i.e.
//     __cl__8VICameraR7VIVect3T1(this + 0x4e90, A, B).
//  4. The 84 bytes at this + 0xaeb8 are copied to this + 0xaf0c, then Read__5VIPadR12VIConsolePad
//     (this + 0xae58, this + 0xaeb8) runs, then Process__13VISoundDevice(this + 0x5098) and
//     Render__7VISceneR8VICamera(this + 0x6220, this + 0x4e90).
//  5. When the word at this + 0xd5a8 is 1: SetFocus__5VIWndP5VIWndb(this + 0xaf64, 0, 1), the update
//     flag becomes 0 and the result code becomes 4. Otherwise the update flag is
//     Update__15VIWindowManager(this + 0xaf64).
//  6. Update__7VIFader(this + 0xae38); EndScene__8VIRaster(this). Then it waits until
//     VITimer::GetTime() has advanced by at least 18 from a reading taken here.
//  7. A nonzero update flag returns the result code. Otherwise the state word at this + 0x153cc
//     is read; zero is replaced by 2. State 2 stores 3 there, runs Begin__7VIFaderff(fader
//     this + 0xae38, 1.0f, 0.0f), SetRoot and SetFocus(…, 1) of the window manager on the splash
//     window this + 0xafe8, Begin__7VIFaderff(fader, 0.0f, 0.5f) and
//     StartAmbientSoundtrack__7VISetup(this), and returns the result code. Any other nonzero state
//     reads the word at splash + 0x198: 0 returns 3; 1 continues; anything else returns the
//     result code. Continuing: when the word at this + 0x153c8 is 1 it is cleared and
//     Begin__7VIFaderff(fader, 1.0f, 0.5f) runs, returning the result code; otherwise when the
//     word at splash + 0x25c0 is 1 the result is 4; otherwise a float at this + 0xae40 equal to
//     1.0f makes the result 2.
// The words at this + 0xd5a8 and splash + 0x25c0 (0xafe8 + 0x25c0) are the same address, read at
// two points in the frame.
// The time functions return 64-bit values (the original subtracts them with dsubu).
// Inferred: the 84-byte copy is a console pad snapshot of the previous frame, from the pad being
// read straight after it. The return codes 2, 3 and 4 are request codes to Setup::main; their
// meanings are not established here (see setup_main).
// Unknown: the member types, the camera parameter meanings, and the negative-zero vector.

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
int Update__15VIWindowManager(void* manager);
void Update__7VIFader(void* fader);
void EndScene__8VIRaster(void* raster);
void Begin__7VIFaderff(void* fader, float from, float to);
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window);
void StartAmbientSoundtrack__7VISetup(void* setup);
}

namespace {
float float_from_bits(unsigned bits) {
    float value;
    __builtin_memcpy(&value, &bits, sizeof value);
    return value;
}
}

extern "C" int Process__7VISetup(void* self) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    void* const camera = base + 0x4e90;
    void* const scene = base + 0x6220;
    void* const manager = base + 0xaf64;
    void* const fader = base + 0xae38;
    void* const splash = base + 0xafe8;
    int result = 0;
    Vect3 first = {-0.0f, -0.0f, -0.0f};
    Vect3 second = {0.0f, 0.0f, 0.0f};
    Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera, self, &second, &first, 55.0f, float_from_bits(0x3fb6db6du),
                                              float_from_bits(0x3dccccccu), 1000.0f);
    BeginScene__8VIRaster(self);
    SetTime__7VIScenel(scene, GetTime__7VITimer());
    first = {0.0f, 0.0f, 1.0f};
    second = {0.0f, 0.0f, -1.0f};
    __cl__8VICameraR7VIVect3T1(camera, &first, &second);
    *reinterpret_cast<ConsolePad*>(base + 0xaf0c) = *reinterpret_cast<ConsolePad*>(base + 0xaeb8);
    Read__5VIPadR12VIConsolePad(base + 0xae58, reinterpret_cast<ConsolePad*>(base + 0xaeb8));
    Process__13VISoundDevice(base + 0x5098);
    Render__7VISceneR8VICamera(scene, camera);
    int updated;
    if (*reinterpret_cast<int*>(base + 0xd5a8) == 1) {
        SetFocus__5VIWndP5VIWndb(manager, 0, true);
        updated = 0;
        result = 4;
    } else {
        updated = Update__15VIWindowManager(manager);
    }
    Update__7VIFader(fader);
    EndScene__8VIRaster(self);
    const long long start = GetTime__7VITimer();
    while (GetTime__7VITimer() - start < 18) {
    }
    if (updated != 0) {
        return result;
    }
    int state = *reinterpret_cast<int*>(base + 0x153cc);
    if (state == 0) {
        state = 2;
        *reinterpret_cast<int*>(base + 0x153cc) = 2;
    }
    if (state == 0) {
        return result;
    }
    if (state == 2) {
        *reinterpret_cast<int*>(base + 0x153cc) = 3;
        Begin__7VIFaderff(fader, 1.0f, 0.0f);
        SetRoot__15VIWindowManagerP5VIWnd(manager, splash);
        SetFocus__5VIWndP5VIWndb(manager, splash, true);
        Begin__7VIFaderff(fader, 0.0f, 0.5f);
        StartAmbientSoundtrack__7VISetup(self);
        return result;
    }
    const int splash_state = *reinterpret_cast<int*>(static_cast<unsigned char*>(splash) + 0x198);
    if (splash_state == 0) {
        return 3;
    }
    if (splash_state != 1) {
        return result;
    }
    if (*reinterpret_cast<int*>(base + 0x153c8) == 1) {
        *reinterpret_cast<int*>(base + 0x153c8) = 0;
        Begin__7VIFaderff(fader, 1.0f, 0.5f);
        return result;
    }
    if (*reinterpret_cast<int*>(static_cast<unsigned char*>(splash) + 0x25c0) == 1) {
        return 4;
    }
    if (*reinterpret_cast<float*>(base + 0xae40) == 1.0f) {
        result = 2;
    }
    return result;
}
