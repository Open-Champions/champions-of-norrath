// Host contract for the render data definitions. Not an EE plan input.
#include "data_render_globals.cpp"
#include <cstdio>
static_assert(sizeof rainSplashParticle == 80 && sizeof slideDustParticleBigDark == 80, "particle size");
static_assert(sizeof scissor == 48, "scissor size");
int main() {
    if (modelDrawFade != 0 || modelShadowScale != 1.0f || modelOption != 0 || shadowBrightness != 0.25f) return 1;
    if (scissor[0] != 320.0f || scissor[1] != 224.0f || scissor[2] != 120.0f || scissor[3] != 100.0f) return 2;
    if (scissor[6] != 320.0f || scissor[9] != 100.0f || scissor[4] != 0.0f || scissor[11] != 0.0f) return 3;
    if (scx1 | scy1 | scx2 | scy2) return 4;
    unsigned char* all[] = {rainSplashParticle, slideDustParticle, slideDustParticleDark, slideDustParticleBig,
                            slideDustParticleBigDark, norrath_container_msg_run_fuseParticle_bytes,
                            norrath_container_msg_run_smokeParticle_bytes};
    for (unsigned char* a : all)
        for (int i = 0; i < 80; i++) if (a[i]) return 5;
    std::printf("render data contract passed\n");
    return 0;
}
