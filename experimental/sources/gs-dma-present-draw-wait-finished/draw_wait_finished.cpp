/* Observed drawWaitFinished__Fv at 0x0015faa0, 32 bytes. It waits on the
 * semaphore id in endDrawSemaphore (0x004868e0) and returns. Fresh
 * compilation is not an original-byte match.
 */
extern "C" int endDrawSemaphore;
extern "C" void WaitSema(int identifier);

extern "C" void drawWaitFinished__Fv() {
    WaitSema(endDrawSemaphore);
}
