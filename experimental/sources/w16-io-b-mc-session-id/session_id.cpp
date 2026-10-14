// Observed: VISetMcSessionId__FUl at 0x0109fd48 is 48 bytes. It calls
// scePrintf("VISetMcSessionId()\n") and then stores the whole 64-bit argument
// register into the local doubleword sSessionId (0x011e8020) with one sd,
// returning nothing.
// Inferred: the stored value identifies the card session for VIIsMcSameSession.
// Unknown: the intended width (the mangled argument is a 32-bit unsigned long,
// the store is 64 bits) and any reader of sSessionId.
extern "C" int scePrintf(const char* format, ...);

extern "C" {
long long sSessionId;

void VISetMcSessionId__FUl(unsigned long id) {
    scePrintf("VISetMcSessionId()\n");
    sSessionId = (long)id;
}
}
