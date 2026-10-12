// Observed superGamestateInit at 0x00486dc0. Eight bytes in .bss, so zero at
// load. The authored game loop uses it as a pair: a name pointer and a state
// word, and passes a nonzero name with the state to scriptSetGameState before
// clearing it. The layout here is that pair; the image does not record a type.
struct SuperGamestateInit {
    const char* name;
    int state;
};
extern "C" {
SuperGamestateInit superGamestateInit;
}
