#include "ctor.cpp"

static void* seen;
static int calls;
extern "C" void __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map) {
    seen = map;
    ++calls;
}

int main() {
    char object[16];
    __12VIDictionary(object);
    if (calls != 1 || seen != object) return 1;
    __12VIDictionary(object + 4);
    if (calls != 2 || seen != object + 4) return 2;
    return 0;
}
