// Observed msg_poke__8JumpGatei at 0x002844f0, 72 bytes.
// Calls DLG_CheckMutex__Fv. When it returns zero, or when it returns nonzero and the signed byte
// netNetworkMode is nonzero, calls jumpGateInit__FiPcc with the incoming value, a null string and
// zero. When it returns nonzero and netNetworkMode is zero, nothing more happens. No result is used.
// Unknown: what the value is and what the mutex guards. Nothing here ran on the EE.
extern "C" {
int DLG_CheckMutex__Fv();
void jumpGateInit__FiPcc(int value, char* name, char flag);
extern signed char netNetworkMode;
}

extern "C" void msg_poke__8JumpGatei(void* self, int value) {
    (void)self;
    if (DLG_CheckMutex__Fv() == 0 || netNetworkMode != 0) jumpGateInit__FiPcc(value, nullptr, 0);
}
