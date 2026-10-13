// Observed sceCdStStart at 0x002e4110, 52 bytes. It stores 1 in stm_status
// (0x00447c34), then returns sceCdStream(lsn, 0, 0, 1, mode) with the first
// argument passed through and the second argument becoming the mode pointer
// (the last, fifth stream argument). Callers pass the address of the CDmode
// object.
// Inferred: command 1 starts the stream; the meaning of the first argument
// beyond its use as the CD position by callers is not established here.
extern "C" {
extern int stm_status;
int sceCdStream(int argument, int count, void* buffer, int command, void* mode);

int sceCdStStart(int sector, void* mode) {
    stm_status = 1;
    return sceCdStream(sector, 0, 0, 1, mode);
}
}
