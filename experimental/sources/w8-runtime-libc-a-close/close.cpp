// Observed close at 0x002d57c8, 8 bytes: it returns -1 and nothing else. The
// descriptor is ignored and errno is not written, so the reentrant wrapper
// norrath_factory_close_reentrant only stores errno when something else leaves
// a nonzero value there. No file-system service is reached from this body.
extern "C" int close(int) {
    return -1;
}
