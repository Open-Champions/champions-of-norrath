#include "msg_route.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    std::memset(object, 0x3c, sizeof object);
    msg_route__10GameObjecti(object, 1);
    msg_route__10GameObjecti(object, 0);
    for (unsigned char b : object) expect(b == 0x3c, "object unchanged");
    return failures ? 1 : 0;
}
