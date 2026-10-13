// Host check only; not a build input.
#include "ctor.cpp"
static void* seen;
static int calls;
extern "C" void Init__7VILList(void* self) { seen = self; ++calls; }
int main() {
    int x;
    __7VILList(&x);
    return (calls == 1 && seen == &x) ? 0 : 1;
}
