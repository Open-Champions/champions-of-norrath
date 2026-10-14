#include <cstdio>
#include "in_post_action.cpp"
int main() {
    if (sizeof scriptInPostAction != 1 || scriptInPostAction != 0) return 1;
    scriptInPostAction = 1;
    std::puts("ok");
    return 0;
}
