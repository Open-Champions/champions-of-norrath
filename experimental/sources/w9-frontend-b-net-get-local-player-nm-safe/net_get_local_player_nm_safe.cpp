// Observed: netGetLocalPlayerNmSafe__Fi at 0x001354a8 is 96 bytes and calls
// nothing. It walks partner records 0 up to nmPartners (signed compare), where
// the records start at partners and are 17632 bytes apart. For each record whose
// signed byte at offset 17620 is nonzero it decrements the argument and, when
// the old value was zero, returns that record's index. It returns -1 if there
// are no records or the count is not reached. The argument is decremented only
// for records with a nonzero flag byte.
// Inferred: the result is the index of the Nth flagged partner, counting from
// zero; the "Nm" in the name is not interpreted.
// Unknown: what the flag byte at 17620 means.

extern "C" int nmPartners;
extern "C" signed char partners[];

extern "C" int netGetLocalPlayerNmSafe__Fi(int n) {
    for (int i = 0; i < nmPartners; ++i) {
        if (partners[17620 + 17632 * i] != 0) {
            if (n-- == 0) return i;
        }
    }
    return -1;
}
