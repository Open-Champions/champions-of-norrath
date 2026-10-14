// Observed: StopBgm__13VISoundDevicei at 0x011264f0 is 112 bytes. Argument: slot index (a1).
// With name = device + (slot << 8) + 3436: if the first byte of name (signed) is non-zero
// and strcmp(device + 3948, name) == 0, it stores -1 at device + 4460. In every case it then
// stores a zero byte at name[0]. The original leaves a pointer to name in v0; the return
// value is not meaningful and the function is authored as void.
// Inferred: 3436 + 256 * slot are the stored background-music name buffers written by
// PlayBgm__13VISoundDeviceiPCc; +3948 (slot 2) is compared as the currently active name,
// and +4460 is a word that is reset to -1 when the stopped name is the active one.
// Unknown: which slot +3948 really is for the caller, and what the word at +4460 means.
extern "C" {
int strcmp(const char* a, const char* b);

void StopBgm__13VISoundDevicei(unsigned char* device, int slot) {
    char* name = reinterpret_cast<char*>(device + (slot << 8) + 3436);
    if (*name != 0 && strcmp(reinterpret_cast<const char*>(device + 3948), name) == 0) {
        *reinterpret_cast<int*>(device + 4460) = -1;
    }
    *name = 0;
}
}
