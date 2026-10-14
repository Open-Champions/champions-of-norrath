// Host check only; not a build input. Also compared with the original in a private interpreter (5 texts, all 43
// words covered). strlen is renamed so the check's own stub does not replace the library routine. The character
// converter is a stub whose results exercise the byte swap and the 16-bit truncation (upper bits set).
#define strlen host_strlen
#include "ascii_string_to_sjis.cpp"
#undef strlen
#include <cstdio>
#include <cstring>
#include <string>

extern "C" unsigned host_strlen(const char* text) { return static_cast<unsigned>(std::strlen(text)); }
static std::string trace;
// Scripted converter results: a function of the character, with bits set above the low 16.
static unsigned scripted_code(unsigned c) { return 0x12340000u | (((c * 0x0101u) & 0xffffu) ^ (c & 0x80 ? 0x1000u : 0u) ^ (c << 3)); }
extern "C" unsigned Ascii2Sjis__16VIIconDefinitionc(void* self, char character) {
    trace += (self ? "s" : "n") + std::to_string(static_cast<int>(character)) + ";";
    const unsigned c = static_cast<unsigned char>(character);
    return scripted_code(c);
}

int main() {
    bool ok = true;
    int object;
    for (const char* text : {"", "a", "Hello", "\x80\xff\x01z", "this text is longer than a handful of characters"}) {
        unsigned short out[80];
        std::memset(out, 0x77, sizeof out);
        trace.clear();
        unsigned short* end = AsciiString2Sjis__16VIIconDefinitionPcPUs(&object, text, out);
        const size_t length = std::strlen(text);
        std::string want_trace;
        bool this_ok = end == out + length;
        for (size_t i = 0; i < length; ++i) {
            const unsigned c = static_cast<unsigned char>(text[i]);
            const unsigned code = scripted_code(c);
            // The original ORs the shifted full word: for a 16-bit result this is a byte swap, and bits 16-23 of a wider
            // result also reach the high byte.
            const unsigned short swapped = static_cast<unsigned short>(((code & 0xffu) << 8) | (code >> 8));
            want_trace += "s" + std::to_string(static_cast<int>(static_cast<char>(text[i]))) + ";";
            this_ok = this_ok && out[i] == swapped;
        }
        this_ok = this_ok && out[length] == 0 && trace == want_trace;
        for (size_t i = length + 1; i < 80; ++i) this_ok = this_ok && out[i] == 0x7777;
        ok = ok && this_ok;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
