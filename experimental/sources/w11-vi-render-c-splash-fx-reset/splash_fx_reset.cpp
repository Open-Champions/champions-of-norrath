// Observed VISplashFxReset__Fv at 0x01006f98, 92 bytes. The file-local words of the splash effect
// are set in this order (addresses are the image's, names are its local symbol names published
// with the same `VISplashFx$` prefix as the sibling units):
//   0x011e2810 (sLastAnimTime)    64-bit result of GetTime__7VITimer()
//   0x011e2800 (sNextFlockDelay)  0
//   0x011e2820 (sNextBoltDelay)   150 + (rand() & 255)
//   0x011e281c (sGlowDir)         1
//   0x011e2818 (sGlowStep)        0
// GetTime is called before rand. Nothing is returned.
// The units of the delays and the meaning of the glow step and direction are unknown.
extern "C" {
long long GetTime__7VITimer();
int rand();

extern long long norrath_splash_last_anim_time __asm__("VISplashFx$sLastAnimTime");
extern int norrath_splash_next_flock_delay __asm__("VISplashFx$sNextFlockDelay");
extern int norrath_splash_next_bolt_delay __asm__("VISplashFx$sNextBoltDelay");
extern int norrath_splash_glow_dir __asm__("VISplashFx$sGlowDir");
extern int norrath_splash_glow_step __asm__("VISplashFx$sGlowStep");

void VISplashFxReset__Fv() {
    norrath_splash_last_anim_time = GetTime__7VITimer();
    norrath_splash_next_flock_delay = 0;
    norrath_splash_next_bolt_delay = 150 + (rand() & 0xff);
    norrath_splash_glow_dir = 1;
    norrath_splash_glow_step = 0;
}
}
