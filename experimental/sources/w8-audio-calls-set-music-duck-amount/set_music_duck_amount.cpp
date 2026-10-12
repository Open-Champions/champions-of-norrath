/* soundSetMusicDuckAmount__Ff at 0x0024f4a0 is 20 bytes. It stores
 * 1.0f - amount in desiredMusicDuckAmount (0x00485968). Nothing else is
 * read or written.
 */
extern "C" {
extern float desiredMusicDuckAmount;

void soundSetMusicDuckAmount__Ff(float amount) {
    desiredMusicDuckAmount = 1.0f - amount;
}
}
