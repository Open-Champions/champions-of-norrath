// farClip at 0x004869d8, four .sbss bytes, next lightHaloTexture.
// The image value is zero. worldInit later stores bits 0x44e10000.
// The reviewed -G0 profile keeps this zero word in BSS.
extern "C" {
float farClip;
}
static_assert(sizeof farClip == 4u, "farClip width");
