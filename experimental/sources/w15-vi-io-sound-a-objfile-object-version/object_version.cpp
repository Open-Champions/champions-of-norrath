// Observed: ObjectVersion__9VIObjFile at 0x010a65e8, 32 bytes. With the object file as this, the signed word at +232 selects an
// entry. If it is negative the result is 0. Otherwise the result is the unsigned halfword at this + index * 32 + 242.
// Inferred: +232 is the index of the currently open object; the halfword is that object's version number as stored in its
// 32-byte directory record (NumSubObjects reads +248 and ObjectSize reads +244 of the same record).
// Unknown: member names, and the maximum index.
extern "C" {
unsigned ObjectVersion__9VIObjFile(unsigned char* object) {
    int index = *reinterpret_cast<int*>(object + 232);
    if (index < 0) return 0;
    return *reinterpret_cast<unsigned short*>(object + index * 32 + 242);
}
}
