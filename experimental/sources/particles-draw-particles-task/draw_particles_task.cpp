// drawParticlesTask__Fv at 0x00180018, 32 bytes, read from the pinned
// SLUS-20565 image. Fresh compilation is not an original-byte match. No
// particle has been drawn on the EE.
//
// The task passes worldCameraMatrix, the 48 bytes at 0x0037ef00, to
// P_DrawParticles__FP8Matrix34. There is no other store.
extern "C" {
void P_DrawParticles__FP8Matrix34(void* matrix);
extern unsigned char worldCameraMatrix[];

void drawParticlesTask__Fv() {
    P_DrawParticles__FP8Matrix34(worldCameraMatrix);
}
}
