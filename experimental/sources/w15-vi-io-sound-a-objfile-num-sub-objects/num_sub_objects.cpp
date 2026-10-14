// Observed: NumSubObjects__9VIObjFile at 0x010a65c8, 32 bytes. With the object file as this, the signed word at +232 selects an
// entry. If it is negative the result is -1. Otherwise the result is the word at this + index * 32 + 248.
// Inferred: +232 is the index of the currently open object and the entries are 32-byte directory records starting at this + 216, so
// the +248 field is the record's sub-object count (the same record that ObjectVersion and ObjectSize read at +242 and +244).
// Unknown: member names, and the maximum index.
extern "C" {
int NumSubObjects__9VIObjFile(unsigned char* object) {
    int index = *reinterpret_cast<int*>(object + 232);
    if (index < 0) return -1;
    return *reinterpret_cast<int*>(object + index * 32 + 248);
}
}
