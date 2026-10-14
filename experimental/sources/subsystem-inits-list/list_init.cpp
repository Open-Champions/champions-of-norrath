// listInit__Fv at 0x00194420, 92 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No list has been walked
// on the EE.
//
// nodes at 0x007ad000 runs 121920 bytes, to oniSuckParticle.1175, which is
// 6096 records of 20 bytes. The image walks from the high end with counter
// 6094 and writes each record's first word as the next record. freeNodes at
// gp-3972 receives nodes. The last record's first word is then stored as zero.
// Bytes after the first word of each record are not written.
extern "C" {
extern unsigned char nodes[];
extern unsigned char* freeNodes;

static void norrath_store_pointer(void* slot, const void* value) {
    const auto bits = static_cast<unsigned>(reinterpret_cast<unsigned long>(value));
    auto* dest = static_cast<unsigned char*>(slot);
    dest[0] = static_cast<unsigned char>(bits);
    dest[1] = static_cast<unsigned char>(bits >> 8);
    dest[2] = static_cast<unsigned char>(bits >> 16);
    dest[3] = static_cast<unsigned char>(bits >> 24);
}

void listInit__Fv() {
    constexpr int kRecords = 6096;
    constexpr int kStride = 20;
    for (int index = 0; index < kRecords - 1; ++index) {
        norrath_store_pointer(nodes + index * kStride, nodes + (index + 1) * kStride);
    }
    norrath_store_pointer(nodes + (kRecords - 1) * kStride, nullptr);
    freeNodes = nodes;
}
}
