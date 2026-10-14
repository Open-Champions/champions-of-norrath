// Observed: SetZWrite__20VIParticleDefinitionb at 0x010b8940 is 64 bytes. With (this, flag) it stores the flag at this+752, then calls
// Material__8VIRasteri(word at this+772, word at this+776) and calls SetZWrite__10VIMateriali(result, flag != 0 (read back from this+752)).
// Inferred: the +772/+776 pair identifies the definition's material; the call forwards the depth-write setting to it.
// Unknown: the width semantics of the stored flag (it is stored as the full word and only normalised for the callee).
extern "C" {
void* Material__8VIRasteri(int a, int b);
void SetZWrite__10VIMateriali(void* material, int flag);

void SetZWrite__20VIParticleDefinitionb(char* self, int flag) {
    *(int*)(self + 752) = flag;
    void* material = Material__8VIRasteri(*(int*)(self + 772), *(int*)(self + 776));
    SetZWrite__10VIMateriali(material, *(int*)(self + 752) != 0);
}
}
