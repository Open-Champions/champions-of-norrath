// Host check only; not a build input. strlen and strcpy are the host's; the Shift-JIS conversion is a stub
// that records the copy it was given and the destination address.
#include <cstdio>
#include <cstring>
#include <string>
#include "settitle.cpp"

static std::string seen;
static void* seen_self;
static unsigned short* seen_out;
static int calls;
extern "C" void AsciiString2Sjis__16VIIconDefinitionPcPUs(void* self, char* text, unsigned short* out) {
    seen = text; seen_self = self; seen_out = out; ++calls;
}

struct Case { const char* text; int result; const char* copy; int split2; };

int main() {
    bool ok = true;
    const Case cases[] = {
        {"", 0, "", 32},
        {"abc", 0, "abc", 32},
        {"abc\ndef", 0, "abcdef", 6},
        {"\nabc", 0, "abc", 0},
        {"abc\n", 0, "abc", 6},
        {"a\nb\nc", -1, nullptr, 0},
        {"aaaaaaaaaaaaaaaa\nbbbbbbbbbbbbbbbb", 0, "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbb", 32},
        {"aaaaaaaaaaaaaaaaa\nb", -1, nullptr, 0},
        {"aaaaaaaaaaaaaaaa\nbbbbbbbbbbbbbbbbbbb", -1, nullptr, 0},
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 0, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 32},
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", -1, nullptr, 0},
    };
    for (const Case& c : cases) {
        unsigned char object[256];
        std::memset(object, 0x33, sizeof object);
        calls = 0;
        const int r = SetTitle__16VIIconDefinitionPCc(object, c.text);
        bool good = r == c.result;
        if (c.copy) {
            const unsigned short at6 = *reinterpret_cast<unsigned short*>(object + 6);
            good = good && calls == 1 && seen == c.copy && seen_self == object && seen_out == reinterpret_cast<unsigned short*>(object + 192);
            good = good && at6 == (std::strchr(c.text, '\n') ? 2 * (std::strchr(c.text, '\n') - c.text) : 32);
            good = good && at6 == c.split2;
        } else {
            good = good && calls == 0 && *reinterpret_cast<unsigned short*>(object + 6) == 0x3333;
        }
        ok = ok && good;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
