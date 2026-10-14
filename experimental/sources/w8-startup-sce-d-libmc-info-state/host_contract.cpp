#include "libmc_info_state.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_libmc_param_next_view == 192, "span to currentDir");
    static_assert(sizeof norrath_libmc_info_type_pointer_word_view == 4, "word");
    for (int word : norrath_libmc_param_next_view)
        if (word != 0) return 1;
    if (norrath_libmc_info_type_pointer_word_view[0] || norrath_libmc_info_free_pointer_word_view[0] ||
        norrath_libmc_info_format_pointer_word_view[0])
        return 2;
    std::puts("libmc info state contract passed");
    return 0;
}
