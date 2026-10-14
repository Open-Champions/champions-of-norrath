// Observed: CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44 at 0x0101dc40 is 60 bytes. Members
// are 32-byte records starting at offset 0 of the sprite object. For member i it calls
// Transform__10VIMatrix44RC7VIVect3fT1 with: the matrix argument as the receiver, a pointer to
// member offset 72 (a 3-float vector), the float at member offset 84, and a pointer to member
// offset 88 (the output vector); it then returns 0 whatever Transform returned.
// Inferred: the three member fields at 72/84/88 are an input position, a scalar and a transformed
// result; the member array layout before offset 72 is not known (the base offset is observed).
// Unknown: the meaning of the scalar (passed in a float register) and Transform's own result.
extern "C" {
int Transform__10VIMatrix44RC7VIVect3fT1(void* matrix, const void* in, float scalar, void* out);

int CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44(unsigned char* self, int index, void* matrix) {
    unsigned char* member = self + index * 32;
    Transform__10VIMatrix44RC7VIVect3fT1(matrix, member + 72, *reinterpret_cast<float*>(member + 84),
                                         member + 88);
    return 0;
}
}
