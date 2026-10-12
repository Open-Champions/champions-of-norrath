/* Observed PowerOffHandler__FPv at 0x002cc200, 48 bytes. It ignores its
 * argument, calls iSetAlarm(1000, Bounce__FiUsPv, 0), then stores 1 in
 * powerOffMode (0x004865f8, one byte) and returns. The result of iSetAlarm is
 * dropped. What unit the 1000 is in is unknown. Compiled output is not
 * claimed to match the original.
 */
extern "C" {
extern unsigned char powerOffMode;
int Bounce__FiUsPv(int, unsigned short, void*);
int iSetAlarm(int ticks, int (*handler)(int, unsigned short, void*), void* argument);
}

extern "C" void PowerOffHandler__FPv(void*) {
    iSetAlarm(1000, Bounce__FiUsPv, nullptr);
    powerOffMode = 1;
}
