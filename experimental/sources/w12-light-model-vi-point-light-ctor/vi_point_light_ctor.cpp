#include "../w11-rend-a-common/rend.hpp"
// Observed __12VIPointLight at 0x010c3950, extent 80. Argument: the object (80 bytes, the size
// VIScene::CreateSprite allocates for sprite type 8). It calls the VISprite constructor
// __8VISprite on the object, then stores the address of _vt$12VIPointLight at +60, the int 8 at
// +52, and four zero floats at +64, +68, +72 and +76. Nothing is returned (the original leaves a
// vtable address in the result register, which is not a value).
// Inferred: a sprite-derived point light whose +52 field (the sprite type?) is 8, matching the
// type number CreateSprite uses. Unknown: what the zeroed floats are, and the meaning of +52.
extern "C" {
extern unsigned char vt_VIPointLight[] __asm__("_vt$12VIPointLight");
void __8VISprite(void* self);
void __12VIPointLight(void* self) {
    using namespace rend;
    __8VISprite(self);
    set_ptr(self, 60, vt_VIPointLight);
    at<i32>(self, 52) = 8;
    at<f32>(self, 64) = 0.0f;
    at<f32>(self, 68) = 0.0f;
    at<f32>(self, 72) = 0.0f;
    at<f32>(self, 76) = 0.0f;
}
}
