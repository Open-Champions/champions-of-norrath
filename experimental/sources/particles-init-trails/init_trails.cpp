// initTrails__Fv at 0x00184248, 112 bytes, read from the pinned SLUS-20565
// image. Fresh compilation is not an original-byte match. No trail has been
// initialized on the EE.
//
// trail at 0x00780b00 is 64 records of 1040 bytes. The halfword at the start
// of each record is stored as zero, last record first. bolt at 0x006f6e50 is
// 20 records of 1844 bytes. The byte at offset 1836 of each record is stored
// as zero, last record first. trailSequenceNm at 0x00484310 then receives 1.
extern "C" {
extern unsigned char trail[];
extern unsigned char bolt[];
extern unsigned trailSequenceNm;

void initTrails__Fv() {
    for (int index = 63; index >= 0; --index) {
        unsigned char* record = trail + index * 1040;
        record[0] = 0;
        record[1] = 0;
    }
    for (int index = 19; index >= 0; --index) {
        bolt[index * 1844 + 1836] = 0;
    }
    trailSequenceNm = 1u;
}
}
