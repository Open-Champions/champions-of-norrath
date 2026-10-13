// Observed sceDeci2Poll at 0x002df8f8, 36 bytes. It stores its argument in a
// one-word stack block and returns Deci2Call(4, block).
// Inferred: the argument is a socket value as returned by sceDeci2Open
// (sceTtyWrite polls with its tinfo socket) and 4 is the poll request.
extern "C" {
int Deci2Call(int request, void* words);

int sceDeci2Poll(int socket) {
    int words[1];
    words[0] = socket;
    return Deci2Call(4, words);
}
}
