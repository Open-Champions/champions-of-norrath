// Observed: OnKeyUp__10VIWndMcErri at 0x0109fa20 is 76 bytes. With (this, key) it calls
// scePrintf("VIWndMcErr::OnKeyUp(KeyCode = %d)\n", key) and then
// ProcessScreen__10VIWndMcErriii(this, 4, key, 0). Nothing is returned.
// Inferred: ProcessScreen receives a screen event number 4, the key and a zero, from the call
// shape alone.
// Unknown: what the numbers 4 and 0 stand for.
extern "C" {
int scePrintf(const char* format, ...);
void ProcessScreen__10VIWndMcErriii(void* self, int a, int b, int c);

void OnKeyUp__10VIWndMcErri(void* self, int key) {
    scePrintf("VIWndMcErr::OnKeyUp(KeyCode = %d)\n", key);
    ProcessScreen__10VIWndMcErriii(self, 4, key, 0);
}
}
