// Observed: initStuff__6PhPropi at 0x00148738 is 644 bytes with an 80-byte
// frame. Arguments are (self, flags); offsets below are bytes into self.
//  1. If the signed byte at 568 is nonzero and the 64-bit word at 336 has bit
//     0x20000 set, flags gets bit 0x1000.
//  2. Calls modelGetBoundingBox__FP12_modelHeaderlPiN52(word at 76, the 64-bit
//     value at 232, &box[0] .. &box[5]) for six int outputs, then scales each
//     as a float by the float at 576.
//  3. Calls bodyInit__FP5_bodyfffff(self + 400, 1.0, 1.0, s3 - s0, s4 - s1,
//     s5 - s2), where s_i is box[i] converted to float and multiplied by the
//     scale.
//  4. Copies 12 bytes from self + 16 to self + 416.
//  5. Stores float(self[252]) * k, float(self[256]) * k, float(self[48]) * k at
//     444, 448, 452 where k = 0.016666666 (the float at 0x0047fd5c).
//  6. Picks a range r = 2.0, or 10.0 if (flags & 0x10) or (flags & 0x400).
//  7. Advances the linear congruential generator in the word at 68 three steps
//     (state = state * 1664525 + 1013904223), storing the final state back, and
//     stores at 456, 460, 464 the values (2 * (step_i >> 16) / 65536 - 1) * r
//     for steps 1, 2, 3. Also stores -1 in the word at 556.
//  8. If the 64-bit value at 232 is nonzero, stores in the word at 556 the index
//     of its highest set bit.
//  9. Calls matToQuaternion__FP4QuatP8Matrix34(self + 428, self + 272),
//     quatNormalize__FP4Quat(self + 428) and bodyUpdateMatrix__FP5_body(self +
//     400).
// 10. Stores -0.1 (the float at 0x0047fd60) at 572 if (flags & 0x800), else -0.4
//     (0x0047fd64).
// 11. If (flags & 0x1000), zeroes the words at 404, 408 and 412.
// Inferred: the object is a physics prop, from the class name PhProp in the
// symbol; the body at +400 and its quaternion at +428 are inferred from the
// callee argument types; 1.0/60 for k reads as a frame period only by value.
// Unknown: the meaning of flags 0x10, 0x400, 0x800 and 0x1000, of the bytes at
// 568, 336 and 232, and of the words at 556 and 572; the layout of the prop.

extern "C" void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* model, long long what,
                                                            int* a, int* b, int* c, int* d,
                                                            int* e, int* f);
extern "C" void bodyInit__FP5_bodyfffff(void* body, float a, float b, float c, float d, float e);
extern "C" void matToQuaternion__FP4QuatP8Matrix34(void* quat, void* matrix);
extern "C" void quatNormalize__FP4Quat(void* quat);
extern "C" void bodyUpdateMatrix__FP5_body(void* body);

template <typename T>
static inline T& at(unsigned char* base, int offset) {
    return *reinterpret_cast<T*>(base + offset);
}

extern "C" void initStuff__6PhPropi(void* object, int flags) {
    unsigned char* self = static_cast<unsigned char*>(object);
    if (at<signed char>(self, 568) != 0) {
        if ((at<long long>(self, 336) & 0x20000) != 0) flags |= 0x1000;
    }
    int box[6];
    modelGetBoundingBox__FP12_modelHeaderlPiN52(at<void*>(self, 76), at<long long>(self, 232),
                                                &box[0], &box[1], &box[2], &box[3], &box[4],
                                                &box[5]);
    float scale = at<float>(self, 576);
    float s0 = static_cast<float>(box[0]) * scale;
    float s1 = static_cast<float>(box[1]) * scale;
    float s2 = static_cast<float>(box[2]) * scale;
    float s3 = static_cast<float>(box[3]) * scale;
    float s4 = static_cast<float>(box[4]) * scale;
    float s5 = static_cast<float>(box[5]) * scale;
    bodyInit__FP5_bodyfffff(self + 400, 1.0f, 1.0f, s3 - s0, s4 - s1, s5 - s2);
    __builtin_memcpy(self + 416, self + 16, 12);
    const float k = 0x1.111110p-6f;
    at<float>(self, 444) = at<float>(self, 252) * k;
    at<float>(self, 448) = at<float>(self, 256) * k;
    at<float>(self, 452) = at<float>(self, 48) * k;
    float range = 2.0f;
    if ((flags & 0x10) != 0) range = 10.0f;
    if ((flags & 0x400) != 0) range = 10.0f;
    unsigned state = at<unsigned>(self, 68);
    at<int>(self, 556) = -1;
    unsigned long long mask = static_cast<unsigned long long>(at<long long>(self, 232));
    unsigned r1 = state * 1664525u + 1013904223u;
    unsigned r2 = r1 * 1664525u + 1013904223u;
    unsigned r3 = r2 * 1664525u + 1013904223u;
    const float unit = 1.52587890625e-05f;
    float f1 = static_cast<float>(r1 >> 16) * unit;
    float f2 = static_cast<float>(r2 >> 16) * unit;
    float f3 = static_cast<float>(r3 >> 16) * unit;
    at<unsigned>(self, 68) = r3;
    at<float>(self, 456) = (f1 + f1 - 1.0f) * range;
    at<float>(self, 460) = (f2 + f2 - 1.0f) * range;
    at<float>(self, 464) = (f3 + f3 - 1.0f) * range;
    if (mask != 0) {
        int index = -1;
        do {
            mask >>= 1;
            ++index;
        } while (mask != 0);
        at<int>(self, 556) = index;
    }
    matToQuaternion__FP4QuatP8Matrix34(self + 428, self + 272);
    quatNormalize__FP4Quat(self + 428);
    bodyUpdateMatrix__FP5_body(self + 400);
    if ((flags & 0x800) != 0) at<float>(self, 572) = -0x1.999998p-4f;
    else at<float>(self, 572) = -0x1.999998p-2f;
    if ((flags & 0x1000) != 0) {
        at<int>(self, 412) = 0;
        at<int>(self, 404) = 0;
        at<int>(self, 408) = 0;
    }
}
