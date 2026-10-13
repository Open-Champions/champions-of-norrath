// Observed textureDrawFloat__FP8_textureffiUiff at 0x001726c8, extent 64.
// (texture, x, y, int, unsigned, float, float). It scales the first float by 640
// and then by a word at 0x0047fff8, scales the second float by 224 and then by a
// word at 0x0047fffc, and calls _textureDrawFloat__FP8_textureffiUiff (0x0016d978)
// with the scaled pair and every other argument unchanged. It returns nothing.
// Observed values of the two words in the image: 0x3acccccc (just under 1/640) and
// 0x3b924924 (just under 1/224). No symbol names those two words; they are written
// here as the constants 1/640 and 1/224 that the reviewed compiler folds to the
// same bit patterns (the host test and the paired run check the folded values).
// Inferred: the pair maps normalised screen coordinates to a 640x224 pixel grid
// and back; the net effect of the two multiplications is not an identity in
// floating point. Unknown: the meaning of the last two floats (passed through).
struct Texture;
extern "C" void _textureDrawFloat__FP8_textureffiUiff(Texture*, float, float, int, unsigned, float, float);

extern "C" void textureDrawFloat__FP8_textureffiUiff(Texture* texture, float x, float y, int a, unsigned color, float u, float v) {
    _textureDrawFloat__FP8_textureffiUiff(texture, x * 640.0f * (1.0f / 640.0f), y * 224.0f * (1.0f / 224.0f), a, color, u, v);
}
