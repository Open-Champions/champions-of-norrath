// objectDrawEnable at 0x00483ac8, one .sdata byte, next objectVisUseCamCenter.
// The file byte is 0x01. drawObjectsTask returns immediately when this byte is zero.
// The reviewed -G0 profile does not place this byte in .sdata.
extern "C" {
signed char objectDrawEnable = 1;
}
static_assert(sizeof objectDrawEnable == 1u, "objectDrawEnable width");
