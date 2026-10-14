// Observed: __t6VIList1ZP10VIFlatFile at 0x011f3b60, 16 bytes. With (this) it stores zero at this+8, this+0 and this+4 and returns
// nothing.
// Inferred: the default constructor of the list of flat-file pointers. +0 is the element count and +4 and +8 the first and last node
// links, matching the other VIList instantiations whose Erase and Clear routines read them.
// Unknown: node ownership and whether the original constructor set any other member.
extern "C" {
void __t6VIList1ZP10VIFlatFile(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    words[2] = 0;
    words[0] = 0;
    words[1] = 0;
}
}
