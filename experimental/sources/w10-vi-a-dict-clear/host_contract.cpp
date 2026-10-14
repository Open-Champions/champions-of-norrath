#include "clear.cpp"

static void* seen;
static int calls;
extern "C" void Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* map) {
    seen = map;
    ++calls;
}

int main() {
    char object[16];
    Clear__12VIDictionary(object);
    if (calls != 1 || seen != object) return 1;
    Clear__12VIDictionary(object + 8);
    if (calls != 2 || seen != object + 8) return 2;
    return 0;
}
