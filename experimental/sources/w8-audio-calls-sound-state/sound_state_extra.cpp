/* Two more sound-unit words that the calls in this package use and that no
 * other authored unit defines.
 *
 *   0x00485948  4  .sdata  nextChannel             0 (channel search cursor)
 *   0x00485968  4  .sdata  desiredMusicDuckAmount  0.0f
 *   0x00485964  1  .sdata  paused                  0 (a local symbol in the
 *                          original; soundClear writes zero to it)
 *
 * findChannel reads and writes nextChannel. soundSetMusicDuckAmount writes
 * desiredMusicDuckAmount. The reader of desiredMusicDuckAmount (the music
 * ducking step) is not part of this file.
 */
extern "C" {
int nextChannel = 0;
float desiredMusicDuckAmount = 0.0f;
unsigned char paused = 0;
}
