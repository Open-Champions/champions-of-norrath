#include <cstdio>
#include <cstring>
#include <initializer_list>
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

static std::vector<std::string> calls;
static unsigned short entry[4] = {0x4142, 0x43, 0, 0};
extern "C" unsigned short* textGetString__FPc(const char* key) {
    calls.push_back(std::string("text:") + key);
    return entry;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source) {
    calls.push_back("copy");
    destination[0] = source[0];
    destination[1] = source[1];
}

int main() {
    alignas(8) unsigned char object[512];
    unsigned short name[4];
    for (unsigned flags : {0u, 4u, 0xfffffffbu, 0xffffffffu, 3u, 7u}) {
        std::memset(object, 0, sizeof object);
        std::memcpy(object + 292, &flags, 4);
        std::memset(name, 0xff, sizeof name);
        calls.clear();
        int r = msg_getPokeName__13AutoContainerPUsi(object, name, 99);
        expect(r == 1, "returns 1");
        if (flags & 4) {
            expect(calls.size() == 2 && calls[0] == "text:Ignite" && calls[1] == "copy", "looks up the key then copies");
            expect(name[0] == 0x4142 && name[1] == 0x43 && name[2] == 0xffff, "copies the entry into the buffer");
        } else {
            expect(calls.empty() && name[0] == 0xffff, "bit 2 clear: nothing happens");
        }
    }
    return failures ? 1 : 0;
}
