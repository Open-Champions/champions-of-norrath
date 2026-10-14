/* Observed sceSifFreeIopHeap at 0x002dc2a0, 28 bytes. Forwards its argument
 * to sceSifFreeSysMemory and returns that result.
 */
extern "C" int sceSifFreeSysMemory(void* address);

extern "C" int sceSifFreeIopHeap(void* address) {
    return sceSifFreeSysMemory(address);
}
