/* Observed textureDrawColor__FP8_textureiiiUi at 0x00172708, 28 bytes.
 * It saves ra, calls _textureDrawColor__FP8_textureiiiUi, and returns.
 * The five incoming arguments are not rewritten. The callee is 516 bytes
 * at 0x0016e658 and is not this symbol. Fresh compilation is not an
 * original-byte match.
 */
struct _texture;

extern "C" void _textureDrawColor__FP8_textureiiiUi(
    _texture* texture, int first, int second, int third, unsigned color);

extern "C" void textureDrawColor__FP8_textureiiiUi(
    _texture* texture, int first, int second, int third, unsigned color) {
    _textureDrawColor__FP8_textureiiiUi(texture, first, second, third, color);
}
