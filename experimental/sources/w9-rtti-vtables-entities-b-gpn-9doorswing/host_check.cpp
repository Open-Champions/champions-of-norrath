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

static std::vector<std::string> calls;
static int disable_value;
static unsigned short entry[4] = {0x4142, 0x43, 0, 0};
static void* find_self;
extern "C" int findTagInt__10GameObjectPci(void* object, const char* tag, int index) {
    calls.push_back(std::string("find:") + tag + ":" + std::to_string(index));
    find_self = object;
    return disable_value;
}
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
    alignas(8) unsigned char object[128];
    unsigned short name[4];
    for (int value : {0, 1, -1, 7}) {
        disable_value = value;
        std::memset(name, 0xff, sizeof name);
        calls.clear();
        int r = msg_getPokeName__9DoorSwingPUsi(object, name, 99);
        expect(find_self == object && calls[0] == "find:disable:0", "asks for the integer tag disable, index 0");
        if (value == 0) {
            expect(r == 1, "enabled: returns 1");
            expect(calls.size() == 3 && calls[1] == "text:OpenDoor" && calls[2] == "copy", "looks up OpenDoor then copies");
            expect(name[0] == 0x4142 && name[1] == 0x43 && name[2] == 0xffff, "copies the entry into the buffer");
        } else {
            expect(r == 0, "disabled: returns 0");
            expect(calls.size() == 1 && name[0] == 0xffff, "disabled: buffer untouched");
        }
    }
    return failures ? 1 : 0;
}
