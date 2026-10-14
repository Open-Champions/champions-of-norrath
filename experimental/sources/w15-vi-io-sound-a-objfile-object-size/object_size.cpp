// Observed: ObjectSize__9VIObjFile at 0x010a6628, 32 bytes. With the object file as this, the signed word at +232 selects an
// entry. If it is negative the result is 0. Otherwise the result is the word at this + index * 32 + 244.
// Inferred: +232 is the index of the currently open object and the word is that object's size in its 32-byte directory record
// (NumSubObjects reads +248 and ObjectVersion reads +242 of the same record).
// Unknown: member names, units of the size, and the maximum index.
extern "C" {
unsigned ObjectSize__9VIObjFile(unsigned char* object) {
    int index = *reinterpret_cast<int*>(object + 232);
    if (index < 0) return 0;
    return *reinterpret_cast<unsigned*>(object + index * 32 + 244);
}
}
