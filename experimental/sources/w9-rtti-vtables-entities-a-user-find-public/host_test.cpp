// Host comparison only. Not an EE build input.
#include <cstdio>
#include "user_find_public.cpp"

int main() {
    char buffer[8];
    int bad = 0;
    if (do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3(nullptr, 9, nullptr, buffer, buffer) != 6) ++bad;
    if (do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3(nullptr, 9, nullptr, buffer, buffer + 1) != 1) ++bad;
    if (do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3(nullptr, 0, nullptr, nullptr, nullptr) != 6) ++bad;
    std::printf(bad ? "failed\n" : "user do_find_public_subobj cases passed\n");
    return bad != 0;
}
