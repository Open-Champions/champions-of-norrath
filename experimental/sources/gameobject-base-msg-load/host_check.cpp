#include <cstdio>
#include <cstring>
extern "C" int norrath_vt_msg_load_10GameObject(void* self, char* data);
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    char buffer[8];
    std::memset(object, 0x3c, sizeof object);
    std::memset(buffer, 0x5a, sizeof buffer);
    int value = norrath_vt_msg_load_10GameObject(object, buffer);
    expect(value == 0, "returns zero");
    bool intact = true;
    for (unsigned char b : object) if (b != 0x3c) intact = false;
    for (unsigned char b : buffer) if (b != 0x5a) intact = false;
    expect(intact, "object and buffer stay put");
    expect(norrath_vt_msg_load_10GameObject(nullptr, buffer) == 0, "null self returns zero");
    return failures ? 1 : 0;
}
