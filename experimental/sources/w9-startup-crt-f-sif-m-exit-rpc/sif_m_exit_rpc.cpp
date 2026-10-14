/* Observed sceSifMExitRpc at 0x002e46d0, 48 bytes. Stores zero in the word
 * _sceSifMInitCheck at 0x00447c48, calls sceSifRemoveCmdHandler(0x80000018),
 * and then returns the result of sceSifRemoveRebootNotifyHandler(0x80000014)
 * as a tail call. The original reserves a 16-byte frame and saves $ra even
 * though the last call is a jump. The return value is whatever the second
 * routine leaves in $v0; callers that treat this as void ignore it.
 */
extern "C" {
extern int _sceSifMInitCheck;

void sceSifRemoveCmdHandler(unsigned command);
int sceSifRemoveRebootNotifyHandler(unsigned identifier);

int sceSifMExitRpc() {
    _sceSifMInitCheck = 0;
    sceSifRemoveCmdHandler(0x80000018u);
    return sceSifRemoveRebootNotifyHandler(0x80000014u);
}
}
