// Host check only; not a build input.
#include "setstationname.cpp"
#include <cstdio>
#include <cstring>

static const void* seen_dest;
static const char* seen_src;
static unsigned long seen_count;
static int calls;
extern "C" char* strncpy(char* dest, const char* source, unsigned long count) {
    seen_dest = dest; seen_src = source; seen_count = count; ++calls;
    return dest;
}

int main() {
    static unsigned char object[2048];
    bool ok = true;
    const char text[] = "abc";
    SetStationName__20VIWndGenericRendererPCc(object, text);
    ok = ok && calls == 1 && seen_dest == object + 1092 && seen_src == text && seen_count == 63;
    calls = 0;
    SetStationName__20VIWndGenericRendererPCc(object, nullptr);
    ok = ok && calls == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
