// P_CreateParticleSystem__Fv at 0x00180260, 212 bytes, read from the pinned
// SLUS-20565 image.
// Observed: the function scans the 16 PS records (312 bytes each) for the first
// whose word at +304 is zero. With none free it returns 0. Otherwise it stores
// (psSequenceNm << 8) | index in that word, increments psSequenceNm, makes the
// 48 bytes at record+256 an identity matrix, clears the word at +308 and
// returns the stored id. Fresh compilation is not an original-byte match.
extern "C" {
extern unsigned char PS[];
extern int psSequenceNm;
void matIdent__FP8Matrix34(void* matrix);
}

namespace {
struct Record {
    unsigned char groups[256];
    unsigned char matrix[48];
    int id;
    int spare;
};
static_assert(sizeof(Record) == 312, "PS record size");
}

extern "C" int P_CreateParticleSystem__Fv() {
    Record* records = reinterpret_cast<Record*>(PS);
    int i = 0;
    while (i < 16 && records[i].id != 0) ++i;
    if (i == 16) return 0;
    Record& r = records[i];
    r.id = (psSequenceNm << 8) | i;
    ++psSequenceNm;
    matIdent__FP8Matrix34(r.matrix);
    r.spare = 0;
    return r.id;
}
