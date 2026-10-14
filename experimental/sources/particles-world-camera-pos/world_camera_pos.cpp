// worldCameraPos at 0x0037ee80. The symbol size is 12 and the file bytes are zero.
// worldCameraMatrix begins 128 bytes later; the gap is not this object.
// drawObjectsTask reads the float at byte 8. The reviewed -G0 profile may
// place these zeros in BSS.
extern "C" {
unsigned char worldCameraPos[12];
}
static_assert(sizeof worldCameraPos == 12u, "worldCameraPos extent");
