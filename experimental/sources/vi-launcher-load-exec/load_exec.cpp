// Observed LoadExecPS2 at 0x002ddca0, 72 bytes. It returns through
// _LoadExecPS2. SetArg receives the three incoming arguments. Its result
// is the first _LoadExecPS2 argument, the original count is the second,
// and the word _kExecArg at 0x00445d48 plus 4 is the third. init_tlb
// writes that word. This file does not define it. TerminateLibrary runs
// between SetArg and _LoadExecPS2.
extern "C" {
unsigned SetArg(const char* path, int argc, char** argv);
void TerminateLibrary();
extern unsigned _kExecArg;
void _LoadExecPS2(unsigned filename, int argc, unsigned exec_arg);

void LoadExecPS2(const char* path, int argc, char** argv) {
    unsigned filename = SetArg(path, argc, argv);
    TerminateLibrary();
    _LoadExecPS2(filename, argc, _kExecArg + 4);
}
}
