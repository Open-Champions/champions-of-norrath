/* Observed Bounce__FiUsPv at 0x002cc1e0, 32 bytes. It ignores all three
 * arguments, calls iSignalSema with the word in sid (0x00486db4) and leaves
 * that call's result as its own. PowerOffHandler__FPv registers it with
 * iSetAlarm. Its signature is read from the mangled name: an integer, an
 * unsigned halfword and a pointer. Compiled output is not claimed to match
 * the original.
 */
extern "C" {
extern int sid;
int iSignalSema(int semaphore);
}

extern "C" int Bounce__FiUsPv(int, unsigned short, void*) {
    return iSignalSema(sid);
}
