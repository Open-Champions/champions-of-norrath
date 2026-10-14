// Observed msg_save__10DoorSecretPc at 0x21c050, 24 bytes. Writes the byte at
// object+88 to the output pointer and returns the number of bytes written, 1.
// Unknown: the meaning of that byte.
extern "C" int msg_save__10DoorSecretPc(void* self, char* out) {
    *out = static_cast<char>(static_cast<const unsigned char*>(self)[88]);
    return 1;
}
