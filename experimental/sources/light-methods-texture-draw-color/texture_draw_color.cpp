// Observed textureDrawColor__FP8_textureiiiUi at 0x172708, 28 bytes.
// It keeps the five incoming arguments and calls
// _textureDrawColor__FP8_textureiiiUi. The return word is that callee's
// result. The symbol spelling does not encode a return type.

extern "C" int _textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);

extern "C" int textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color) {
    return _textureDrawColor__FP8_textureiiiUi(texture, x, y, mode, color);
}
