#include <cstdio>
#include <cstring>
extern "C" void norrath_vt_msg_alert_10GameObject(void* self, int code, float amount, const void* point, void* other);
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    unsigned char* point = new unsigned char[12];
    std::memset(object, 0x3c, sizeof object);
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(0x80 + i);
    norrath_vt_msg_alert_10GameObject(object, 7, 1.25f, point, object); norrath_vt_msg_alert_10GameObject(nullptr, 7, 1.25f, point, nullptr);
    bool intact = true;
    for (unsigned char b : object) if (b != 0x3c) intact = false;
    for (int i = 0; i < 12; ++i) if (point[i] != static_cast<unsigned char>(0x80 + i)) intact = false;
    expect(intact, "object and point stay put");
    delete[] point;
    return failures ? 1 : 0;
}
