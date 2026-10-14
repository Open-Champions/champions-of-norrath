// Host check: halfword text is modelled with std::u16string; textGetString__FPc returns "T(<key>)".
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct NameDescription { char* key; };
struct NameItem {
    const NameDescription* description;
    unsigned char gap[8 - sizeof(void*)];
    long long flags;
    unsigned char gap2[32 - 16];
    unsigned char prefix, suffix;
};

extern "C" {
unsigned char _4item$prefixTable[40 * 8];
unsigned char _4item$suffixTable[20 * 8];
signed char language;
unsigned short* getName__4itemPQ24item11descriptioni(const NameItem*, int);
}

static std::string trace;
static int genderResult = 0, genderCalls;
static std::vector<std::u16string> pool(64);
static int poolNext;
static std::u16string widen(const std::string& s) { return std::u16string(s.begin(), s.end()); }
static std::string narrow(const unsigned short* p) { std::string s; while (*p) s += (char)*p++; return s; }
static unsigned short* make(const std::string& s) { pool[poolNext] = widen(s); return (unsigned short*)pool[poolNext++].data(); }
static void cat(unsigned short* d, const std::string& s) { size_t n = 0; while (d[n]) ++n; for (char c : s) d[n++] = (unsigned char)c; d[n] = 0; }

extern "C" {
char* strcpy(char* d, const char* s) { trace += "cpy:"; trace += s; trace += ";"; char* o = d; while ((*d++ = *s++)) {} return o; }
char* strchr(const char* s, int c) { for (; *s; ++s) if (*s == (char)c) return (char*)s; return nullptr; }
int sprintf(char* d, const char* f, ...) {
    // only "%s_name" with the destination as its own argument, or with a description key
    __builtin_va_list ap; __builtin_va_start(ap, f); const char* a = __builtin_va_arg(ap, const char*); __builtin_va_end(ap);
    std::string s = std::string(a) + "_name"; for (size_t k = 0; k <= s.size(); ++k) d[k] = s.c_str()[k]; trace += "fmt:" + s + ";"; return (int)s.size();
}
unsigned short* textGetString__FPc(char* k) { trace += std::string("txt:") + k + ";"; return make(std::string("T(") + k + ")"); }
void ustrcat__FPUsT0(unsigned short* d, unsigned short* s) { trace += "cat:" + narrow(s) + ";"; cat(d, narrow(s)); }
void ustrcat__FPUsPc(unsigned short* d, const char* s) { trace += std::string("catc:") + s + ";"; cat(d, s); }
int findAndStripGender__4itemPUs(unsigned short*) { ++genderCalls; trace += "gender;"; return genderResult; }
unsigned short* getAttributeName__4itemiPc(int m, char* key) { trace += "attr(" + std::to_string(m) + "," + key + ");"; return make(std::string("A") + std::to_string(m)); }
void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c, float e, float g, float h, float i, float j) {
    trace += "usp:" + narrow(f) + ";"; d[0] = 0; cat(d, narrow(f));
    if (a || b || c || e || g || h || i || j) trace += "NONZERO;";
}
}

static char keys[8][16];
static NameDescription desc = {keys[0]};
static NameItem item;
static void setup(const char* key, long long flags, int prefix, int suffix, int lang) {
    for (size_t k = 0; k <= std::strlen(key); ++k) keys[0][k] = key[k]; desc.key = keys[0];
    item = NameItem{}; item.description = &desc; item.flags = flags; item.prefix = prefix; item.suffix = suffix; language = lang;
    // row keys
    static char pk[8][8], sk[8][8];
    for (int i = 0; i < 8; ++i) {
        std::snprintf(pk[i], 8, "pre%d", i); std::snprintf(sk[i], 8, "suf%d", i);
        char* a = pk[i]; char* b = sk[i];
        std::memcpy(_4item$prefixTable + 40 * i, &a, sizeof a); std::memcpy(_4item$suffixTable + 20 * i, &b, sizeof b);
    }
    trace.clear(); poolNext = 0; genderCalls = 0;
}
static std::string run(int decorate) { return narrow(getName__4itemPQ24item11descriptioni(&item, decorate)); }
#define EXPECT(got, want, code) do { std::string g = (got); if (g != (want)) { std::printf("fail %d: got '%s' want '%s' trace %s\n", code, g.c_str(), (want), trace.c_str()); return code; } } while (0)

