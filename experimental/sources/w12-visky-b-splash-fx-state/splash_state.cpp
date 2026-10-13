// Observed in the pinned image: the five file-local words of the splash effect, all zero in the
// file and inside the file-backed data of the VI module segment. VISplashFxReset__Fv writes all
// five (see w11-vi-render-c-splash-fx-reset); the effect's per-frame routine reads them.
//   0x011e2800  VISplashFx$sNextFlockDelay   int
//   0x011e2810  VISplashFx$sLastAnimTime     64-bit value
//   0x011e2818  VISplashFx$sGlowStep         int
//   0x011e281c  VISplashFx$sGlowDir          int
//   0x011e2820  VISplashFx$sNextBoltDelay    int
// The names are the ones the sibling units publish for these image-local symbols; the sizes are
// those of the accesses in the reset routine (a doubleword store for the time, word stores for the
// rest). Unknown: whether other file-local words sit between these addresses.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int norrath_splash_next_flock_delay __asm__("VISplashFx$sNextFlockDelay") NORRATH_LOADED(".data") = 0;
long long norrath_splash_last_anim_time __asm__("VISplashFx$sLastAnimTime") NORRATH_LOADED(".data") = 0;
int norrath_splash_glow_step __asm__("VISplashFx$sGlowStep") NORRATH_LOADED(".data") = 0;
int norrath_splash_glow_dir __asm__("VISplashFx$sGlowDir") NORRATH_LOADED(".data") = 0;
int norrath_splash_next_bolt_delay __asm__("VISplashFx$sNextBoltDelay") NORRATH_LOADED(".data") = 0;
}
