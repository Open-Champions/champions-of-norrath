#include "destroy.cpp"

#include <cstdio>
#include <string>

static std::string trace;
static unsigned char* object_base;

static std::string at(void* pointer) { return std::to_string(static_cast<unsigned char*>(pointer) - object_base); }

extern "C" void _$_10VIWndLegal(void* object, int a0) {
    trace += "_$_10VIWndLegal@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_9VIWndEula(void* object, int a0) {
    trace += "_$_9VIWndEula@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_9VIWndDnas(void* object, int a0) {
    trace += "_$_9VIWndDnas@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_12VIWndPatcher(void* object, int a0) {
    trace += "_$_12VIWndPatcher@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_11VIWndSplash(void* object, int a0) {
    trace += "_$_11VIWndSplash@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_15VIWindowManager(void* object, int a0) {
    trace += "_$_15VIWindowManager@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_5VIPad(void* object, int a0) {
    trace += "_$_5VIPad@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_7VIFader(void* object, int a0) {
    trace += "_$_7VIFader@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_4VIUI(void* object, int a0) {
    trace += "_$_4VIUI@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_7VIScene(void* object, int a0) {
    trace += "_$_7VIScene@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_13VISoundDevice(void* object, int a0) {
    trace += "_$_13VISoundDevice@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_12VIDictionary(void* object, int a0) {
    trace += "_$_12VIDictionary@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_10VIMatrix44(void* object, int a0) {
    trace += "_$_10VIMatrix44@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_9VICollide(void* object, int a0) {
    trace += "_$_9VICollide@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_8VIRaster(void* object, int a0) {
    trace += "_$_8VIRaster@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void __builtin_delete(void* object) {
    trace += "delete@" + at(object) + ";";
}

int main() {
    static unsigned char storage[0x20000];
    object_base = storage;
    trace.clear();
    _$_7VISetup(storage, 0);
    if (trace != "_$_10VIWndLegal@86928,2;_$_9VIWndEula@77160,2;_$_9VIWndDnas@73824,2;_$_12VIWndPatcher@54704,2;_$_11VIWndSplash@45032,2;_$_15VIWindowManager@44900,2;_$_5VIPad@44632,2;_$_7VIFader@44600,2;_$_4VIUI@44512,2;_$_7VIScene@25120,2;_$_13VISoundDevice@20632,2;_$_12VIDictionary@20608,2;_$_10VIMatrix44@20352,2;_$_10VIMatrix44@20288,2;_$_10VIMatrix44@20224,2;_$_10VIMatrix44@20160,2;_$_9VICollide@19552,2;_$_8VIRaster@0,2;") {
        std::puts(trace.c_str());
        return 1;
    }
    trace.clear();
    _$_7VISetup(storage, 1);
    if (trace != "_$_10VIWndLegal@86928,2;_$_9VIWndEula@77160,2;_$_9VIWndDnas@73824,2;_$_12VIWndPatcher@54704,2;_$_11VIWndSplash@45032,2;_$_15VIWindowManager@44900,2;_$_5VIPad@44632,2;_$_7VIFader@44600,2;_$_4VIUI@44512,2;_$_7VIScene@25120,2;_$_13VISoundDevice@20632,2;_$_12VIDictionary@20608,2;_$_10VIMatrix44@20352,2;_$_10VIMatrix44@20288,2;_$_10VIMatrix44@20224,2;_$_10VIMatrix44@20160,2;_$_9VICollide@19552,2;_$_8VIRaster@0,2;delete@0;") {
        std::puts(trace.c_str());
        return 2;
    }
    trace.clear();
    _$_7VISetup(storage, 2);
    if (trace != "_$_10VIWndLegal@86928,2;_$_9VIWndEula@77160,2;_$_9VIWndDnas@73824,2;_$_12VIWndPatcher@54704,2;_$_11VIWndSplash@45032,2;_$_15VIWindowManager@44900,2;_$_5VIPad@44632,2;_$_7VIFader@44600,2;_$_4VIUI@44512,2;_$_7VIScene@25120,2;_$_13VISoundDevice@20632,2;_$_12VIDictionary@20608,2;_$_10VIMatrix44@20352,2;_$_10VIMatrix44@20288,2;_$_10VIMatrix44@20224,2;_$_10VIMatrix44@20160,2;_$_9VICollide@19552,2;_$_8VIRaster@0,2;") {
        std::puts(trace.c_str());
        return 3;
    }
    trace.clear();
    _$_7VISetup(storage, 3);
    if (trace != "_$_10VIWndLegal@86928,2;_$_9VIWndEula@77160,2;_$_9VIWndDnas@73824,2;_$_12VIWndPatcher@54704,2;_$_11VIWndSplash@45032,2;_$_15VIWindowManager@44900,2;_$_5VIPad@44632,2;_$_7VIFader@44600,2;_$_4VIUI@44512,2;_$_7VIScene@25120,2;_$_13VISoundDevice@20632,2;_$_12VIDictionary@20608,2;_$_10VIMatrix44@20352,2;_$_10VIMatrix44@20288,2;_$_10VIMatrix44@20224,2;_$_10VIMatrix44@20160,2;_$_9VICollide@19552,2;_$_8VIRaster@0,2;delete@0;") {
        std::puts(trace.c_str());
        return 4;
    }
    std::puts("vi setup destroy passed");
    return 0;
}
