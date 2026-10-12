/* Observed sceSifExitRpc at 0x002d7aa8, 36 bytes. Calls sceSifExitCmd, then
 * stores zero in the local word at 0x00444968 (named _sceSifInitCheck in the
 * symbol table). Returns nothing.
 */
extern "C" void sceSifExitCmd();

int sceSifInitCheck;

extern "C" void sceSifExitRpc() {
    sceSifExitCmd();
    sceSifInitCheck = 0;
}
