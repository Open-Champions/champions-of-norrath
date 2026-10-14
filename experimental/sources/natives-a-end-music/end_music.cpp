// amxEndMusic is the 32-byte native at 0x0013c010. It fades stream 0 and
// returns zero.
extern "C" int soundFadeStream__Fi(int stream);
extern "C" int amxEndMusic__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    soundFadeStream__Fi(0);
    return 0;
}
