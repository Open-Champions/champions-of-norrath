// Observed: ReadString__9VIObjFilePcUs at 0x010a75d0, 108 bytes. Arguments: the object file (this), a destination and a limit
// (low halfword used). It calls ReadStringLength__9VIObjFileRUs(this, &length) with a halfword on the stack; a negative result
// gives -1. Otherwise it calls ReadStringBody__9VIObjFilePcUsUs(this, destination, length, limit) and returns 0 when that result
// is not negative and -1 when it is.
// Inferred: strings are stored as a length followed by the body; the body reader is told both the stored length and the room
// available.
// The image clears the upper half of the limit argument on entry (it was compiled to widen a halfword argument itself), so the
// parameter is declared as a full word here and truncated before the call.
// Unknown: the member names, and the contract of the two callees beyond their sign.
extern "C" {
int ReadStringLength__9VIObjFileRUs(unsigned char* file, unsigned short* length);
int ReadStringBody__9VIObjFilePcUsUs(unsigned char* file, char* destination, unsigned short length, unsigned short limit);

int ReadString__9VIObjFilePcUs(unsigned char* file, char* destination, unsigned limit) {
    unsigned short length;
    if (ReadStringLength__9VIObjFileRUs(file, &length) < 0) return -1;
    return ReadStringBody__9VIObjFilePcUsUs(file, destination, length, static_cast<unsigned short>(limit)) < 0 ? -1 : 0;
}
}