int main() {
    // not decorated, or flag bit 9: just "<key>_name" text, key not trimmed at '+'
    setup("axe+3", 0, 1, 1, 0); EXPECT(run(0), "T(axe+3_name)", 1);
    EXPECT(trace, "fmt:axe+3_name;txt:axe+3_name;cat:T(axe+3_name);gender;", 2);
    setup("axe+3", 0x200, 1, 1, 0); EXPECT(run(1), "T(axe+3_name)", 3);
    struct Case { const char* key; int prefix, suffix, lang, gender; const char* result; const char* traceHas; };
    const Case cases[] = {
        // other languages: attribute (gender code) first, then the text, suffix with "of"
        {"axe+3", 2, 3, 0, 2, "A2 T(axe_name) T(of) A2", "attr(2,pre2);cat:A2;catc: ;cpy:axe+3;"},
        {"axe+3", 255, 255, 0, 2, "T(axe_name)", "gender;cpy:axe+3;fmt:axe_name;txt:axe_name;cat:T(axe_name);gender;"},
        {"axe", 255, 3, 0, 1, "T(axe_name) T(of) A1", "txt:of;cat:T(of);catc: ;attr(1,suf3);cat:A1;"},
        // language 3: text, then " " and the attribute chosen from the gender code
        {"axe+3", 2, 3, 3, 4, "T(axe_name) A3 T(suf3)", "catc: ;attr(3,pre2);cat:A3;catc: ;txt:suf3;cat:T(suf3);"},
        {"axe+3", 2, 255, 3, 9, "T(axe_name) A-1", "attr(-1,pre2);cat:A-1;"},
        {"axe+3", 2, 255, 3, 1, "T(axe_name) A0", "attr(0,pre2);"},
        {"axe+3", 2, 255, 3, 2, "T(axe_name) A1", "attr(1,pre2);"},
        {"axe+3", 2, 255, 3, 3, "T(axe_name) A2", "attr(2,pre2);"},
        {"axe", 255, 3, 3, 1, "T(axe_name) T(suf3)", "catc: ;txt:suf3;cat:T(suf3);"},
        // language 4: the attribute replaces the buffer through usprintf, then " " and the text
        {"axe+3", 2, 3, 4, 2, "A1 T(axe_name) T(suf3)", "attr(1,pre2);usp:A1;catc: ;txt:axe_name;cat:T(axe_name);gender;catc: ;txt:suf3;"},
        {"axe+3", 2, 255, 4, 9, "A2 T(axe_name)", "attr(2,pre2);usp:A2;"},
        {"axe+3", 2, 255, 4, 1, "A0 T(axe_name)", "attr(0,pre2);"},
        {"axe+3", 2, 255, 4, 4, "A3 T(axe_name)", "attr(3,pre2);"},
        {"axe", 255, 3, 4, 1, "T(axe_name) T(suf3)", "catc: ;txt:suf3;cat:T(suf3);"},
    };
    int n = 0;
    for (const Case& c : cases) {
        setup(c.key, 0, c.prefix, c.suffix, c.lang); genderResult = c.gender;
        std::string r = run(1);
        ++n;
        EXPECT(r, c.result, 10 + n);
        if (trace.find(c.traceHas) == std::string::npos) { std::printf("fail %d: trace %s\n", 10 + n, trace.c_str()); return 10 + n; }
        if (trace.find("NONZERO") != std::string::npos) return 50;
    }
    // the buffer is cleared at the start: a second call does not accumulate
    setup("axe", 0, 255, 255, 0); genderResult = 0;
    run(1); EXPECT(run(1), "T(axe_name)", 60);
    return 0;
}
