// Host comparison only. Not an EE build input. Builds a header image with sorted
// public records and checks every lookup against a linear-search model, plus
// misses before, between and after the names and the empty table.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#define amx_FindPublic w9_amx_find_public_under_test
#include "amx_find_public.cpp"
#undef amx_FindPublic

static int run(const std::vector<std::string>& names, int& failures) {
    std::vector<unsigned char> image(64 + names.size() * 24 + 8, 0);
    short count = static_cast<short>(names.size());
    std::memcpy(&image[32], &count, 2);
    unsigned offset = 64;
    std::memcpy(&image[36], &offset, 4);
    for (size_t i = 0; i < names.size(); ++i)
        std::strcpy(reinterpret_cast<char*>(&image[64 + i * 24 + 4]), names[i].c_str());
    AmxView amx{image.data()};
    std::vector<std::string> probes = names;
    probes.insert(probes.end(), {"", "!", "m", "zzzz", "b0", "a"});
    for (const auto& p : probes) {
        int index = -5;
        int status = w9_amx_find_public_under_test(&amx, p.c_str(), &index);
        auto it = std::find(names.begin(), names.end(), p);
        int want_status = it == names.end() ? 19 : 0;
        int want_index = it == names.end() ? 0x7fffffff : int(it - names.begin());
        if (status != want_status || index != want_index) {
            std::printf("count=%zu name=%s got %d/%d want %d/%d\n", names.size(), p.c_str(), status, index, want_status, want_index);
            ++failures;
        }
    }
    return 0;
}

int main() {
    int failures = 0;
    run({}, failures);
    run({"alpha"}, failures);
    run({"alpha", "beta"}, failures);
    std::vector<std::string> many;
    for (int i = 0; i < 37; ++i) { char b[16]; std::snprintf(b, sizeof b, "f%02d", i * 2); many.push_back(b); }
    run(many, failures);
    many.push_back("g");
    run(many, failures);
    if (failures) return 2;
    std::printf("amx_FindPublic cases passed\n");
    return 0;
}
