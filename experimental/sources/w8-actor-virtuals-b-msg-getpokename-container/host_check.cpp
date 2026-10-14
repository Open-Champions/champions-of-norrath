#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "msg_get_poke_name.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static std::vector<std::string> log_calls;
static unsigned short entry[4] = {1, 2, 3, 0};
extern "C" unsigned short* textGetString__FPc(const char* key) {
    log_calls.push_back(std::string("text:") + key);
    return entry;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source) {
    log_calls.push_back(source == entry ? "copy:entry" : "copy:other");
    for (int i = 0; i < 4; ++i) destination[i] = source[i];
}

int main() {
    alignas(8) unsigned char object[128];
    unsigned short name[8];
    std::memset(object, 0, sizeof object);
    std::memset(name, 0xff, sizeof name);
    unsigned options = 4;
    std::memcpy(object + 104, &options, 4);
    int result = msg_getPokeName__9ContainerPUsi(object, name, 77);
    expect(result == 1, "returns 1");
    expect(log_calls == std::vector<std::string>({"text:Ignite", "copy:entry"}), "looks up then copies");
    expect(name[0] == 1 && name[2] == 3 && name[3] == 0 && name[4] == 0xffff, "buffer written");
    for (unsigned value : {0u, 3u, 0xfffffffbu}) {
        log_calls.clear();
        std::memcpy(object + 104, &value, 4);
        result = msg_getPokeName__9ContainerPUsi(object, name, 0);
        expect(result == 1 && log_calls.empty(), "without bit 2 only the return value");
    }
    return failures ? 1 : 0;
}
