#include <cstdint>
#include <cstdio>

extern "C" int textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);

static int failures = 0;
static int calls = 0;
static void* seen_texture = nullptr;
static int seen_x = 0;
static int seen_y = 0;
static int seen_mode = 0;
static unsigned seen_color = 0;
static int result_word = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" int _textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color) {
    ++calls;
    seen_texture = texture;
    seen_x = x;
    seen_y = y;
    seen_mode = mode;
    seen_color = color;
    return result_word;
}

int main() {
    unsigned char texture[4] = {1, 2, 3, 4};
    result_word = -23;
    int returned = textureDrawColor__FP8_textureiiiUi(texture, 192, 330, 3, 0x80808080u);
    expect(calls == 1, "calls the inner color draw once");
    expect(seen_texture == texture && seen_x == 192 && seen_y == 330 && seen_mode == 3, "forwards position and mode");
    expect(seen_color == 0x80808080u, "forwards the color word");
    expect(returned == -23, "returns the inner result");

    result_word = 7;
    returned = textureDrawColor__FP8_textureiiiUi(nullptr, 64, 50, 3, 0u);
    expect(calls == 2 && seen_texture == nullptr && seen_x == 64 && seen_y == 50 && seen_color == 0u && returned == 7,
           "a null texture and a zero color still pass through");
    return failures == 0 ? 0 : 1;
}
