// Observed: ProcessScreen__10VIWndMcErriii at 0x0109c7e0 is 668 bytes. With (this, action, param1, param2) it
// first calls scePrintf with the format "VIWndMcErr::ProcessScreen(mScreen = %d, Action = %d, Param1 = %d,
// Param2 = %d)\n" (0x011d2e60) and the word at +412 (mScreen) followed by the three arguments. It then
// dispatches on mScreen through a jump table of 61 entries covering 4..64: the values
// 4, 10, 11, 12, 15, 16, 17, 20, 30, 50, 52, 54, 60, 61, 62, 63 and 64 call ProcessScreenNN__10VIWndMcErriii
// (NN = the value, two digits) with the same four arguments; every other value (including anything outside
// 4..64) calls nothing. It always returns 0.
// Inferred: this is the dispatcher of the memory-card error window's per-screen handlers; the parameter names
// come from the debug format string.
// Unknown: what each screen number is, the return type (an int zero is returned), and the handlers' bodies.
extern "C" {
int scePrintf(const char* format, ...);
void ProcessScreen04__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen10__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen11__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen12__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen15__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen16__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen17__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen20__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen30__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen50__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen52__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen54__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen60__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen61__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen62__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen63__10VIWndMcErriii(void* self, int action, int param1, int param2);
void ProcessScreen64__10VIWndMcErriii(void* self, int action, int param1, int param2);

int ProcessScreen__10VIWndMcErriii(void* self, int action, int param1, int param2) {
    const int screen = static_cast<int*>(self)[103];
    scePrintf("VIWndMcErr::ProcessScreen(mScreen = %d, Action = %d, Param1 = %d, Param2 = %d)\n", screen, action,
              param1, param2);
    switch (screen) {
    case 4:
        ProcessScreen04__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 10:
        ProcessScreen10__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 11:
        ProcessScreen11__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 12:
        ProcessScreen12__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 15:
        ProcessScreen15__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 16:
        ProcessScreen16__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 17:
        ProcessScreen17__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 20:
        ProcessScreen20__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 30:
        ProcessScreen30__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 50:
        ProcessScreen50__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 52:
        ProcessScreen52__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 54:
        ProcessScreen54__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 60:
        ProcessScreen60__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 61:
        ProcessScreen61__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 62:
        ProcessScreen62__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 63:
        ProcessScreen63__10VIWndMcErriii(self, action, param1, param2);
        break;
    case 64:
        ProcessScreen64__10VIWndMcErriii(self, action, param1, param2);
        break;
    default:
        break;
    }
    return 0;
}
}
