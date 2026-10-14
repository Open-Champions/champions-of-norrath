// Observed Clear__7VIWorld at 0x010fe478, 96 bytes. With the world as this, three members are
// cleared in order, and the first negative result stops the sequence with -1:
//   Clear__t7VIArray1Z6VIZone(this + 72), Clear__11VIWorldTree(this + 52),
//   Clear__t7VIArray1Z13VIWorldRegion(this + 80).
// The result is -1 when the last of them is negative, otherwise zero (a positive result of any
// of the three is not returned).
extern "C" {
int Clear__t7VIArray1Z6VIZone(void* array);
int Clear__11VIWorldTree(void* tree);
int Clear__t7VIArray1Z13VIWorldRegion(void* array);

int Clear__7VIWorld(void* self) {
    char* base = static_cast<char*>(self);
    if (Clear__t7VIArray1Z6VIZone(base + 72) < 0) return -1;
    if (Clear__11VIWorldTree(base + 52) < 0) return -1;
    return Clear__t7VIArray1Z13VIWorldRegion(base + 80) < 0 ? -1 : 0;
}
}
