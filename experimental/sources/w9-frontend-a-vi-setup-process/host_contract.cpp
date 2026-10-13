#include "process.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static unsigned char* object_base;
static long long clock_now;
static int update_result;
static int word_after_frame;
static bool word_changes;

static std::string at(const void* pointer) {
    return pointer == nullptr ? "null" : std::to_string(static_cast<const unsigned char*>(pointer) - object_base);
}
static std::string bits(float value) {
    unsigned word;
    std::memcpy(&word, &value, 4);
    return std::to_string(word);
}
static std::string vec(const Vect3* v) { return bits(v->x) + "/" + bits(v->y) + "/" + bits(v->z); }

extern "C" {
void Init__8VICameraR8VIRasterRC7VIVect3T2ffff(void* camera, void* raster, Vect3* a, Vect3* b, float f0, float f1, float f2, float f3) {
    trace += "cam@" + at(camera) + "," + at(raster) + ",B" + vec(a) + ",A" + vec(b) + "," + bits(f0) + "," + bits(f1) + "," + bits(f2) +
             "," + bits(f3) + ";";
}
void BeginScene__8VIRaster(void* raster) { trace += "begin@" + at(raster) + ";"; }
long long GetTime__7VITimer() { return clock_now += 9; }
void SetTime__7VIScenel(void* scene, long long time) { trace += "settime@" + at(scene) + "," + std::to_string(time) + ";"; }
void __cl__8VICameraR7VIVect3T1(void* camera, Vect3* a, Vect3* b) { trace += "call@" + at(camera) + "," + vec(a) + "," + vec(b) + ";"; }
void Read__5VIPadR12VIConsolePad(void* pad, ConsolePad* state) { trace += "read@" + at(pad) + "," + at(state) + ";"; }
void Process__13VISoundDevice(void* sound) { trace += "sound@" + at(sound) + ";"; }
void Render__7VISceneR8VICamera(void* scene, void* camera) { trace += "render@" + at(scene) + "," + at(camera) + ";"; }
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag) {
    trace += "focus@" + at(manager) + "," + at(window) + "," + std::to_string(flag) + ";";
}
int Update__15VIWindowManager(void* manager) {
    trace += "wmupdate@" + at(manager) + ";";
    return update_result;
}
void Update__7VIFader(void* fader) { trace += "faderupdate@" + at(fader) + ";"; }
void EndScene__8VIRaster(void* raster) {
    trace += "end@" + at(raster) + ";";
    // this + 0xd5a8 and splash + 0x25c0 are the same word; the frame may change it before it is read again.
    if (word_changes) std::memcpy(object_base + 0xd5a8, &word_after_frame, 4);
}
void Begin__7VIFaderff(void* fader, float from, float to) {
    trace += "fade@" + at(fader) + "," + std::to_string(static_cast<int>(from * 10)) + "," + std::to_string(static_cast<int>(to * 10)) + ";";
}
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window) { trace += "root@" + at(manager) + "," + at(window) + ";"; }
void StartAmbientSoundtrack__7VISetup(void* setup) { trace += "ambient@" + at(setup) + ";"; }
}

static void put(unsigned offset, int value) { std::memcpy(object_base + offset, &value, 4); }
static void put_float(unsigned offset, float value) { std::memcpy(object_base + offset, &value, 4); }

struct Setup {
    int focus_word = 0;
    int update = 0;
    int state = 0;
    int splash_state = 0;
    int word_153c8 = 0;
    int word_after = 0;
    float fade_value = 0.0f;
};

static int run(const Setup& s) {
    static unsigned char storage[0x20000];
    std::memset(storage, 0, sizeof storage);
    object_base = storage;
    for (int i = 0; i < 84; ++i) storage[0xaeb8 + i] = static_cast<unsigned char>(i + 1);
    put(0xd5a8, s.focus_word);
    put(0x153cc, s.state);
    put(0xafe8 + 0x198, s.splash_state);
    put(0x153c8, s.word_153c8);
    word_after_frame = s.word_after;
    word_changes = s.word_after != 0;
    put_float(0xae40, s.fade_value);
    update_result = s.update;
    clock_now = 0;
    trace.clear();
    const int result = Process__7VISetup(storage);
    for (int i = 0; i < 84; ++i) {
        if (storage[0xaf0c + i] != i + 1) return -100;
    }
    return result;
}

int main() {
    const std::string neg0 = "2147483648";
    const std::string head = "cam@20112,0,B0/0/0,A" + neg0 + "/" + neg0 + "/" + neg0 + ",1113325568,1068948333,1036831948,1148846080;begin@0;settime@25120,9;"
                             "call@20112,0/0/1065353216,0/0/3212836864;read@44632,44728;sound@20632;render@25120,20112;";
    Setup s;
    // Plain frame: window manager update reports done, so only the common calls run.
    s.update = 1;
    int r = run(s);
    if (r != 0 || trace != head + "wmupdate@44900;faderupdate@44600;end@0;") {
        std::puts(trace.c_str());
        std::puts(head.c_str());
        return 1;
    }
    // Focus word 1 clears the update flag and forces result 4; state 0 becomes 2.
    s = Setup();
    s.focus_word = 1;
    r = run(s);
    if (r != 4 || trace != head + "focus@44900,null,1;faderupdate@44600;end@0;fade@44600,10,0;root@44900,45032;focus@44900,45032,1;fade@44600,0,5;ambient@0;") return 2;
    // State 2 behaves the same without the focus word, and stores 3.
    s = Setup();
    s.state = 2;
    r = run(s);
    if (r != 0) return 3;
    int stored;
    std::memcpy(&stored, object_base + 0x153cc, 4);
    if (stored != 3) return 4;
    // State 5, splash word 0 returns 3.
    s = Setup();
    s.state = 5;
    r = run(s);
    if (r != 3 || trace != head + "wmupdate@44900;faderupdate@44600;end@0;") return 5;
    // Splash word 2 returns the result code (0).
    s.splash_state = 2;
    if (run(s) != 0) return 6;
    // Splash word 1 with 0x153c8 == 1 clears it and restarts the fade.
    s.splash_state = 1;
    s.word_153c8 = 1;
    r = run(s);
    std::memcpy(&stored, object_base + 0x153c8, 4);
    if (r != 0 || stored != 0 || trace != head + "wmupdate@44900;faderupdate@44600;end@0;fade@44600,10,5;") return 7;
    // Otherwise splash + 0x25c0 == 1 returns 4.
    s.word_153c8 = 0;
    s.word_after = 1;
    if (run(s) != 4) return 8;
    // Otherwise a 1.0f at 0xae40 returns 2, and anything else 0.
    s.word_after = 0;
    s.fade_value = 1.0f;
    if (run(s) != 2) return 9;
    s.fade_value = 0.5f;
    if (run(s) != 0) return 10;
    // The frame wait needs the clock to advance by 18: the stub moves 9 per reading.
    if (clock_now != 9 * 4) return 11;
    std::puts("vi setup process passed");
    return 0;
}
