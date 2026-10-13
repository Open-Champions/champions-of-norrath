// Observed: SetNozzleHprVar__20VIParticleAttributesRC7VIVect3 is 96 bytes. It copies three
// floats from the argument to offsets 672, 676 and 680 of the attribute block, stores 0 at
// offset 700 when all three compare equal to 0.0f and 1 otherwise (NaN counts as non-zero),
// and adds one to the word at offset 716.
// Inferred: the floats are heading/pitch/roll variance and the word at 700 is an
// "any variance set" flag; the field names come from the symbol only.
// Unknown: what consumes the flag and the counter.
extern "C" {
void SetNozzleHprVar__20VIParticleAttributesRC7VIVect3(unsigned char* attrs, const float* hpr) {
    float* stored = reinterpret_cast<float*>(attrs + 672);
    float x = hpr[0];
    float y = hpr[1];
    float z = hpr[2];
    *reinterpret_cast<unsigned*>(attrs + 700) = (x != 0.0f || y != 0.0f || z != 0.0f) ? 1u : 0u;
    stored[0] = x;
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
    stored[1] = y;
    stored[2] = z;
}
}
