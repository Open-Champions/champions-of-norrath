// Host contract for HateHole msg_run; not an EE input.
#include "msg_run.cpp"
#include <cstdio>
static int calls; static void* seen;
void msg_run__8AutoProp(void* self) { ++calls; seen = self; }
int main() {
    int object[4], other[4];
    msg_run__8HateHole(object);
    if (calls != 1 || seen != object) { std::fputs("fail: base call\n", stderr); return 1; }
    msg_run__8HateHole(other);
    if (calls != 2 || seen != other) { std::fputs("fail: second call\n", stderr); return 1; }
    std::puts("hatehole msg_run ok");
    return 0;
}
