// Host check only; not a build input. Also compared with the original in a private interpreter (4 names, all
// 41 words covered). snprintf is modelled for the single %s format; the check pins the buffer size, the format
// text, the name argument and that the result is the hash of the formatted text.
#include "hash_resource_id_icon.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
extern "C" int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const char* name = __builtin_va_arg(args, const char*);
    __builtin_va_end(args);
    std::string out = format;
    out.replace(out.find("%s"), 2, name);
    trace += "S" + std::to_string(size) + ",'" + format + "','" + name + "';";
    std::memcpy(buffer, out.c_str(), out.size() + 1);
    return static_cast<int>(out.size());
}
extern "C" unsigned VIHashResourceID(const char* name) {
    unsigned h = 17;
    for (const char* c = name; *c; ++c) h = h * 31 + static_cast<unsigned char>(*c);
    trace += std::string("H:") + name + ";";
    return h;
}
int main() {
    bool ok = true;
    for (const char* name : {"dnas_64", "", "a", "much_longer_icon_name"}) {
        trace.clear();
        const unsigned result = VIHashResourceID_Icon__FPCc(name);
        const std::string full = std::string("P:\\studio\\eqps2\\Game_Assets\\UI\\") + name + ".tga";
        unsigned h = 17;
        for (char c : full) h = h * 31 + static_cast<unsigned char>(c);
        ok = ok && result == h && trace == "S512,'P:\\studio\\eqps2\\Game_Assets\\UI\\%s.tga','" + std::string(name) + "';H:" + full + ";";
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
