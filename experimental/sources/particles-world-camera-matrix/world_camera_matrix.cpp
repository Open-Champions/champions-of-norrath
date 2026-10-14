// worldCameraMatrix at 0x0037ef00, 48 file bytes, all zero, next worldViewMatrix.
// drawParticlesTask passes this object to P_DrawParticles.
// The reviewed -G0 profile may place these zeros in BSS.
extern "C" {
unsigned char worldCameraMatrix[48];
}
static_assert(sizeof worldCameraMatrix == 48u, "worldCameraMatrix extent");
