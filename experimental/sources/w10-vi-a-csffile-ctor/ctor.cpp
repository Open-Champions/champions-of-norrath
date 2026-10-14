// Observed: __9VICSFFile at 0x010a4be0 is 12 bytes and returns nothing. It
// stores zero in the words at offsets 0 and 4.
// Unknown: what those words hold; the size of VICSFFile.
extern "C" {
void __9VICSFFile(void* object) {
    int* words = static_cast<int*>(object);
    words[1] = 0;
    words[0] = 0;
}
}
