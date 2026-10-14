// Host comparison only. Not an EE build input.
#include <cstdio>

static int seen_service;
static const char* seen_text;
static int calls;
static int reply = 7;
extern "C" int Deci2Call(int service, void* arguments) {
    seen_service = service;
    seen_text = *static_cast<const char**>(arguments);
    ++calls;
    return reply;
}
#include "kputs.cpp"

int main() {
    int failures = 0;
    const char text[] = "line";
    int r = kputs(text);
    if (calls != 1 || seen_service != 16 || seen_text != text) { std::printf("call shape wrong\n"); ++failures; }
    if (r != 7) { std::printf("result must be the Deci2Call result\n"); ++failures; }
    reply = -3;
    if (kputs(nullptr) != -3 || seen_text != nullptr) { std::printf("null pointer must be forwarded\n"); ++failures; }
    if (failures) return 2;
    std::printf("kputs cases passed\n");
    return 0;
}
