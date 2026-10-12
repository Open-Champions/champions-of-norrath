/* Observed sceSifExitRpc at 0x002d7aa8, 36 bytes. Calls sceSifExitCmd, then
 * stores zero in the word _sceSifInitCheck at 0x00444968. That word is
 * defined by the sony-cd unit. Returns nothing.
 */
extern "C" void sceSifExitCmd();
extern "C" int _sceSifInitCheck;

extern "C" void sceSifExitRpc() {
    sceSifExitCmd();
    _sceSifInitCheck = 0;
}
