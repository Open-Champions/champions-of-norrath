// Host contract for engineReboot. Not an EE plan input.
#include <csetjmp>
#include <cstring>
#include "engine_reboot.cpp"

static std::jmp_buf escape;
static char order[32];
static int count = 0;
static void note(char c) { order[count++] = c; }
static int netArg = -1, disableArg = -1, removeCause = -1, removeHandler = -1;
static int loadArgc = -1;
static char* loadArgv = reinterpret_cast<char*>(1);
static char loadPath[64];

extern "C" {
int vblankHID = 0x1234;
void soundQuiet__Fv() { note('a'); }
void netFree__Fb(bool flag) { note('b'); netArg = flag; }
void DisableIntc(int cause) { note('c'); disableArg = cause; }
void RemoveIntcHandler(int cause, int handler) { note('d'); removeCause = cause; removeHandler = handler; }
void drawRemoveInterrupts__Fv() { note('e'); }
void texTerminateThread__Fv() { note('f'); }
void scePadEnd() { note('g'); }
void UnpreparePowerOff__Fv() { note('h'); }
void sceSifExitCmd() { note('i'); }
void LoadExecPS2(const char* path, int argc, char** argv) {
    note('j');
    std::strncpy(loadPath, path, sizeof loadPath - 1);
    loadArgc = argc;
    loadArgv = reinterpret_cast<char*>(argv);
    std::longjmp(escape, 1);  // stands in for the endless wait after a failed load
}
}

int main() {
    if (setjmp(escape) == 0) {
        engineReboot__Fv();
        return 1;  // must not return
    }
    order[count] = 0;
    if (std::strcmp(order, "abcdefghij") != 0) return 2;
    if (netArg != 1 || disableArg != 2 || removeCause != 2 || removeHandler != 0x1234) return 3;
    if (std::strcmp(loadPath, "cdrom0:\\SLUS_205.65;1") != 0) return 4;
    if (loadArgc != 0 || loadArgv != nullptr) return 5;
    return 0;
}
