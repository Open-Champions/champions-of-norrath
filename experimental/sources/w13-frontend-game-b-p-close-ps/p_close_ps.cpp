// Observed P_CloseParticleSystem__Fi at 0x00180380, extent 64. For a nonzero id
// whose PS record (0x006f5ad0, stride 312, selected by the low byte of the id)
// holds that same id at +304, currentPS (0x0048430c) is set back to the
// globalSystem table (0x0037f730). Any other case changes nothing. Returns
// nothing. Note it resets currentPS only when the id matches; it does not check
// that currentPS pointed at that record.
extern "C" {
extern unsigned char PS[];
extern unsigned char globalSystem[];
extern unsigned char* currentPS;
void P_CloseParticleSystem__Fi(int id) {
    if (id != 0) {
        const unsigned char* record = PS + (static_cast<unsigned>(id) & 0xffu) * 312u;
        if (*reinterpret_cast<const int*>(record + 304) == id) currentPS = globalSystem;
    }
}
}
