// Observed objectFindObject__FPc at 0x143a58, extent 40. Calls the three
// argument finder with capacity 2 and a stack word, and returns that word:
// the first match, or zero.
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** output, int capacity);

extern "C" void* objectFindObject__FPc(char* name) {
    void* found[2];
    objectFindObject__FPcPP10GameObjecti(name, found, 2);
    return found[0];
}
