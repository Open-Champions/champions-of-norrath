/* Observed sceDeci2ExSend at 0x002df9b0, 52 bytes. Builds three words on the stack, the socket (first
 * argument), the buffer pointer (second) and the length masked to 16 bits (third), and
 * returns Deci2Call(-6, words) (0x002d5510). The service number is passed as a signed
 * immediate.
 * Inferred: the negative service numbers are the extended send calls; sceDeci2ExSend is called by
 * the TTY handler.
 * Unknown: the contract of Deci2Call beyond the argument block it receives.
 */
extern "C" {
int Deci2Call(int service, void* arguments);

int sceDeci2ExSend(int socket, void* buffer, unsigned length) {
    __UINTPTR_TYPE__ words[3];
    words[0] = static_cast<__UINTPTR_TYPE__>(socket);
    words[1] = reinterpret_cast<__UINTPTR_TYPE__>(buffer);
    words[2] = length & 0xffffu;
    return Deci2Call(-6, words);
}
}
