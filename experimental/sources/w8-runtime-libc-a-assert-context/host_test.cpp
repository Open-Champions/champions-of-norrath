// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>

#include "assert_context.cpp"

int main() {
    if (norrath_assert_context_word_view == nullptr || std::strcmp(norrath_assert_context_word_view, "none") != 0) {
        std::printf("context word must start at the text \"none\"\n");
        return 1;
    }
    std::printf("assert context cases passed\n");
    return 0;
}
