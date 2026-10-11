#include <cstdio>
#include <cstring>
extern "C" int norrath_vt_msg_get_poke_name_10GameObject(void* self, unsigned short* text, int length);
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    unsigned short text[4];
    std::memset(object, 0x3c, sizeof object);
    std::memset(text, 0x5a, sizeof text);
    int value = norrath_vt_msg_get_poke_name_10GameObject(object, text, 8);
    expect(value == 0, "returns zero");
    bool intact = true;
    for (unsigned char b : object) if (b != 0x3c) intact = false;
    for (unsigned i = 0; i < sizeof text; ++i)
        if (reinterpret_cast<unsigned char*>(text)[i] != 0x5a) intact = false;
    expect(intact, "object and name stay put");
    expect(norrath_vt_msg_get_poke_name_10GameObject(nullptr, text, 8) == 0, "null self returns zero");
    return failures ? 1 : 0;
}
