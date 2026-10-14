// Host check only; not a build input.
#include "save.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

static const void* seen_data;
static int seen_size, next_result, calls;
static char seen_name[32];
extern "C" int VIMemCardWriteFile__FPCcPCvi(const char* name, const void* data, int size) {
    std::strncpy(seen_name, name, sizeof seen_name - 1);
    seen_data = data; seen_size = size; ++calls;
    return next_result;
}

int main() {
    static char options[1024];
    bool ok = true;
    for (int result : {0, 5, -1}) {
        next_result = result;
        ok = ok && Save__13VIGameOptions(options) == result && seen_data == options && seen_size == 1024 &&
             std::strcmp(seen_name, "game.opt") == 0;
    }
    ok = ok && calls == 3;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
