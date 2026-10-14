// Observed: SetName__15VIParticleMotifPCc at 0x010b4b80 is 64 bytes. With (this, name) it calls strcpy(this+724, name), then
// VIHashResourceID(name) and stores the result at this+756. Nothing is returned.
// Inferred: the motif keeps its name text at +724 and the resource hash of that name at +756.
// Unknown: the capacity of the name buffer (no length check exists in this routine) and the return type of the hash.
extern "C" {
char* strcpy(char* dst, const char* src);
unsigned VIHashResourceID(const char* name);

void SetName__15VIParticleMotifPCc(char* self, const char* name) {
    strcpy(self + 724, name);
    *(unsigned*)(self + 756) = VIHashResourceID(name);
}
}
