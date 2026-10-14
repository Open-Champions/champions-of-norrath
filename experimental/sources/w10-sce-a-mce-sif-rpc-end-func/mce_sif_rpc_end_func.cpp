/* Observed mceSifRpcEndFunc at 0x002e0de0, 12 bytes: the completion function
 * handed to sceSifCallRpc by the memory-card calls. Its argument is not used.
 * It signals the semaphore whose id is the word semaid_timerfunc
 * (0x00446078) with iSignalSema (the interrupt-context variant) as a tail
 * call and returns that result. Inferred: the end of an asynchronous card
 * call is announced this way; the observed code only signals the semaphore.
 */
extern "C" {
extern int semaid_timerfunc;

int iSignalSema(int semaphore);

int mceSifRpcEndFunc(void*) {
    return iSignalSema(semaid_timerfunc);
}
}
