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
static unsigned short open_entry[4] = {0x4f, 0x50, 0, 0};
static unsigned short gold_entry[4] = {0x47, 0x4c, 0, 0};
static float printf_args[8];
static unsigned short* printf_format;
static const unsigned short* copied_from;
extern "C" unsigned short* textGetString__FPc(const char* key) {
    calls.push_back(std::string("text:") + key);
    return std::strcmp(key, "OpenChest") == 0 ? open_entry : gold_entry;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source) {
    calls.push_back("copy");
    copied_from = source;
    for (int i = 0; i < 6; ++i) destination[i] = source[i];
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* destination, unsigned short* format, float a, float b, float c,
                                         float d, float e, float f, float g, float h) {
    calls.push_back("printf");
    printf_format = format;
    printf_args[0] = a; printf_args[1] = b; printf_args[2] = c; printf_args[3] = d;
    printf_args[4] = e; printf_args[5] = f; printf_args[6] = g; printf_args[7] = h;
    destination[0] = 0x31;
    destination[1] = static_cast<unsigned short>(static_cast<int>(a));
    destination[2] = 0;
    destination[3] = destination[4] = destination[5] = 0;
}

int main() {
    alignas(8) unsigned char object[512];
    unsigned short name[8];
    struct Case { unsigned word; short amount; };
    for (Case c : {Case{0, 0}, Case{0, 50}, Case{1, 0}, Case{1, -4}, Case{1, 1}, Case{0x80000000u, 250}, Case{7, 32767}}) {
        std::memset(object, 0x44, sizeof object);
        std::memcpy(object + 216, &c.word, 4);
        std::memcpy(object + 220, &c.amount, 2);
        std::memset(name, 0xff, sizeof name);
        calls.clear();
        int r = msg_getPokeName__9AutoChestPUsi(object, name, 9);
        expect(r == 1, "returns 1");
        if (c.word == 0) {
            expect(calls.size() == 2 && calls[0] == "text:OpenChest" && calls[1] == "copy" && copied_from == open_entry, "open chest text");
            expect(name[0] == 0x4f && name[1] == 0x50, "buffer holds the entry");
        } else if (c.amount > 0) {
            expect(calls.size() == 3 && calls[0] == "text:PickUpGold" && calls[1] == "printf" && calls[2] == "copy", "gold text formatted then copied");
            expect(printf_format == gold_entry && printf_args[0] == static_cast<float>(c.amount), "amount is the first float");
            expect(printf_args[1] == 0.0f && printf_args[2] == 0.0f && printf_args[7] == 0.0f, "other floats are zero");
            expect(name[0] == 0x31 && name[1] == static_cast<unsigned short>(c.amount), "buffer holds the formatted text");
        } else {
            expect(calls.empty() && name[0] == 0xffff, "nothing to show");
        }
    }
    return failures ? 1 : 0;
}
