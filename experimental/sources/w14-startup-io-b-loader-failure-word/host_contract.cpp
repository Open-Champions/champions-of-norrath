#include "failure_word.cpp"

#include <cstdio>

int main() {
    if (sizeof norrath_loader_failure_word_view != 4u) return 1;
    if (norrath_loader_failure_word_view != 0u) return 2;
    norrath_loader_failure_word_view = 2u;  // cdLoad's failure store must be observable and keep its value
    if (norrath_loader_failure_word_view != 2u) return 3;
    std::puts("loader failure word contract passed");
    return 0;
}
