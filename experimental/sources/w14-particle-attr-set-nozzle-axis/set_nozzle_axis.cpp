// Observed: SetNozzleAxis__20VIParticleAttributesRC7VIVect3 is 80 bytes. It copies three
// floats (x, y, z) from the argument to offsets 660, 664 and 668 of the attribute block,
// calls Normalize__7VIVect3 on the block address + 660, then adds one to the word at
// offset 716.
// Inferred: the three floats are a nozzle axis direction that is stored normalized.
// Unknown: Normalize__7VIVect3's behavior for a zero vector, and what consumes the counter.
extern "C" {
void Normalize__7VIVect3(void* vector);

void SetNozzleAxis__20VIParticleAttributesRC7VIVect3(unsigned char* attrs, const float* axis) {
    float* stored = reinterpret_cast<float*>(attrs + 660);
    stored[0] = axis[0];
    stored[1] = axis[1];
    stored[2] = axis[2];
    Normalize__7VIVect3(stored);
    *reinterpret_cast<unsigned*>(attrs + 716) += 1;
}
}
