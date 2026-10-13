/* Observed UnpreparePowerOff__Fv at 0x002cc2c0, 44 bytes. It passes the word tid
 * (0x00486db8, the thread id PreparePowerOff__Fv stores) to TerminateThread and
 * then calls sceCdPOffCallback(0, 0). The two zeros are the observed arguments:
 * zero in the handler position and zero in the argument position, which is read
 * here as clearing the registration; the library's own treatment of them is
 * unknown. Nothing is returned. The semaphore sid is left alone, and the stored
 * tid is not cleared. Compiled output is not claimed to match the original.
 */
extern "C" {
extern int tid;
int TerminateThread(int thread);
void sceCdPOffCallback(void (*handler)(void*), int argument);
}

extern "C" void UnpreparePowerOff__Fv() {
    TerminateThread(tid);
    sceCdPOffCallback(0, 0);
}
