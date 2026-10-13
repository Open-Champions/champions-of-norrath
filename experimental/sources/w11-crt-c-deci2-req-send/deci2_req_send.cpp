// Observed sceDeci2ReqSend at 0x002df8c8, 48 bytes. It stores its first
// argument and the sign-extended low byte of its second argument in a
// two-word stack block and returns Deci2Call(3, block).
// Inferred: the first argument is a socket as returned by sceDeci2Open and 3
// is the send request; sceTtyWrite passes the signed byte at offset 7 of its
// packet header as the second argument.
extern "C" {
int Deci2Call(int request, void* words);

int sceDeci2ReqSend(int socket, int destination) {
    int words[2];
    words[0] = socket;
    words[1] = static_cast<signed char>(destination);
    return Deci2Call(3, words);
}
}
