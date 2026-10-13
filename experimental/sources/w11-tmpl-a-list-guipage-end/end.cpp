// Observed: End__Ct6VIList1ZP9VIGUIPage at 0x01209600, 12 bytes. It ignores its argument and
// returns the constant 0x7fffffff.
// Inferred: the end marker of iteration over VIList<VIGUIPage*>.
extern "C" {
int End__Ct6VIList1ZP9VIGUIPage(const void* list) {
    (void)list;
    return 0x7fffffff;
}
}
