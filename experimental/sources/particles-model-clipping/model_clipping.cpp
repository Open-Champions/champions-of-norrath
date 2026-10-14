// modelClippingOn at 0x0048407c, one .sdata byte, next modelDrawFade.
// The file byte is zero. drawObjectsTask stores 0 or 1 here.
// The reviewed -G0 profile may place this zero byte in BSS.
extern "C" {
unsigned char modelClippingOn;
}
static_assert(sizeof modelClippingOn == 1u, "modelClippingOn width");
