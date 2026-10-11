#include <cstdio>
#include <cstring>
extern "C" void* norrath_vt_get_model_10GameObject(void* self);
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
int main() {
    unsigned char object[32];
    std::memset(object, 0x3c, sizeof object);
    void* value = norrath_vt_get_model_10GameObject(object);
    expect(value == nullptr, "returns null");
    bool intact = true;
    for (unsigned char b : object) if (b != 0x3c) intact = false;
    expect(intact, "object bytes stay put");
    expect(norrath_vt_get_model_10GameObject(nullptr) == nullptr, "null self returns null");
    return failures ? 1 : 0;
}
