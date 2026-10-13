// Observed: Process__9VIStation at 0x01017f90 is 1004 bytes; per-frame step for the
// station object (the same large object that Init__9VIStationiPCcPcT3T2T2i fills).
// Byte offsets are from the object. It returns 0 while running and, once finished, the
// word at offset 1400 of the renderer member (this + 0xb820).
//  1. If word 0x17a30 is nonzero and the word at 28 of the EULA window (this + 0x15408)
//     is nonzero: clear that word, set word 0xb81c to the renderer address,
//     SetFocus__5VIWndP5VIWndb(manager = this + 0xb798, renderer, true) and
//     RemoveChild__5VIWndP5VIWnd(renderer, eula). Word 0x17a30 is then cleared in all cases.
//  2. Process__13VISoundDevice(this + 22184).
//  3. Camera setup: Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera = this + 21664,
//     raster = this + 1552, {0,0,0}, {-0.0,-0.0,-0.0}, 55.0f, 0x3fb6db6d (about 1.4286f),
//     0x3dcccccc (about 0.1f), 1000.0f), BeginScene__8VIRaster(raster),
//     SetTime__7VIScenel(scene = this + 26672, GetTime__7VITimer()), then the camera
//     function-call operator __cl__8VICameraR7VIVect3T1(camera, {0,0,1}, {0,0,-1}).
//  4. The 84 bytes at 0xb6e8 are copied to 0xb73c, Read__5VIPadR12VIConsolePad(this +
//     0xb470, this + 0xb6e8) refreshes them, then Render__7VISceneR8VICamera(scene,
//     camera), Update__15VIWindowManager(manager), Update__7VIFader(this + 0xb448) and
//     EndScene__8VIRaster(raster).
//  5. If word 0xb468 is zero: when the float at 8 of the fader equals 1.0f, word 0xb81c =
//     0, SetFocus__5VIWndP5VIWndb(manager, null, true), the finished flag is set and
//     word 0xb468 is read again; otherwise the earlier value is kept. If that value is
//     zero a counter (a function-local static int, symbol mActiveTries.2910) is
//     incremented; at 16 or more the same clear-and-focus step runs and the flag is set.
//  6. If finished, strcpy(this + 1028, this + 0xbc64) and strcpy(this + 1284, this +
//     0xbca4) and the renderer word at 1400 is returned; otherwise 0.
// Inferred: the 84-byte block is a gamepad state with the previous state kept after it;
// the fader value 1.0 means fully faded in or out. Unknown: the roles of words 0xb81c,
// 0xb468 and 0x17a30, and what is at 28 of the EULA window.
extern "C" {
extern int norrath_station_active_tries __asm__("mActiveTries.2910");

void* strcpy(char* destination, const char* source);
long long GetTime__7VITimer();
void Process__13VISoundDevice(void* device);
int Init__8VICameraR8VIRasterRC7VIVect3T2ffff(void* camera, void* raster, const float* position, const float* target, float a, float b, float c, float d);
void BeginScene__8VIRaster(void* raster);
void SetTime__7VIScenel(void* scene, long time);
void __cl__8VICameraR7VIVect3T1(void* camera, const float* a, const float* b);
void Read__5VIPadR12VIConsolePad(void* pad, void* state);
void Render__7VISceneR8VICamera(void* scene, void* camera);
void Update__15VIWindowManager(void* manager);
void Update__7VIFader(void* fader);
void EndScene__8VIRaster(void* raster);
void SetFocus__5VIWndP5VIWndb(void* window, void* target, bool flag);
void RemoveChild__5VIWndP5VIWnd(void* window, void* child);

struct StationPadState {
    char bytes[84];
};

int Process__9VIStation(void* object) {
    char* base = static_cast<char*>(object);
    char* manager = base + 0xb798;
    char* renderer = base + 0xb820;
    char* fader = base + 0xb448;
    char* camera = base + 21664;
    char* raster = base + 1552;
    char* scene = base + 26672;
    bool running = true;
    int result = 0;
    if (*reinterpret_cast<int*>(base + 0x17a30) != 0) {
        char* eula = base + 0x15408;
        if (*reinterpret_cast<int*>(eula + 28) != 0) {
            *reinterpret_cast<int*>(eula + 28) = 0;
            *reinterpret_cast<void**>(base + 0xb81c) = renderer;
            SetFocus__5VIWndP5VIWndb(manager, renderer, true);
            RemoveChild__5VIWndP5VIWnd(renderer, eula);
        }
    }
    *reinterpret_cast<int*>(base + 0x17a30) = 0;
    Process__13VISoundDevice(base + 22184);
    alignas(16) float first[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float second[4] = {-0.0f, -0.0f, -0.0f, 0.0f};
    Init__8VICameraR8VIRasterRC7VIVect3T2ffff(camera, raster, first, second, 55.0f,
        __builtin_bit_cast(float, 0x3fb6db6du), __builtin_bit_cast(float, 0x3dccccccu), 1000.0f);
    BeginScene__8VIRaster(raster);
    SetTime__7VIScenel(scene, static_cast<long>(GetTime__7VITimer()));
    second[0] = 0.0f;
    second[1] = 0.0f;
    second[2] = 1.0f;
    first[2] = -1.0f;
    __cl__8VICameraR7VIVect3T1(camera, second, first);
    *reinterpret_cast<StationPadState*>(base + 0xb73c) = *reinterpret_cast<StationPadState*>(base + 0xb6e8);
    Read__5VIPadR12VIConsolePad(base + 0xb470, base + 0xb6e8);
    Render__7VISceneR8VICamera(scene, camera);
    Update__15VIWindowManager(manager);
    Update__7VIFader(fader);
    EndScene__8VIRaster(raster);
    int flag = *reinterpret_cast<int*>(base + 0xb468);
    if (flag == 0) {
        if (*reinterpret_cast<float*>(fader + 8) == 1.0f) {
            *reinterpret_cast<int*>(base + 0xb81c) = 0;
            SetFocus__5VIWndP5VIWndb(manager, nullptr, true);
            flag = *reinterpret_cast<int*>(base + 0xb468);
            running = false;
        }
        if (flag == 0) {
            int tries = norrath_station_active_tries + 1;
            norrath_station_active_tries = tries;
            if (tries >= 16) {
                *reinterpret_cast<int*>(base + 0xb81c) = 0;
                SetFocus__5VIWndP5VIWndb(manager, nullptr, true);
                running = false;
            }
        }
    }
    if (!running) {
        strcpy(base + 1028, base + 0xbc64);
        strcpy(base + 1284, base + 0xbca4);
        result = *reinterpret_cast<int*>(renderer + 1400);
    }
    return result;
}
}
