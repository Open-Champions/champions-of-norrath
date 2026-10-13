// Observed P_OpenParticleSystem__Fi at 0x00180338, extent 68. For a nonzero id,
// the PS record selected by the low byte of the id (PS at 0x006f5ad0, stride 312)
// becomes the current system (pointer word currentPS, 0x0048430c) only if the
// word at +304 of that record equals the whole id. A zero id, or a mismatch,
// changes nothing. Returns nothing.
// Inferred: the +304 word is the id the record was created under, so a stale id
// whose low byte collides with a live record is refused. Unknown: how ids are
// allocated.
extern "C" {
extern unsigned char PS[];
extern unsigned char* currentPS;
void P_OpenParticleSystem__Fi(int id) {
    if (id != 0) {
        unsigned char* record = PS + (static_cast<unsigned>(id) & 0xffu) * 312u;
        if (*reinterpret_cast<int*>(record + 304) == id) currentPS = record;
    }
}
}
