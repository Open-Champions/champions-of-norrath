#define NORRATH_W8C_HOST 1
#include "save.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int calls = 0;
static const char* seen[8];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int objectSaveAddToStringPool__FPCc(const char* text) {
    seen[calls] = text;
    return 0x100 + calls++;
}
int main() {
    unsigned char object[200];
    unsigned char out[64];
    char strings[5][4] = {"a", "b", "c", "d", "e"};
    std::memset(object, 0, sizeof object);
    std::memset(out, 0x77, sizeof out);
    for (unsigned i = 0; i < 5; ++i) {
        w8c::host_note(strings[i]);
        w8c::write_word(w8c::address(object) + 76u + 4u * i, w8c::address_bits(strings[i]));
    }
    w8c::write_single(w8c::address(object) + 28u, -70000.75f);
    w8c::write_single(w8c::address(object) + 32u, 12.5f);
    w8c::write_word(w8c::address(object) + 104u, 0xdeadbeefu);
    const int written = msg_save__9ContainerPc(object, reinterpret_cast<char*>(out));
    expect(written == 28, "returns 28");
    expect(calls == 5, "five pool calls");
    for (int i = 0; i < 5; ++i) {
        expect(seen[i] == strings[i], "pool argument order");
        expect(w8c::read_word(w8c::address(out) + 4u * i) == 0x100u + i, "pool result stored in order");
    }
    expect(w8c::read_u16(w8c::address(out) + 20u) == static_cast<unsigned>(static_cast<unsigned short>(-70000)), "first float truncated toward zero");
    expect(w8c::read_u16(w8c::address(out) + 22u) == 12u, "second float truncated");
    expect(w8c::read_word(w8c::address(out) + 24u) == 0xdeadbeefu, "word at 104");
    expect(out[28] == 0x77, "nothing past 28 bytes");
    return failures ? 1 : 0;
}
