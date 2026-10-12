// Observed: mcProcessKick__Fv at 0x002d0ef8 is 36 bytes (a local symbol in the
// original). It calls mcHeartBeat__Fv, then loads the signed byte
// MC_KickProtect and returns 1 when that byte is zero, otherwise 0.
// Inferred: nothing.
// Unknown: who reads the return value.

extern "C" void mcHeartBeat__Fv();
extern "C" signed char MC_KickProtect;

extern "C" int mcProcessKick__Fv() {
    mcHeartBeat__Fv();
    return MC_KickProtect == 0;
}
