#include "init.cpp"

static void* seen;
static int seen_count;
static int result;
extern "C" int Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* map, int count) {
    seen = map;
    seen_count = count;
    return result;
}

int main() {
    char object[8];
    result = 0;
    if (Init__12VIDictionaryi(object, 64) != 0 || seen != object || seen_count != 64) return 1;
    result = 17;
    if (Init__12VIDictionaryi(object, 3) != 0 || seen_count != 3) return 2;
    result = -1;
    if (Init__12VIDictionaryi(object, 3) != -1) return 3;
    result = -200;
    if (Init__12VIDictionaryi(object, 3) != -1) return 4;
    return 0;
}
