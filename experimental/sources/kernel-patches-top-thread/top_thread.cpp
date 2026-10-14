/* Observed topThread at 0x002d5de0, 216 bytes. InitThread starts it as the
 * body of the thread that carries out queued thread requests, and passes
 * topArg as the argument. It never returns. Each pass waits on the semaphore
 * in topSema, then takes the word at offset 0 of the argument, masks it with
 * 511 to get an index, and stores index + 1 back (unmasked). The request at
 * offset 8 + 2 * index is a pair of bytes: a kind and an argument byte.
 * Kind 0 calls WakeupThread, kind 1 RotateThreadReadyQueue, and kind 2
 * SuspendThread, each with the argument byte. Any other kind prints one
 * fixed diagnostic line through kprintf and passes no other argument.
 * Inferred: this is the consumer side of the ring that iWakeupThread fills
 * through the word at offset 4. The consumer cursor and the diagnostic text
 * are observed. The kind names are not original names.
 * Unknown: what the three thread services do inside the kernel.
 */
extern "C" {
extern int topSema;
int WaitSema(int semaphore);
int WakeupThread(int thread);
int RotateThreadReadyQueue(int argument);
int SuspendThread(int thread);
void kprintf(const char* format, ...);
void topThread(unsigned* ring) {
    unsigned char* requests = reinterpret_cast<unsigned char*>(ring) + 8;
    for (;;) {
        WaitSema(topSema);
        const unsigned index = ring[0] & 0x1ffu;
        ring[0] = index + 1;
        const unsigned char kind = requests[index * 2];
        const unsigned char argument = requests[index * 2 + 1];
        if (kind == 1) {
            RotateThreadReadyQueue(argument);
        } else if (kind == 0) {
            WakeupThread(argument);
        } else if (kind == 2) {
            SuspendThread(argument);
        } else {
            kprintf("## internel error in libkernl.a!\n");
        }
    }
}
}
