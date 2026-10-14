// Observed sceMcMkdir at 0x002e10a0, 52 bytes. It calls sceMcOpen(port, slot, name, 64)
// with the incoming three arguments unchanged and the mode word 64 (0x40). When that
// result is zero, mcRunCmdNo (0x00446070) is set to 11. The open result is returned
// either way.
// Inferred: 64 is the create/directory mode bit and 11 is the pending-command number
// for this call; no table was read to confirm that naming.
extern "C" {
extern int mcRunCmdNo;
int sceMcOpen(int port, int slot, const char* name, int mode);

int sceMcMkdir(int port, int slot, const char* name) {
    const int result = sceMcOpen(port, slot, name, 64);
    if (result == 0) mcRunCmdNo = 11;
    return result;
}
}
