// Observed getPicSize__4itemiPQ24item11descriptionPiT3 at 0x00108428, 112
// bytes. Arguments: an int, a description pointer, and two int pointers.
// A 64-byte name buffer in the frame is filled by
// getPicName__4itemPciPQ24item11description(buffer, int, description), then
// picFind__4itemPc(buffer) returns a record pointer. With a null result both
// output ints are set to 0 (first, then second). Otherwise the first output
// receives the signed halfword at record+6 and the second the signed halfword at
// record+8; the halfword at +8 is loaded before the +6 one.
// Unknown: what the record is beyond these two fields; they are presumably the
// picture width and height, which is not shown by this routine.
extern "C" {
void getPicName__4itemPciPQ24item11description(char* buffer, int a, void* description);
unsigned char* picFind__4itemPc(char* name);
}

extern "C" void getPicSize__4itemiPQ24item11descriptionPiT3(int a, void* description, int* first, int* second) {
    char name[64];
    getPicName__4itemPciPQ24item11description(name, a, description);
    const unsigned char* record = picFind__4itemPc(name);
    if (record == 0) {
        *first = 0;
        *second = 0;
    } else {
        *first = *reinterpret_cast<const short*>(record + 6);
        *second = *reinterpret_cast<const short*>(record + 8);
    }
}
