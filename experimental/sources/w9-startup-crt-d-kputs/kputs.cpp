// Observed kputs at 0x002dfa68, 36 bytes. It stores its pointer argument in a
// one-word local, then calls Deci2Call (0x002d5510) with the value 16 and the
// address of that local. Nothing writes the result register afterwards, so the
// Deci2Call result is what the caller sees; callers in the image ignore it.
// Unknown: what service 16 does, and the contract of Deci2Call.
extern "C" {
int Deci2Call(int service, void* arguments);

int kputs(const char* text) {
    const char* argument = text;
    return Deci2Call(16, &argument);
}
}
