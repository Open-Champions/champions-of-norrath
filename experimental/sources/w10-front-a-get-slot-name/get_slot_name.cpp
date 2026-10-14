// Observed: getSlotName__FiiPc at 0x0025be78 is 196 bytes. It fills and returns the function-local
// 4-byte static name buffer (symbol name.1287 at 0x00486c30), from port, slot and slotMax:
//  1. when port is -1 and slot is -1, port and slot are taken from MCA_Port and MCA_Slot;
//  2. when port is still -1, or slot is -1, name[0] becomes 0 and the buffer is returned;
//  3. the slot count is slotMax[port] when slotMax is not null, else scePadGetSlotMax(port);
//  4. with a count of 2 or more the text is sprintf(name, "%d-%c", port + 1, slot + 65), for
//     example "1-A", otherwise sprintf(name, "%d", port + 1).
// The empty case reads its zero byte from an empty string literal in the image.
// Inferred: the text names a memory-card socket, with a letter for the multitap slot.
// Unknown: the exact element type of slotMax (read as a signed byte).

extern "C" {
extern int MCA_Port;
extern int MCA_Slot;
int scePadGetSlotMax(int port);
int sprintf(char* out, const char* format, ...);
}

extern "C" char* getSlotName__FiiPc(int port, int slot, char* slotMax) {
    static char name[4];
    if (port == -1 && slot == port) {
        port = MCA_Port;
        slot = MCA_Slot;
    }
    if (port == -1 || slot == -1) {
        name[0] = '\0';
        return name;
    }
    int count = slotMax ? slotMax[port] : scePadGetSlotMax(port);
    if (count >= 2) sprintf(name, "%d-%c", port + 1, slot + 65);
    else sprintf(name, "%d", port + 1);
    return name;
}
