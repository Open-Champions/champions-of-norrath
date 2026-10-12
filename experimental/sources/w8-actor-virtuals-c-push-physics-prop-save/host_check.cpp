#define NORRATH_W8C_HOST 1
#include "save.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int calls = 0;
static const char* seen[4];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int objectSaveAddToStringPool__FPCc(const char* text) {
    seen[calls] = text;
    return 0x200 + calls++;
}
int main() {
    unsigned char object[300];
    unsigned char out[96];
    char strings[3][4] = {"a", "b", "c"};
    std::memset(object, 0, sizeof object);
    std::memset(out, 0x66, sizeof out);
    using namespace w8c;
    write_word(address(object) + 256u, 0x3f800000u);
    for (unsigned i = 0; i < 3; ++i) {
        host_note(strings[i]);
        write_word(address(object) + 268u + 4u * i, address_bits(strings[i]));
    }
    for (unsigned i = 0; i < 13; ++i) write_word(address(object) + 96u + 4u * i, 0x40000000u + i);
    write_word(address(object) + 148u, 0xaaaaaaaau);
    const int written = msg_save__15PushPhysicsPropPc(object, reinterpret_cast<char*>(out));
    expect(written == 68 && calls == 3, "length and call count");
    expect(read_word(address(out)) == 0x3f800000u, "float 256 first");
    for (int i = 0; i < 3; ++i) {
        expect(seen[i] == strings[i], "string order");
        expect(read_word(address(out) + 4u + 4u * i) == 0x200u + i, "pool results in order");
    }
    for (unsigned i = 0; i < 13; ++i) expect(read_word(address(out) + 16u + 4u * i) == 0x40000000u + i, "float run");
    expect(out[68] == 0x66, "nothing after 68 bytes");
    return failures ? 1 : 0;
}
