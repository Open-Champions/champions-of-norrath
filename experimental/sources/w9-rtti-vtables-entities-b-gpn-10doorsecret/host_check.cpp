#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" char levelName[200];
char levelName[200];

#include "msg_get_poke_name.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::vector<std::string> calls;
extern "C" void ustrcpy__FPUsPc(unsigned short* destination, const char* source) {
    calls.push_back(std::string("copy:") + source);
    destination[0] = static_cast<unsigned short>(source[0]);
    destination[1] = 0;
}
extern "C" int strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && a[i] == b[i]) ++i;
    return static_cast<unsigned char>(a[i]) - static_cast<unsigned char>(b[i]);
}

int main() {
    alignas(8) unsigned char object[128];
    unsigned short name[4];
    for (const char* level : {"cellar1", "cellar2", "cellar", "cellar11", "", "gothic"}) {
        std::memset(levelName, 'X', sizeof levelName);
        std::strcpy(levelName + 81, level);
        std::memset(name, 0xff, sizeof name);
        calls.clear();
        int r = msg_getPokeName__10DoorSecretPUsi(object, name, 99);
        expect(r == (std::strcmp(level, "cellar1") == 0 ? 1 : 0), "result is 1 only for cellar1");
        expect(calls.size() == 1 && calls[0] == "copy: " && name[0] == ' ' && name[1] == 0 && name[2] == 0xffff, "always copies a single space");
    }
    return failures ? 1 : 0;
}
