// Observed in the pinned image: _14VIEffectVolume$mScene at 0x011b1a98, 4 bytes, zero in the file,
// inside the file-backed data of the VI module segment. The EffectVolume global-init unit stores a
// scene pointer in it and the global-free unit clears it (see w11-vi-render-a-effectvolume-*).
// Inferred: a pointer-sized static member holding the scene that effect volumes belong to.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
void* norrath_effect_volume_scene __asm__("_14VIEffectVolume$mScene") NORRATH_LOADED(".data") = 0;
}
