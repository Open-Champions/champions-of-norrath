// Host check for the pm list words. Not an EE plan input.
extern "C" {
extern char pmlistchars[0x1000];
extern int pmlistcharsused;
}
int main() {
    for (char c : pmlistchars) if (c) return 1;
    return pmlistcharsused != 0;
}
