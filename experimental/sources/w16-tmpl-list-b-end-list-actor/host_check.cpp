// Host check only; not a build input.
#include "end.cpp"
int main() {
    unsigned o[4] = {1, 2, 3, 4};
    if (End__Ct6VIList1Z7VIActor(o) != 0x7fffffff) return 1;
    if (End__Ct6VIList1Z7VIActor(nullptr) != 0x7fffffff) return 2;
    if (o[0] != 1 || o[1] != 2 || o[2] != 3 || o[3] != 4) return 3;
    return 0;
}
