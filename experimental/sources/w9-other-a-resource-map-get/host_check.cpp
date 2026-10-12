#include "resource_map_get.cpp"
int resourceFrame;
static int calls;
static char* seenFirst;
static char* seenSecond;
static int result;
void* lumpFindResource__FPcT0(char* first, char* second) {
    ++calls;
    seenFirst = first;
    seenSecond = second;
    return &result;
}
int main() {
    char a[] = "a", b[] = "b";
    int cache;
    ResourceMap map = {4, a, b, &cache};
    resourceFrame = 4;
    if (get__11ResourceMap(&map) != &cache || calls != 0) return 1;
    resourceFrame = 5;
    if (get__11ResourceMap(&map) != &result || calls != 1) return 2;
    if (seenFirst != a || seenSecond != b || map.frame != 5 || map.cached != &result) return 3;
    if (get__11ResourceMap(&map) != &result || calls != 1) return 4;
    return 0;
}
