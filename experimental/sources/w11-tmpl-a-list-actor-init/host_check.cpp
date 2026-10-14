#include "init.cpp"
static int ret, calls;
static void* seen;
extern "C" int Clear__t6VIList1Z7VIActor(void* list) { seen = list; ++calls; return ret; }
int main() {
    char list[16];
    int cases[5][2] = {{0, 0}, {7, 0}, {-1, -1}, {-5, -1}, {0x7fffffff, 0}};
    for (int i = 0; i < 5; ++i) {
        ret = cases[i][0]; calls = 0; seen = 0;
        if (Init__t6VIList1Z7VIActor(list) != cases[i][1] || calls != 1 || seen != list) return 1 + i;
    }
    return 0;
}
