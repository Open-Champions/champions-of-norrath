/* _GLOBAL_$D$soundSfxVolume at 0x0024fae8 is 32 bytes. This is the
 * compiler-made static destructor entry for the sound unit. It calls the
 * unit's local static initialization routine with phase 0 and priority
 * 0xffff (move a0,zero and li a1,0xffff before the call).
 *
 * That routine is already authored as norrath_sound_static_initialization.
 * For phase 0 it walks the 32 sequence records from the last to the first,
 * calling listDestroy on the list at offset 40 and the animation state
 * destructor (flag 2) on each record.
 */
extern "C" {
void norrath_sound_static_initialization(int phase, unsigned priority);

void _GLOBAL_$D$soundSfxVolume() {
    norrath_sound_static_initialization(0, 0xffffu);
}
}
