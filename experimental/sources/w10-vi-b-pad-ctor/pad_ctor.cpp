// Observed __5VIPad at 0x010ff0a0, 16 bytes. It clears the words at offsets
// 4, 52 and 0 of the object and returns. The VIPad size and the meaning of
// those three words are not established here.
extern "C" {
struct VIPad;
void __5VIPad(VIPad* object) {
    unsigned* words = reinterpret_cast<unsigned*>(object);
    words[1] = 0;
    words[13] = 0;
    words[0] = 0;
}
}
