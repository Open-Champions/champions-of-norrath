// Observed: SetInnerHprVar__20VIParticleAttributesRC7VIVect3 at 0x010b4868 is 96 bytes. With (this, v) where v points at three floats it stores 1 at this+688 when any of the three
// floats compares unequal to zero (a NaN counts as nonzero) and 0 when all three equal zero, copies the three floats to this+624,
// +628, +632, and increments the word at this+716 by one. Nothing is returned. The flag is stored before the copy.
// Inferred: this+624 is a three-float attribute and this+688 records whether it is nonzero; this+716 is a modification counter.
// Unknown: the meaning of the counter's consumers and the names of the vector and flag.
extern "C" {
void SetInnerHprVar__20VIParticleAttributesRC7VIVect3(char* self, const float* v) {
    *(int*)(self + 688) = (v[0] != 0.0f || v[1] != 0.0f || v[2] != 0.0f);
    float* dst = (float*)(self + 624);
    dst[0] = v[0];
    *(int*)(self + 716) += 1;
    dst[1] = v[1];
    dst[2] = v[2];
}
}
