// The data symbols are provided here by the test. The declarations in the unit are extern.
long long norrath_splash_last_anim_time_def __asm__("VISplashFx$sLastAnimTime") = 0x1111111122222222ll;
int norrath_splash_next_flock_delay_def __asm__("VISplashFx$sNextFlockDelay") = 55;
int norrath_splash_next_bolt_delay_def __asm__("VISplashFx$sNextBoltDelay") = 66;
int norrath_splash_glow_dir_def __asm__("VISplashFx$sGlowDir") = 77;
int norrath_splash_glow_step_def __asm__("VISplashFx$sGlowStep") = 88;

#include "splash_fx_reset.cpp"

static int order[16];
static int n;
static int next_rand;

extern "C" long long GetTime__7VITimer() { order[n++] = 1; return 0x123456789abcll; }
extern "C" int rand() { order[n++] = 2; return next_rand; }

int main() {
    next_rand = 0x7fff1234;
    VISplashFxReset__Fv();
    if (n != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (norrath_splash_last_anim_time_def != 0x123456789abcll) return 2;
    if (norrath_splash_next_flock_delay_def != 0) return 3;
    if (norrath_splash_next_bolt_delay_def != 150 + 0x34) return 4;
    if (norrath_splash_glow_dir_def != 1 || norrath_splash_glow_step_def != 0) return 5;
    next_rand = 0xff;
    VISplashFxReset__Fv();
    if (norrath_splash_next_bolt_delay_def != 150 + 255) return 6;
    next_rand = -1;
    VISplashFxReset__Fv();
    if (norrath_splash_next_bolt_delay_def != 150 + 255) return 7;
    return 0;
}
