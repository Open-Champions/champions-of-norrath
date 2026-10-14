// Observed: Begin__Ct6VIList1ZP10VIFlatFile at 0x011f3bc8 is 40 bytes. It reads the word at
// list+4. If that word is nonzero it returns it unchanged; if it is zero it tail-calls
// End__Ct6VIList1ZP10VIFlatFile with the same list and returns that call's result.
// Inferred: list+4 is the head node pointer of a linked list (the material list keeps count,
// head, tail at +0, +4, +8), so this is the begin iterator and End supplies the empty value.
// Unknown: the template parameters beyond the mangled name, and what End returns.
extern "C" {
void* End__Ct6VIList1ZP10VIFlatFile(const void* list);

void* Begin__Ct6VIList1ZP10VIFlatFile(const void* list) {
    void* const head = *reinterpret_cast<void* const*>(static_cast<const unsigned char*>(list) + 4);
    if (head == 0) return End__Ct6VIList1ZP10VIFlatFile(list);
    return head;
}
}
