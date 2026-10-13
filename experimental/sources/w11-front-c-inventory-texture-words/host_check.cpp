// Host check for the inventory texture words. Not an EE plan input.
extern "C" {
extern void* tinyIcon[9];
extern void* selectionBox[4];
extern void* taskBar[12];
extern void* rcursor;
}
int main() {
    static_assert(sizeof(tinyIcon) / sizeof(tinyIcon[0]) == 9, "tinyIcon words");
    static_assert(sizeof(selectionBox) / sizeof(selectionBox[0]) == 4, "selectionBox words");
    static_assert(sizeof(taskBar) / sizeof(taskBar[0]) == 12, "taskBar words");
    for (void* p : tinyIcon) if (p) return 1;
    for (void* p : selectionBox) if (p) return 2;
    for (void* p : taskBar) if (p) return 3;
    return rcursor ? 4 : 0;
}
