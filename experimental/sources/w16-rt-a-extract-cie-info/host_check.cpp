// Host check (not a plan input). Build (host pointers are 64-bit, so the record is the host layout):
//   c++ -O1 -std=c++17 -c -Dextract_cie_info=w16_extract -Dstrcmp=w16_strcmp -Dstrlen=w16_strlen -Ddecode_uleb128=w16_uleb \
//       -Ddecode_sleb128=w16_sleb -o /tmp/w16_eci_src.o extract_cie_info.cpp
//   c++ -O1 -std=c++17 -o /tmp/w16_eci host_check.cpp /tmp/w16_eci_src.o && /tmp/w16_eci
// The two LEB128 decoders are exercised with their real encodings here (stand-ins that implement the format).
#include <cstdio>
#include <cstring>
#include <vector>

struct CieInfo { const char* text; unsigned eh_pointer; unsigned code_align; int data_align; unsigned return_column; };
extern "C" const unsigned char* w16_extract(const unsigned char* entry, CieInfo* info);

static std::vector<int> trace;
extern "C" int w16_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
extern "C" unsigned w16_strlen(const char* t) { return static_cast<unsigned>(std::strlen(t)); }
extern "C" const unsigned char* w16_uleb(const unsigned char* p, unsigned* v) {
    unsigned r = 0, s = 0;
    unsigned char b;
    do { b = *p++; r |= static_cast<unsigned>(b & 0x7f) << s; s += 7; } while (b & 0x80);
    *v = r; trace.push_back(1);
    return p;
}
extern "C" const unsigned char* w16_sleb(const unsigned char* p, int* v) {
    int r = 0; unsigned s = 0; unsigned char b;
    do { b = *p++; r |= (b & 0x7f) << s; s += 7; } while (b & 0x80);
    if (s < 32 && (b & 0x40)) r |= -(1 << s);
    *v = r; trace.push_back(2);
    return p;
}

// Buffer: [cie: 4 length, 4 id, 1 version, text..., body...] then padding, then the fde whose word 1 is the back offset.
struct Case { const char* text; std::vector<unsigned char> body; bool expect_null; };

int main() {
    unsigned cases = 0;
    const std::vector<Case> list = {
        {"", {0x04, 0x7c, 0x1f}, false},
        {"eh", {0x78, 0x56, 0x34, 0x12, 0x01, 0x7e, 0x21}, false},
        {"zR", {0x01, 0x78, 0x0e, 0x01, 0x1b}, false},       // skip count 1 after the return column
        {"zPLR", {0x81, 0x01, 0x7c, 0x0f, 0x03, 0xaa, 0xbb, 0xcc}, false},  // multi-byte code alignment, skip 3
        {"x", {0x01, 0x01, 0x01}, true},
        {"ehz", {0x01, 0x01, 0x01}, true},
        {"e", {0x01, 0x01, 0x01}, true},
    };
    for (const Case& c : list) {
        alignas(8) unsigned char buffer[128] = {};
        const unsigned cie = 8;
        buffer[cie + 8] = 1;
        std::memcpy(buffer + cie + 9, c.text, std::strlen(c.text) + 1);
        const unsigned body = cie + 9 + static_cast<unsigned>(std::strlen(c.text)) + 1;
        std::memcpy(buffer + body, c.body.data(), c.body.size());
        const unsigned fde = 64;
        // The text must sit at entry + 13 - back, so back = fde + 13 - (cie + 9).
        const int back = static_cast<int>(fde + 13 - (cie + 9));
        std::memcpy(buffer + fde + 4, &back, 4);
        CieInfo info;
        std::memset(&info, 0xaa, sizeof info);
        trace.clear();
        const unsigned char* result = w16_extract(buffer + fde, &info);
        if (info.text != reinterpret_cast<const char*>(buffer + cie + 9)) return 1;
        if (c.expect_null) {
            if (result != nullptr || !trace.empty() || info.eh_pointer != 0xaaaaaaaau) return 2;
            ++cases;
            continue;
        }
        const bool eh = std::strcmp(c.text, "eh") == 0;
        const unsigned char* p = buffer + body;
        unsigned expect_eh = 0;
        if (eh) { std::memcpy(&expect_eh, p, 4); p += 4; }
        if (info.eh_pointer != expect_eh) return 3;
        unsigned ca; const unsigned char* q = w16_uleb(p, &ca); int da; q = w16_sleb(q, &da);
        const unsigned rc = *q++;
        if (c.text[0] == 'z') { unsigned skip; q = w16_uleb(q, &skip); q += skip; }
        if (info.code_align != ca || info.data_align != da || info.return_column != rc) return 4;
        if (result != q) { std::printf("cursor off by %ld for %s\n", static_cast<long>(result - q), c.text); return 5; }
        ++cases;
    }
    std::printf("%u extract_cie_info cases passed\n", cases);
    return 0;
}
