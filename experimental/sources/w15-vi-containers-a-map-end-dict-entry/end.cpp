// Observed: End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry at 0x0120a628, 8 bytes (a weak const member). It returns zero and
// reads nothing from the map.
// Inferred: index 0 is the "none" node of the index-based tree, so zero is the end iterator that Find and Next hand back.
// Unknown: nothing beyond the constant result.
extern "C" {
int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* map) {
    (void)map;
    return 0;
}
}
