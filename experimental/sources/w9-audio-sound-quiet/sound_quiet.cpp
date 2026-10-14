/* soundQuiet__Fv at 0x0024f4d8 is 100 bytes. It issues four sceSdRemote
 * calls, in this order, each with first argument 1, second argument 0x8010
 * and fourth argument 0:
 *
 *   third argument 0x0980, 0x0a80, 0x0981, 0x0a81
 *
 * The results are discarded and nothing is returned. What the sound
 * processor does with the commands is not established here; the pairs of
 * values 0x980/0x981 and 0xa80/0xa81 differ in the low bit only, which is
 * all that is observed.
 */
extern "C" {
int sceSdRemote(int one, int command, int argument, int extra);

void soundQuiet__Fv() {
    sceSdRemote(1, 0x8010, 0x980, 0);
    sceSdRemote(1, 0x8010, 0xa80, 0);
    sceSdRemote(1, 0x8010, 0x981, 0);
    sceSdRemote(1, 0x8010, 0xa81, 0);
}
}
