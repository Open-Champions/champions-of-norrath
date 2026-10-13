#include "dtor.cpp"

static int order[8];
static int count;
static int map_flags = -1;
static void* deleted;

extern "C" void Clear__12VIDictionary(void* object) { order[count++] = (object ? 1 : 99); }
extern "C" void _$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map, int flags) {
    order[count++] = (map ? 2 : 99);
    map_flags = flags;
}
extern "C" void __builtin_delete(void* pointer) {
    order[count++] = 3;
    deleted = pointer;
}

int main() {
    char object[8];
    _$_12VIDictionary(object, 0);
    if (count != 2 || order[0] != 1 || order[1] != 2 || map_flags != 2 || deleted) return 1;
    count = 0;
    _$_12VIDictionary(object, 2);
    if (count != 2 || deleted) return 2;
    count = 0;
    _$_12VIDictionary(object, 1);
    if (count != 3 || order[2] != 3 || map_flags != 2 || deleted != object) return 3;
    return 0;
}
