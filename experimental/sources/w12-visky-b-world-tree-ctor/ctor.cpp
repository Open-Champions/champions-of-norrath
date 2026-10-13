// Observed: __11VIWorldTree at 0x01100190 is 52 bytes. With the object as this it calls
// __t7VIArray1Z11VIWorldNode(this), then __t7VIArray1Z11VIWorldLeaf(this + 8), then stores 1 in
// the word at this+16. Nothing is returned.
// Inferred: the tree holds an array of nodes at +0 and an array of leaves at +8 (the destructor
// destroys them in the opposite order), and the word at +16 starts at 1.
// Unknown: what the word at +16 counts or flags.
extern "C" {
void __t7VIArray1Z11VIWorldNode(void* array);
void __t7VIArray1Z11VIWorldLeaf(void* array);

void __11VIWorldTree(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    __t7VIArray1Z11VIWorldNode(self);
    __t7VIArray1Z11VIWorldLeaf(bytes + 8);
    *reinterpret_cast<int*>(bytes + 16) = 1;
}
}
