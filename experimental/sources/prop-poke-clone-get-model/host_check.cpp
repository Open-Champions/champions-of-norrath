#include "get_model.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    std::memset(object, 0x3c, sizeof object);
    expect(getModel__10GameObject(object) == nullptr, "returns null");
    for (unsigned char b : object) expect(b == 0x3c, "object byte unchanged");
    expect(getModel__10GameObject(nullptr) == nullptr, "null self returns null");
    return failures ? 1 : 0;
}
