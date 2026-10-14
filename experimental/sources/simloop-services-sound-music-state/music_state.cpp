/* Observed .sdata words. musicPlaying (0x00484bf8) and
 * emergencyMusicWaitAbort (0x00484c00) are 4 bytes each and start at zero in
 * the executable. musicDuckAmount (0x0048593c) is 4 bytes and starts as the
 * bits of 1.0f (0x3f800000). The word at 0x00484bfc between the first two is
 * a different object and is not defined here.
 */
extern "C" {
int musicPlaying = 0;
int emergencyMusicWaitAbort = 0;
float musicDuckAmount = 1.0f;
}
