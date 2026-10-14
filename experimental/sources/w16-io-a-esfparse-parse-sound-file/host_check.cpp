// Host check only; not a build input.
#include <string>
#include <stdint.h>
#include "parse_sound_file.cpp"
static std::string tr; static int open_ret, type_ret, count_ret, parse_ret, close_ret;
static void* objectSeen; static const char* nameSeen; static int modeSeen; static unsigned sceneLow;
extern "C" void __9VIObjFile(void* object) { objectSeen = object; tr += "C;"; }
extern "C" void _$_9VIObjFile(void* object, int flags) { tr += (object == objectSeen && flags == 2) ? "D;" : "D?;"; }
extern "C" int Open__9VIObjFilePCcQ29VIObjFile8FileMode(void* object, const char* name, int mode) {
    nameSeen = name; modeSeen = mode; tr += object == objectSeen ? "O;" : "O?;"; return open_ret;
}
extern "C" int FileType__9VIObjFile(void*) { tr += "T;"; return type_ret; }
extern "C" int NumObjects__9VIObjFile(void*) { tr += "N;"; return count_ret; }
extern "C" int Close__9VIObjFile(void*) { tr += "X;"; return close_ret; }
extern "C" int ParseSound__10VIESFParse(void* parse) {
    unsigned* w = static_cast<unsigned*>(parse);
    tr += (w[6] == sceneLow && w[8] == 1 && w[3] == 2 && w[5] == 4 && w[4] == 3 && w[7] == 0 &&
           w[9] == static_cast<unsigned>(reinterpret_cast<uintptr_t>(objectSeen))) ? "P;" : "P?;";
    return parse_ret;
}
int main() {
    alignas(8) unsigned char parse[64];
    alignas(8) static unsigned char scene[20000];
    unsigned* sw = reinterpret_cast<unsigned*>(scene);
    sw[19352 / 4] = 1; sw[19356 / 4] = 2; sw[19360 / 4] = 3; sw[19364 / 4] = 4;
    sceneLow = static_cast<unsigned>(reinterpret_cast<uintptr_t>(scene));
    unsigned* pw = reinterpret_cast<unsigned*>(parse);
    auto run = [&]() { for (auto& b : parse) b = 0xcc; tr.clear(); return ParseSound__10VIESFParsePCcR7VIScene(parse, "n", scene); };
    open_ret = 0; type_ret = 0xab4f; count_ret = 1; parse_ret = 7; close_ret = 0;
    if (run() != 7 || tr != "C;O;T;N;P;X;D;" || nameSeen == nullptr || modeSeen != 0) return 1;
    for (int i : {6, 7, 8, 3, 4, 5, 9}) if (pw[i] != 0) return 2;
    open_ret = -1;
    if (run() != -1 || tr != "C;O;D;") return 3;
    if (pw[6] != sceneLow || pw[8] != 1 || pw[9] == 0) return 4;
    open_ret = 0; type_ret = 0xab50;
    if (run() != -1 || tr != "C;O;T;D;") return 5;
    type_ret = 0xab4f; count_ret = 2;
    if (run() != -1 || tr != "C;O;T;N;D;") return 6;
    count_ret = 1; parse_ret = -3;
    if (run() != -1 || tr != "C;O;T;N;P;D;" || pw[6] != sceneLow) return 7;
    parse_ret = 0; close_ret = -1;
    if (run() != -1 || tr != "C;O;T;N;P;X;D;" || pw[6] != sceneLow) return 8;
    close_ret = 0;
    if (run() != 0 || pw[6] != 0) return 9;
    return 0;
}
