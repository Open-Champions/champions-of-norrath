#include "free.cpp"

#include <cstdio>
#include <string>

static std::string trace;
static unsigned char* object_base;

static std::string at(void* pointer) { return std::to_string(static_cast<unsigned char*>(pointer) - object_base); }

extern "C" void StopAmbientSoundtrack__7VISetup(void* object) {
    trace += "StopAmbientSoundtrack__7VISetup@" + at(object) + ";";
}
extern "C" void Free__7VIFader(void* object) {
    trace += "Free__7VIFader@" + at(object) + ";";
}
extern "C" void Free__9VIWndEula(void* object) {
    trace += "Free__9VIWndEula@" + at(object) + ";";
}
extern "C" void Free__10VIWndLegal(void* object) {
    trace += "Free__10VIWndLegal@" + at(object) + ";";
}
extern "C" void Free__9VIWndDnas(void* object) {
    trace += "Free__9VIWndDnas@" + at(object) + ";";
}
extern "C" void Free__12VIWndPatcher(void* object) {
    trace += "Free__12VIWndPatcher@" + at(object) + ";";
}
extern "C" void Free__11VIWndSplash(void* object) {
    trace += "Free__11VIWndSplash@" + at(object) + ";";
}
extern "C" void VISplashFxFree__Fv() {
    trace += "VISplashFxFree__Fv;";
}
extern "C" void Close__4VIUI(void* object) {
    trace += "Close__4VIUI@" + at(object) + ";";
}
extern "C" void Clear__7VIScene(void* object) {
    trace += "Clear__7VIScene@" + at(object) + ";";
}
extern "C" void Close__13VISoundDevice(void* object) {
    trace += "Close__13VISoundDevice@" + at(object) + ";";
}
extern "C" void Clear__12VIDictionary(void* object) {
    trace += "Clear__12VIDictionary@" + at(object) + ";";
}
extern "C" void Close__9VICollide(void* object) {
    trace += "Close__9VICollide@" + at(object) + ";";
}
extern "C" void Close__8VIRaster(void* object) {
    trace += "Close__8VIRaster@" + at(object) + ";";
}
extern "C" void Close__5VIPad(void* object) {
    trace += "Close__5VIPad@" + at(object) + ";";
}

int main() {
    static unsigned char storage[0x20000];
    object_base = storage;
    Free__7VISetup(storage);
    if (trace != "StopAmbientSoundtrack__7VISetup@0;Free__7VIFader@44600;Free__9VIWndEula@77160;Free__10VIWndLegal@86928;Free__9VIWndDnas@73824;Free__12VIWndPatcher@54704;Free__11VIWndSplash@45032;VISplashFxFree__Fv;Close__4VIUI@44512;Clear__7VIScene@25120;Close__13VISoundDevice@20632;Clear__12VIDictionary@20608;Close__9VICollide@19552;Close__8VIRaster@0;Close__5VIPad@44632;") {
        std::puts(trace.c_str());
        return 1;
    }
    std::puts("vi setup free passed");
    return 0;
}
