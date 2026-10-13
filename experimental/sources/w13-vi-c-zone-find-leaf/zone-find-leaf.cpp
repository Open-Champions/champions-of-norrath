// Observed: FindLeaf__C6VIZoneRC7VIVect3 at 0x01116958, 192 bytes. With the zone as this
// and a point (x, y, z): if the word at +68 is zero the result is zero. Otherwise the
// array of 24-byte nodes is the pointer at +72, and the walk starts at node index zero.
// A node holds four floats (a plane: p0, p1, p2, p3) at +0 and two link words at +16 and
// +20. The plane value is ((x*p0 + y*p1) + z*p2) + p3; if 0 <= value the next link is the
// word at +16, otherwise the one at +20. If bit 30 (0x40000000) of the link is set, the
// result is the link masked with 0x0fffffff; otherwise the link is the next node index.
// The plane floats are addressed by the unmasked index, and the links by the index masked
// with 0x0fffffff, exactly as the image does; they differ only for indices with bits 28, 29
// or 31 set, which valid trees do not contain.
// Inferred: a binary space partition whose leaves are tagged with bit 30. Unknown: member
// names, and what the word at +68 counts.
extern "C" {
unsigned FindLeaf__C6VIZoneRC7VIVect3(const void* zone, const float* point) {
    const unsigned char* bytes = static_cast<const unsigned char*>(zone);
    if (*reinterpret_cast<const int*>(bytes + 68) == 0) return 0;
    const unsigned char* nodes = *reinterpret_cast<const unsigned char* const*>(bytes + 72);
    float x = point[0], y = point[1], z = point[2];
    unsigned index = 0;
    for (;;) {
        const float* plane = reinterpret_cast<const float*>(nodes + index * 24);
        const unsigned char* node = nodes + (index & 0x0fffffffu) * 24;
        float value = x * plane[0] + y * plane[1] + z * plane[2] + plane[3];
        index = (0.0f <= value) ? *reinterpret_cast<const unsigned*>(node + 16)
                                : *reinterpret_cast<const unsigned*>(node + 20);
        if ((index & 0x40000000u) != 0) return index & 0x0fffffffu;
    }
}
}
