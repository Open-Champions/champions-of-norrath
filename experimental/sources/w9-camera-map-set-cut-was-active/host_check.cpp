// Host contract for SetCutWasActive__6Camera. Not an EE plan input.
#include "set_cut_was_active.cpp"

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
    camera[365] = 0;
    SetCutWasActive__6Camera(camera);
    expect(camera[365] == 5, "byte 365 becomes 5");
    unsigned char want[400];
    std::memset(want, 0x33, sizeof want);
    want[365] = 5;
    expect(std::memcmp(camera, want, sizeof camera) == 0, "no other byte changes");
    return failures ? 1 : 0;
}
