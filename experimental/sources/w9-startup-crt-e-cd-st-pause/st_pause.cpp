// Observed sceCdStPause at 0x002e4368, 80 bytes. The store of zero to stm_status
// (0x00447c34) sits in the delay slot of the debug branch, so it happens whether or not
// SCE_CD_debug (0x00446090) is above zero. When it is, the string at 0x0047bd60 is
// printed through scePrintf. The result is sceCdStream(0, 0, 0, 7, dum_mode).
// The cd-archive package carries the same body under its own norrath_ name; this is the
// exported symbol.
extern "C" {
extern int SCE_CD_debug;
extern int stm_status;
extern unsigned char dum_mode[];
int scePrintf(const char* format, ...);
unsigned sceCdStream(unsigned argument, unsigned count, void* buffer, unsigned command, const unsigned char* mode);

unsigned sceCdStPause() {
    stm_status = 0;
    if (SCE_CD_debug > 0) scePrintf("sceCdStPause call\n");
    return sceCdStream(0u, 0u, nullptr, 7u, dum_mode);
}
}
