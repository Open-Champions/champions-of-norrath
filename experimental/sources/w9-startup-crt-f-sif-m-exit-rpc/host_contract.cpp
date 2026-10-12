// Host check only; not an EE build input.
int _sceSifMInitCheck = 1;
static unsigned seen_command, seen_identifier;
static int order;
extern "C" {
void sceSifRemoveCmdHandler(unsigned command) { seen_command = command; order = order * 10 + 1; }
int sceSifRemoveRebootNotifyHandler(unsigned identifier) { seen_identifier = identifier; order = order * 10 + 2; return -105; }
int sceSifMExitRpc();
}
int main() {
    int result = sceSifMExitRpc();
    if (_sceSifMInitCheck != 0 || order != 12) return 1;
    if (seen_command != 0x80000018u || seen_identifier != 0x80000014u) return 2;
    if (result != -105) return 3;
    return 0;
}
