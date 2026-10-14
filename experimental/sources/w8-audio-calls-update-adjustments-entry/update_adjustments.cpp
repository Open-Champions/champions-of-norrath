/* soundUpdateAdjustments__Fv at 0x0024c1c0 is 228 bytes. The authored body
 * is norrath_sound_update_adjustments
 * (sources/game-sound-restore-update-build/update_adjustments.cpp): it
 * scales the first two stream volumes by 256, reads the stereo word and the
 * stream-2 play status, and sends command 0x80. initialize_sound.cpp calls
 * that body by its authored name. This entry forwards to it with no
 * arguments.
 *
 * It is a separate small function, not an alias and not an original-byte
 * match.
 */
extern "C" {
void norrath_sound_update_adjustments();

void soundUpdateAdjustments__Fv() {
    norrath_sound_update_adjustments();
}
}
