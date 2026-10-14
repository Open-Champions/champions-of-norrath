// Observed cdInit__Fv at 0x00164e58, 164 bytes. This is not an existing
// authored body. CDmode, isDual, and dualLayer2Start are the views already
// defined in this package. The search name is the directory-name view, and
// the directory passed to loadDirectory is directory A.
extern "C" {
extern unsigned char CDmode[4];
extern unsigned char isDual[4];
extern unsigned char dualLayer2Start[4];
extern unsigned char norrath_archive_directory_a_view[];
extern const unsigned char norrath_archive_directory_name_view[];
int sceCdReadDvdDualInfo(unsigned char* dual, unsigned char* layer_start);
int sceCdSearchFile(unsigned char* file, const unsigned char* name);
void loadDirectory__FP9directoryii(unsigned char* directory, unsigned sector, unsigned blocks);
void* sceSifAllocIopHeap(unsigned bytes);
void sceCdStInit(unsigned bufmax, unsigned bankmax, unsigned iop_address);
static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
void cdInit__Fv() {
    CDmode[2] = 0;
    CDmode[0] = static_cast<unsigned char>(0xc8u);
    CDmode[1] = 0;
    sceCdReadDvdDualInfo(isDual, dualLayer2Start);
    if (load_word(isDual) != 0u) CDmode[1] = 0;
    unsigned char file[32];
    if (sceCdSearchFile(file, norrath_archive_directory_name_view) != 0) {
        const unsigned sector = load_word(file);
        const unsigned blocks = (load_word(file + 4) + 2047u) >> 11;
        loadDirectory__FP9directoryii(norrath_archive_directory_a_view, sector, blocks);
    }
    const unsigned allocated = static_cast<unsigned>(
        reinterpret_cast<__UINTPTR_TYPE__>(sceSifAllocIopHeap(0x14800u)));
    const unsigned aligned = (allocated + 2047u) & 0xfffff800u;
    sceCdStInit(40u, 3u, aligned);
}
}
