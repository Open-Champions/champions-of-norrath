/* _GLOBAL_$I$soundSfxVolume at 0x0024fac8 is 32 bytes. This is the
 * compiler-made static constructor entry for the sound unit. It calls the
 * unit's local static initialization routine with phase 1 and priority
 * 0xffff (li a0,1 and li a1,0xffff before the call).
 *
 * That routine is already authored as norrath_sound_static_initialization
 * (sources/game-sound-startup-build/static_initialization.cpp). For phase 1
 * it sets byte 5 to 1 and word 40 to 0 in each of the 32 sequence records
 * and runs the dialog resource-map constructor. The authored routine is a
 * public symbol; the original's was local to the unit.
 */
extern "C" {
void norrath_sound_static_initialization(int phase, unsigned priority);

void _GLOBAL_$I$soundSfxVolume() {
    norrath_sound_static_initialization(1, 0xffffu);
}
}
