// Host contract for GetTargetNum__6Camera. Not an EE plan input.
#include "get_target_num.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned char camera[400];
    std::memset(camera, 0x33, sizeof camera);
    camera[353] = 3;
    expect(GetTargetNum__6Camera(camera) == 3, "positive byte");
    camera[353] = 0xff;
    expect(GetTargetNum__6Camera(camera) == -1, "sign extension");
    camera[353] = 0x80;
    expect(GetTargetNum__6Camera(camera) == -128, "lowest value");
    camera[353] = 0;
    camera[352] = 0x77; camera[354] = 0x77;
    expect(GetTargetNum__6Camera(camera) == 0, "neighbours are not read");
    expect(camera[352] == 0x77 && camera[354] == 0x77, "nothing is stored");
    return failures ? 1 : 0;
}
