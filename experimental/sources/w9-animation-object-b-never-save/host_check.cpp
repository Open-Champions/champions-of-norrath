// Host check for the neverSave table. Not an EE plan input.
#include <cstdio>
#include <cstring>
extern "C" {
const char norrath_factory_text_483b38[] = "a0", norrath_factory_text_483b40[] = "a1", norrath_factory_text_483b48[] = "a2",
           norrath_factory_text_461fc0[] = "a3", norrath_factory_text_483b50[] = "a4", norrath_factory_text_461fd0[] = "a5",
           norrath_factory_text_461fe0[] = "a6", norrath_factory_text_483b60[] = "a8", norrath_factory_text_483b68[] = "a9",
           norrath_factory_text_461ff0[] = "a10", norrath_factory_text_462000[] = "a11", norrath_factory_text_462010[] = "a12",
           norrath_factory_text_462028[] = "a13", norrath_factory_text_462040[] = "a14", norrath_factory_text_462058[] = "a15",
           norrath_factory_text_462068[] = "a17";
}
#include "never_save.cpp"
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    expect(neverSave[18] == nullptr, "table ends with a null word");
    for (int i = 0; i < 18; ++i) expect(neverSave[i] != nullptr, "eighteen non-null entries");
    expect(std::strcmp(neverSave[7], "faytree") == 0, "faytree is the eighth entry");
    expect(neverSave[15] == neverSave[16] && neverSave[15] == norrath_factory_text_462058, "entries 15 and 16 are the same address");
    expect(neverSave[0] == norrath_factory_text_483b38 && neverSave[17] == norrath_factory_text_462068, "first and last providers in place");
    expect(neverSave[8] == norrath_factory_text_483b60 && neverSave[9] == norrath_factory_text_483b68 && neverSave[10] == norrath_factory_text_461ff0, "middle order");
    if (failures) return 1;
    std::printf("neverSave table check passed\n");
    return 0;
}
