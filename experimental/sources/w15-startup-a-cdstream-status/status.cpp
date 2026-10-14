// Observed: VICdStreamStatus__Fv at 0x01000b58 is 12 bytes. It returns the word at
// 0x011e2644, a file-local word of the CD-stream file (a local named sStatus in the
// symbol table; other local words carry the same name, so it is imported here as
// norrath_cdstream_status). The other three CD-stream functions write 0, 1, 2 or the
// first word of the reply block into it.
// Inferred: it is the last result code of the stream request interface; CDSync tests it
// for nonzero as an error.
// Unknown: the meaning of the individual values 1 and 2 beyond "request not sent" and
// "request still pending".
extern "C" {
extern int norrath_cdstream_status;

int VICdStreamStatus__Fv() {
    return norrath_cdstream_status;
}
}
