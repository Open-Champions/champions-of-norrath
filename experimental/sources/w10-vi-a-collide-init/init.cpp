// Observed: Init__9VICollide at 0x01096df8 is 52 bytes. It calls
// Identity__10VIMatrix44 on the matrices at offsets 16 and 80 of the object, in
// that order, and returns 0.
// Inferred: the two 64-byte regions at offsets 16 and 80 are VIMatrix44
// members, from these calls and the matching destructor calls.
extern "C" {
void Identity__10VIMatrix44(void* matrix);

int Init__9VICollide(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    Identity__10VIMatrix44(bytes + 16);
    Identity__10VIMatrix44(bytes + 80);
    return 0;
}
}
