#include "remove.cpp"

static int find_ret, end_ret, erase_ret, order[8], on, erase_arg; static void *fm, *fk, *em, *xm;
extern "C" int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(void* m, const void* k) { order[on++] = 1; fm = m; fk = (void*)k; return find_ret; }
extern "C" int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* m) { order[on++] = 2; em = m; return end_ret; }
extern "C" int Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void* m, int it) { order[on++] = 3; xm = m; erase_arg = it; return erase_ret; }

int main() {
    int dict, res;
    find_ret = 5; end_ret = 5;
    if (Remove__12VIDictionaryR10VIResource(&dict, &res) != -1 || on != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (fm != &dict || fk != &res || em != &dict) return 2;
    on = 0; find_ret = 4; erase_ret = -3;
    if (Remove__12VIDictionaryR10VIResource(&dict, &res) != -1 || on != 3 || order[2] != 3) return 3;
    if (xm != &dict || erase_arg != 4) return 4;
    on = 0; erase_ret = 0;
    if (Remove__12VIDictionaryR10VIResource(&dict, &res) != 0) return 5;
    on = 0; erase_ret = 8;
    if (Remove__12VIDictionaryR10VIResource(&dict, &res) != 0) return 6;
    return 0;
}
