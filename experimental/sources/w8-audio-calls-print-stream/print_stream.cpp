/* soundPrintStream__Fi at 0x0024f448 is 84 bytes. It reads the count word
 * at offset 448 of stream record `index` (452 bytes each from `stream`).
 * When it is positive the original runs a countdown loop of that many
 * iterations with nothing in the body: no store, no call, no read. Its name
 * suggests a removed diagnostic print; that is an inference. This source
 * keeps the count read and the empty countdown and has no other effect.
 */
extern "C" {
extern unsigned char stream[];

void soundPrintStream__Fi(int index) {
    int remaining = *reinterpret_cast<int*>(stream + index * 452 + 448);
    while (remaining > 0) {
        --remaining;
        asm volatile("");
    }
}
}
