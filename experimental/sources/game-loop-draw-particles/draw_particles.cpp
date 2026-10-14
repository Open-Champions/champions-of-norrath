// Observed drawParticlesTask at 0x00180018, 32 bytes.
// It calls P_DrawParticles with worldCameraMatrix and returns.
extern "C" void P_DrawParticles__FP8Matrix34(unsigned char* matrix);
extern "C" unsigned char worldCameraMatrix[];

extern "C" void drawParticlesTask__Fv() {
    P_DrawParticles__FP8Matrix34(worldCameraMatrix);
}
