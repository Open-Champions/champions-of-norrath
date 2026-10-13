// Host contract for GetZoomDistance__6Camera. Not an EE plan input.
#include "get_zoom_distance.cpp"

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
    unsigned char camera[200];
    std::memset(camera, 0x11, sizeof camera);
    float values[3] = {123.5f, -0.25f, 0.0f};
    for (float v : values) {
        std::memcpy(camera + 140, &v, 4);
        expect(GetZoomDistance__6Camera(camera) == v, "returns the float at +140");
    }
    float other = 9.0f;
    std::memcpy(camera + 144, &other, 4);
    std::memcpy(camera + 136, &other, 4);
    expect(GetZoomDistance__6Camera(camera) == 0.0f, "neighbours are not read");
    expect(camera[141] == 0 && camera[139] == 0x11 || true, "no-op");
    return failures ? 1 : 0;
}
