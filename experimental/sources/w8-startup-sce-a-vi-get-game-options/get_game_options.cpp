/* Observed VIGetGameOptions__Fv at 0x01019968, 12 bytes, in the 0x01000000
 * module image. It returns the address 0x011e2dd0, the local object
 * sGameOptions, and reads nothing. The object's size and contents are not
 * established here, so it is an unsized import. Username__C13VIGameOptions
 * and Password__C13VIGameOptions are called on this address by the launcher
 * station code. Compiled output is not claimed to match the original.
 */
extern "C" {
extern char sGameOptions[];

void* VIGetGameOptions__Fv() {
    return sGameOptions;
}
}
