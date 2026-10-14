// Observed: Sound__13VISoundDevicei at 0x01126368, 32 bytes. Arguments: the device and an index. If the index is -1 the result is
// 0. Otherwise the result is the word at device + index * 4 + 2584, with no other bounds check (any other index, negative or
// large, is read as given).
// Inferred: the words from +2584 are a table of sound objects addressed by handle, and -1 is the "no sound" handle.
// Unknown: the table length, and the member name.
extern "C" {
unsigned Sound__13VISoundDevicei(unsigned char* device, int index) {
    if (index == -1) return 0;
    return *reinterpret_cast<unsigned*>(device + index * 4 + 2584);
}
}
