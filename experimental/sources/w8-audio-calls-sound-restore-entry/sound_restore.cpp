/* soundRestore__Fb at 0x0024bc38 is 984 bytes. The authored body is
 * norrath_sound_restore (sources/game-sound-restore-update-build), which
 * the audio package notes assign to this address. This entry forwards the
 * flag unchanged.
 *
 * It is a separate small function, not an alias and not an original-byte
 * match: the selected build keeps the frozen restore.cpp, which has no
 * alias, and an alias cannot cross translation units.
 */
extern "C" {
void norrath_sound_restore(bool reset_hardware);

void soundRestore__Fb(bool reset_hardware) {
    norrath_sound_restore(reset_hardware);
}
}
