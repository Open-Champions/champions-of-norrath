// Host check only; not a build input.
#include "end.cpp"
int main() {
    unsigned o[4] = {1, 2, 3, 4};
    if (End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(o) != 0) return 1;
    if (o[0] != 1 || o[1] != 2 || o[2] != 3 || o[3] != 4) return 2;
    return 0;
}
