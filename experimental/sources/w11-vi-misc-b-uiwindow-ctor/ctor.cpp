// Observed: __10VIUIWindow at 0x0103ef08 is 108 bytes. With the object as this it stores zero in
// the word pairs at this+4, this+12 and this+20 (eight bytes each), zero floats in three 16-byte
// groups starting at this+304 (twelve floats, +304 to +351), eight zero bytes at this+352, zero
// words at this+36, this+40 and this+44, and a zero byte at this+48. Nothing is returned and no
// other bytes are written.
// Inferred: the three float groups are one array of three 4-float values. Unknown: what any of
// the fields mean, and the size of the class.
extern "C" {
void __10VIUIWindow(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned* pair = reinterpret_cast<unsigned*>(bytes + 4);
    pair[0] = 0;
    pair[1] = 0;
    pair = reinterpret_cast<unsigned*>(bytes + 12);
    pair[0] = 0;
    pair[1] = 0;
    pair = reinterpret_cast<unsigned*>(bytes + 20);
    pair[0] = 0;
    pair[1] = 0;
    float* group = reinterpret_cast<float*>(bytes + 304);
    for (int i = 0; i < 3; ++i) {
        group[0] = 0.0f;
        group[1] = 0.0f;
        group[2] = 0.0f;
        group[3] = 0.0f;
        group += 4;
    }
    *reinterpret_cast<unsigned long long*>(bytes + 352) = 0;
    *reinterpret_cast<unsigned*>(bytes + 36) = 0;
    *reinterpret_cast<unsigned*>(bytes + 40) = 0;
    *reinterpret_cast<unsigned*>(bytes + 44) = 0;
    bytes[48] = 0;
}
}
