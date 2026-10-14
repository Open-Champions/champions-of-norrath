// Observed: SetZTest__20VIParticleDefinitionb at 0x010b8980 is 64 bytes. With (this, flag) it stores the flag at this+756, then calls
// Material__8VIRasteri(word at this+772, word at this+776) and calls SetZTest__10VIMateriali(result, flag != 0 (read back from this+756)).
// Inferred: same shape as SetZWrite at 0x010b8940, forwarding the depth-test setting to the definition's material.
// Unknown: the width semantics of the stored flag.
extern "C" {
void* Material__8VIRasteri(int a, int b);
void SetZTest__10VIMateriali(void* material, int flag);

void SetZTest__20VIParticleDefinitionb(char* self, int flag) {
    *(int*)(self + 756) = flag;
    void* material = Material__8VIRasteri(*(int*)(self + 772), *(int*)(self + 776));
    SetZTest__10VIMateriali(material, *(int*)(self + 756) != 0);
}
}
