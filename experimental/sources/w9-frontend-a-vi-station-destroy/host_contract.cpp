#include "destroy.cpp"

#include <cstdio>
#include <string>

static std::string trace;
static unsigned char* object_base;

static std::string at(void* pointer) { return std::to_string(static_cast<unsigned char*>(pointer) - object_base); }

extern "C" void _$_9VIWndEula(void* object, int a0) {
    trace += "_$_9VIWndEula@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_20VIWndGenericRenderer(void* object, int a0) {
    trace += "_$_20VIWndGenericRenderer@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_15VIWindowManager(void* object, int a0) {
    trace += "_$_15VIWindowManager@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
}
extern "C" void _$_10VIKeyboard(void* object, int a0) {
    trace += "_$_10VIKeyboard@" + at(object) + "," + std::to_string(static_cast<int>(a0)) + ";";
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
    _$_9VIStation(storage, 0);
    if (trace != "_$_9VIWndEula@87048,2;_$_20VIWndGenericRenderer@47136,2;_$_15VIWindowManager@47000,2;_$_10VIKeyboard@46288,2;_$_5VIPad@46192,2;_$_7VIFader@46152,2;_$_4VIUI@46064,2;_$_7VIScene@26672,2;_$_13VISoundDevice@22184,2;_$_12VIDictionary@22160,2;_$_10VIMatrix44@21904,2;_$_10VIMatrix44@21840,2;_$_10VIMatrix44@21776,2;_$_10VIMatrix44@21712,2;_$_9VICollide@21104,2;_$_8VIRaster@1552,2;") {
        std::puts(trace.c_str());
        return 1;
    }
    trace.clear();
    _$_9VIStation(storage, 1);
    if (trace != "_$_9VIWndEula@87048,2;_$_20VIWndGenericRenderer@47136,2;_$_15VIWindowManager@47000,2;_$_10VIKeyboard@46288,2;_$_5VIPad@46192,2;_$_7VIFader@46152,2;_$_4VIUI@46064,2;_$_7VIScene@26672,2;_$_13VISoundDevice@22184,2;_$_12VIDictionary@22160,2;_$_10VIMatrix44@21904,2;_$_10VIMatrix44@21840,2;_$_10VIMatrix44@21776,2;_$_10VIMatrix44@21712,2;_$_9VICollide@21104,2;_$_8VIRaster@1552,2;delete@0;") {
        std::puts(trace.c_str());
        return 2;
    }
    trace.clear();
    _$_9VIStation(storage, 2);
    if (trace != "_$_9VIWndEula@87048,2;_$_20VIWndGenericRenderer@47136,2;_$_15VIWindowManager@47000,2;_$_10VIKeyboard@46288,2;_$_5VIPad@46192,2;_$_7VIFader@46152,2;_$_4VIUI@46064,2;_$_7VIScene@26672,2;_$_13VISoundDevice@22184,2;_$_12VIDictionary@22160,2;_$_10VIMatrix44@21904,2;_$_10VIMatrix44@21840,2;_$_10VIMatrix44@21776,2;_$_10VIMatrix44@21712,2;_$_9VICollide@21104,2;_$_8VIRaster@1552,2;") {
        std::puts(trace.c_str());
        return 3;
    }
    trace.clear();
    _$_9VIStation(storage, 3);
    if (trace != "_$_9VIWndEula@87048,2;_$_20VIWndGenericRenderer@47136,2;_$_15VIWindowManager@47000,2;_$_10VIKeyboard@46288,2;_$_5VIPad@46192,2;_$_7VIFader@46152,2;_$_4VIUI@46064,2;_$_7VIScene@26672,2;_$_13VISoundDevice@22184,2;_$_12VIDictionary@22160,2;_$_10VIMatrix44@21904,2;_$_10VIMatrix44@21840,2;_$_10VIMatrix44@21776,2;_$_10VIMatrix44@21712,2;_$_9VICollide@21104,2;_$_8VIRaster@1552,2;delete@0;") {
        std::puts(trace.c_str());
        return 4;
    }
    std::puts("vi station destroy passed");
    return 0;
}
