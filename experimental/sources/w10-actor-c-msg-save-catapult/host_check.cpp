#include <cstdio>
#include <cstring>
extern "C" int msg_save__8CatapultPc(void*, char*);
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    unsigned char object[128];
    const int values[] = {-128, -1, 0, 1, 2, 127};
    const char want[] = {0, 0, 0, 1, 1, 1};
    for (int i = 0; i < 6; ++i) {
        std::memset(object, 0x11, sizeof object);
        object[96] = static_cast<unsigned char>(values[i]);
        unsigned char copy[128];
        std::memcpy(copy, object, sizeof object);
        char buffer[4] = {0x33, 0x44, 0x55, 0x66};
        int r = msg_save__8CatapultPc(object, buffer);
        expect(r == 1, "returns 1");
        expect(buffer[0] == want[i], "flag byte");
        expect(buffer[1] == 0x44 && buffer[2] == 0x55 && buffer[3] == 0x66, "only one byte written");
        expect(std::memcmp(object, copy, sizeof object) == 0, "object unchanged");
    }
    return failures ? 1 : 0;
}
