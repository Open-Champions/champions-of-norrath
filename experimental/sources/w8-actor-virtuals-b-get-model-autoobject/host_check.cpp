#include <cstdio>
#include <cstring>

#include "get_model.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    alignas(8) unsigned char object[160];
    std::memset(object, 0x5a, sizeof object);
    object[144] = 0x78; object[145] = 0x56; object[146] = 0x34; object[147] = 0x12;
    expect(getModel__10AutoObject(object) == 0x12345678u, "returns the word at +144");
    object[143] = 0; object[148] = 0;
    expect(getModel__10AutoObject(object) == 0x12345678u, "neighbouring bytes are not read");
    object[147] = 0x80;
    expect(getModel__10AutoObject(object) == 0x80345678u, "high bit survives");
    return failures ? 1 : 0;
}
