#include "startup_strings.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_startup_formats_view == 40, "five format slots");
    static_assert(sizeof norrath_sound_startup_assert_file_view == 9, "sound.cc");
    static_assert(sizeof norrath_sound_startup_assert_conditions_view == 27, "two conditions");
    const char* slots[] = {"a\n", "b\n", "c\n", "d\n", "e\n"};
    for (int i = 0; i < 5; ++i) {
        if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_startup_formats_view + i * 8), slots[i]) != 0)
            return 1;
    }
    if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_startup_assert_file_view), "sound.cc") != 0)
        return 2;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_startup_assert_conditions_view), "!batchIopBuffer") != 0)
        return 3;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_startup_assert_conditions_view + 16), "!IOPbuffer") != 0)
        return 4;
    std::puts("startup strings passed");
    return 0;
}
