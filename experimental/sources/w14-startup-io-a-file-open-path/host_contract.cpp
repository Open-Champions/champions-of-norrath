#include "open_path.cpp"
#include <cctype>
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

int norrath_file_iop_stream __asm__("_6VIFile$sIopStream") = 0;

static std::vector<std::string> calls;
static std::string path_seen, printed_path[2], printed_format[2], prefix_seen;
static int printf_count, search_result, snprintf_size;
static const void* search_entry, *search_file;
static unsigned long long new_pointer;
static unsigned new_size;
static std::string name_seen, format_seen;

extern "C" int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const char* prefix = va_arg(args, const char*);
    const char* name = va_arg(args, const char*);
    va_end(args);
    calls.push_back("snprintf");
    snprintf_size = size; format_seen = format; prefix_seen = prefix; name_seen = name;
    std::string out = std::string(prefix) + "\\" + name + ";1";
    std::strcpy(buffer, out.c_str());
    return static_cast<int>(out.size());
}
extern "C" char* strupr__8VIStringPc(char* text) {
    calls.push_back("strupr");
    for (char* p = text; *p; ++p) *p = static_cast<char>(std::toupper(static_cast<unsigned char>(*p)));
    return text;
}
extern "C" void CDDiskReady__6VIFilei(void*, int mode) { calls.push_back("ready" + std::to_string(mode)); }
extern "C" int CDSearchFile__6VIFileP10sceCdlFILEPCc(void* file, void* entry, const char* path) {
    calls.push_back("search"); search_file = file; search_entry = entry; path_seen = path; return search_result;
}
extern "C" int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    const char* arg = va_arg(args, const char*);
    va_end(args);
    calls.push_back("printf");
    if (printf_count < 2) { printed_format[printf_count] = format; printed_path[printf_count] = arg; }
    ++printf_count;
    return 0;
}
extern "C" void BufferClear__6VIFile(void*) { calls.push_back("clear"); }
extern "C" void* __builtin_vec_new(unsigned size) {
    calls.push_back("new"); new_size = size;
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(new_pointer));
}

int main() {
    int modes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0x38, 0x40, 0x41, 0x44, 0x80, 0x47, -8, -1};
    const char* names[] = {"fx/abc.lmp", "", "a\\b/c//d", "Mixed/Case"};
    for (int mode : modes) for (int found = 0; found < 2; ++found) for (int iop = 0; iop < 2; ++iop)
    for (unsigned long long block : {0ull, 0x500000ull, 0x500009ull, 0x50000full, 0x500010ull})
    for (const char* name : names) {
        alignas(8) unsigned char file[256];
        std::memset(file, 0x6b, sizeof file);
        calls.clear(); printf_count = 0; search_result = found; norrath_file_iop_stream = iop; new_pointer = block;
        int r = Open__6VIFilePCci(file, name, mode);
        if (*reinterpret_cast<int*>(file + 4) != mode) return 1;
        bool known = (mode & 7) != 0;
        if (!known) {
            if (r != -1 || !calls.empty()) return 2;
            continue;
        }
        std::string expect_path;
        for (const char* p = name; *p; ++p) expect_path += *p == '/' ? '\\' : static_cast<char>(std::toupper(static_cast<unsigned char>(*p)));
        expect_path = "\\" + expect_path + ";1";
        for (char& c : expect_path) if (c == '/') c = '\\';
        if (format_seen != "%s\\%s;1" || snprintf_size != 256 || !prefix_seen.empty() || name_seen != name) return 3;
        if (path_seen != expect_path || search_file != file || search_entry != file + 72) return 4;
        if (calls[0] != "snprintf" || calls[1] != "strupr" || calls[2] != "ready0" || calls[3] != "search") return 5;
        if (!found) {
            if (r != -1 || calls.size() != 5 || printed_format[0] != "VIEEFile: Unable to locate %s\n" || printed_path[0] != expect_path) return 6;
            if (file[108] != 0x6b || *reinterpret_cast<int*>(file) != 0x6b6b6b6b) return 7;
            continue;
        }
        if (printed_format[0] != "VIEEFile: %s opened\n" || printed_path[0] != expect_path) return 8;
        if (iop == 0) { if (file[108] != 20 || file[109] != 1 || file[110] != 0) return 9; }
        else if (file[108] != 0x6b || file[109] != 0x6b || file[110] != 0x6b) return 10;
        if (*reinterpret_cast<int*>(file) != 0 || *reinterpret_cast<long long*>(file + 16) != 0) return 11;
        if (calls[4] != "printf" || calls[5] != "clear") return 12;
        if (mode & 0x40) {
            if (r != 0 || calls.size() != 6 || *reinterpret_cast<int*>(file + 8) != 0) return 13;
            if (*reinterpret_cast<unsigned*>(file + 184) != 0x6b6b6b6bu) return 14;
        } else {
            if (calls.size() != 7 || calls[6] != "new" || new_size != 0x8010) return 15;
            if (*reinterpret_cast<unsigned*>(file + 184) != block) return 16;
            if (block == 0) {
                if (r != -1 || *reinterpret_cast<unsigned*>(file + 188) != 0x6b6b6b6bu || *reinterpret_cast<int*>(file + 8) != 0x6b6b6b6b) return 17;
            } else {
                if (r != 0 || *reinterpret_cast<unsigned*>(file + 188) != ((block + 16) & ~15ull) || *reinterpret_cast<int*>(file + 8) != 0) return 18;
            }
        }
        if (file[111] != 0x6b || file[12] != 0x6b || file[24] != 0x6b) return 19;
    }
    return 0;
}
