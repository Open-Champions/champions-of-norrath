struct _texture {
    unsigned char bytes[4];
};

#include "texture_draw_color.cpp"

#include <cstdio>

static int calls;
static _texture* seen_texture;
static int seen_first;
static int seen_second;
static int seen_third;
static unsigned seen_color;

extern "C" void _textureDrawColor__FP8_textureiiiUi(
    _texture* texture, int first, int second, int third, unsigned color) {
    calls += 1;
    seen_texture = texture;
    seen_first = first;
    seen_second = second;
    seen_third = third;
    seen_color = color;
}

int main() {
    _texture marker;
    calls = 0;
    textureDrawColor__FP8_textureiiiUi(&marker, -2, 7, 9, 0xaabbccddu);
    if (calls != 1 || seen_texture != &marker) return 1;
    if (seen_first != -2 || seen_second != 7 || seen_third != 9) return 2;
    if (seen_color != 0xaabbccddu) return 3;
    textureDrawColor__FP8_textureiiiUi(nullptr, 0, 0, 0, 0u);
    if (calls != 2 || seen_texture != nullptr || seen_color != 0u) return 4;
    std::puts("texture draw color contract passed");
    return 0;
}
