// Observed: Begin__Ct6VIList1ZP9VIGUIPage at 0x012095d8, 40 bytes. Returns the word at list+4
// when it is nonzero; otherwise the result of the matching End routine for the same list
// (tail call).
// Inferred: list+4 is the first node; the iterator value is the node address as an int.
extern "C" {
int End__Ct6VIList1ZP9VIGUIPage(const void* list);

int Begin__Ct6VIList1ZP9VIGUIPage(const void* list) {
    int head = *(reinterpret_cast<const int*>(list) + 1);
    if (head != 0) return head;
    return End__Ct6VIList1ZP9VIGUIPage(list);
}
}
