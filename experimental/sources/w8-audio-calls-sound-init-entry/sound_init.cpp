/* soundInit__Ff at 0x0024c010 is 428 bytes. The authored body is
 * norrath_sound_initialize (sources/game-sound-startup-build), whose
 * behavior the audio package notes assign to this address. This entry
 * forwards the incoming float to it unchanged and returns nothing.
 *
 * It is a separate small function, not an alias and not an original-byte
 * match: the selected build keeps the frozen initialize_sound.cpp, which has
 * no alias, and an alias cannot cross translation units.
 */
extern "C" {
void norrath_sound_initialize(float gain);

void soundInit__Ff(float gain) {
    norrath_sound_initialize(gain);
}
}
