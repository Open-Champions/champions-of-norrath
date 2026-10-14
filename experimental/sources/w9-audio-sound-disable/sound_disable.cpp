/* soundDisable__Ff at 0x0024f4b8 is 28 bytes. It stores the argument times
 * 60.0f, converted to an integer by truncation, in disableSoundPlaying
 * (0x00486bd0, gp-relative). The argument is in seconds if the frame rate is
 * 60 per second, which is inferred from the constant and from the frame
 * code that counts the word down; the original does not say so. Nothing
 * else is written and nothing is returned.
 */
extern "C" {
extern int disableSoundPlaying;

void soundDisable__Ff(float seconds) {
    disableSoundPlaying = static_cast<int>(seconds * 60.0f);
}
}
