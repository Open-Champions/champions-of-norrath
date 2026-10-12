#include "free.cpp"

#include <cstdio>
#include <string>

static std::string trace;
static unsigned char* object_base;

static std::string at(void* pointer) { return std::to_string(static_cast<unsigned char*>(pointer) - object_base); }

extern "C" void Free__9VIWndEula(void* object) {
    trace += "Free__9VIWndEula@" + at(object) + ";";
}
extern "C" void Close__4VIUI(void* object) {
    trace += "Close__4VIUI@" + at(object) + ";";
}
extern "C" int printf(const char* format, ...) {
    trace += std::string("printf:") + format + ";";
    return 0;
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
extern "C" void Free__7VIFader(void* object) {
    trace += "Free__7VIFader@" + at(object) + ";";
}
extern "C" void Close__8VIRaster(void* object) {
    trace += "Close__8VIRaster@" + at(object) + ";";
}
extern "C" void CleanUp__11VIGUIObject() {
    trace += "CleanUp__11VIGUIObject;";
}
extern "C" void Free__15VIWindowManager(void* object) {
    trace += "Free__15VIWindowManager@" + at(object) + ";";
}
extern "C" void Free__10VIKeyboard(void* object) {
    trace += "Free__10VIKeyboard@" + at(object) + ";";
}

int main() {
    static unsigned char storage[0x20000];
    object_base = storage;
    Free__9VIStation(storage);
    if (trace != "Free__9VIWndEula@87048;Close__4VIUI@46064;printf:qaa54\n;Clear__7VIScene@26672;printf:qaa\n;Close__13VISoundDevice@22184;printf:qaa1\n;Clear__12VIDictionary@22160;Close__9VICollide@21104;Free__7VIFader@46152;printf:qav\n;Close__8VIRaster@1552;CleanUp__11VIGUIObject;Free__15VIWindowManager@47000;Free__10VIKeyboard@46288;") {
        std::puts(trace.c_str());
        return 1;
    }
    std::puts("vi station free passed");
    return 0;
}
