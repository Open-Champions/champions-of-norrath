// initParticleSystems__Fv at 0x00180228, 56 bytes, read from the pinned
// SLUS-20565 image. Fresh compilation is not an original-byte match. No
// particle system has been initialized on the EE.
//
// PS at 0x006f5ad0 is 16 records of 312 bytes, ending at bolt. The word at
// offset 304 of each record is stored as zero, last record first. psSequenceNm
// at 0x00484308 then receives 1.
extern "C" {
extern unsigned char PS[];
extern unsigned psSequenceNm;

static void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

void initParticleSystems__Fv() {
    for (int index = 15; index >= 0; --index) {
        write_word(PS + index * 312 + 304, 0u);
    }
    psSequenceNm = 1u;
}
}
