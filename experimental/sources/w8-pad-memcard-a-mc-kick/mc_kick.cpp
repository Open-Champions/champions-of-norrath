// Observed: MC_Kick__Fv at 0x0025b560 is 72 bytes. When the signed byte
// MC_KickProtect is zero it stores zero in ranConfig and ranBrowser, calls
// newMode__Fii(-1, 0), then calls MCA_Busy__Fv. A nonzero result calls
// resetWithClose__Fv. A nonzero MC_KickProtect does nothing.
// Inferred: nothing.
// Unknown: what MCA_Busy__Fv reports beyond zero versus nonzero.

extern "C" int newMode__Fii(int mode, int keep);
extern "C" int MCA_Busy__Fv();
extern "C" void resetWithClose__Fv();
extern "C" signed char MC_KickProtect;
extern "C" int ranConfig;
extern "C" int ranBrowser;

extern "C" void MC_Kick__Fv() {
    if (MC_KickProtect == 0) {
        ranConfig = 0;
        ranBrowser = 0;
        newMode__Fii(-1, 0);
        if (MCA_Busy__Fv() != 0) {
            resetWithClose__Fv();
        }
    }
}
