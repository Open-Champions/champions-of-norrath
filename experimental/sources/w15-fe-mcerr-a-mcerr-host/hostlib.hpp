// Host check support only; not a build input. Include after the ProcessScreenNN source (with
// NORRATH_HOST_CONTRACT defined first). It provides recording stubs for the callees the memory-card error
// screens share, and fixed host objects behind the 32-bit "tag" words the sources read: the VIUI object
// (word _5VIWnd$mUI), a parent window with a message table, and the error-window object itself.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
static const unsigned TAG_UI = 0x1000, TAG_PARENT = 0x2000, TAG_TABLE = 0x3000;
static int hl_ui[64];
static unsigned char hl_parent[128];
static struct HlVt { short delta; short index; void (*function)(void*, void*, int, int, int); } hl_table[8];
static unsigned char hl_self[1100] __attribute__((aligned(16)));
static std::string trace;
static int hl_status[16], hl_status_count, hl_status_next;
static int hl_lines = 3;
static int hl_card;  // value the card-status stub writes through its pointer
static unsigned short hl_text[64][4];

extern "C" char* norrath_host_translate(unsigned address) {
    switch (address) {
    case TAG_UI: return reinterpret_cast<char*>(hl_ui);
    case TAG_PARENT: return reinterpret_cast<char*>(hl_parent);
    case TAG_TABLE: return reinterpret_cast<char*>(hl_table);
    default: return nullptr;
    }
}
static std::string hl_off(const void* p) {
    if (p == hl_ui) return "ui";
    if (p >= hl_self && p < hl_self + sizeof hl_self) return "self+" + std::to_string(static_cast<const unsigned char*>(p) - hl_self);
    if (p >= hl_parent && p < hl_parent + sizeof hl_parent) return "parent+" + std::to_string(static_cast<const unsigned char*>(p) - hl_parent);
    return p ? "?" : "0";
}
static void hl_parent_message(void* self, void* from, int message, int a, int b) {
    trace += "msg:" + hl_off(self) + "," + hl_off(from) + "," + std::to_string(message) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}
static void hl_reset(bool with_parent, int mode, int counter) {
    std::memset(hl_self, 0, sizeof hl_self);
    std::memset(hl_ui, 0, sizeof hl_ui);
    hl_ui[12] = 2;
    _5VIWnd$mUI = TAG_UI;
    hl_table[6] = {16, 6, hl_parent_message};
    unsigned t = TAG_TABLE;
    std::memcpy(hl_parent + 32, &t, 4);
    int* w = reinterpret_cast<int*>(hl_self);
    w[107] = with_parent ? static_cast<int>(TAG_PARENT) : 0;
    w[108] = mode;
    w[104] = counter;
    w[15] = 480;  // width word of the embedded window at +40 + 20
    trace.clear();
    hl_status_next = 0;
}
static std::string hl_tid(const unsigned short* t) {
    if (!t) return "0";
    const unsigned short* lo = &hl_text[0][0];
    if (t < lo || t >= lo + 64 * 4) return "L";  // a buffer owned by the caller
    return "t" + std::to_string(reinterpret_cast<const unsigned short(*)[4]>(t) - hl_text);
}
static int* hl_words() { return reinterpret_cast<int*>(hl_self); }
static int hl_check(const char* label, const std::string& expected) {
    if (trace == expected) return 0;
    std::fprintf(stdout, "MISMATCH %s\n  got:  %s\n  want: %s\n", label, trace.c_str(), expected.c_str());
    return 1;
}

extern "C" {
int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += std::string("P[") + format + "]";
    for (const char* f = format; *f; ++f)
        if (*f == '%' && f[1] == 'd') trace += "{" + std::to_string(va_arg(args, int)) + "}";
    va_end(args);
    return 0;
}
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column) {
    trace += "gs(" + std::string(table == hl_ui ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(column) + ");";
    return hl_text[id & 63];
}
int CountLines__10VIUIWindowPCUsi(void* window, const unsigned short* text, int width) {
    trace += "cl(" + hl_off(window) + "," + hl_tid(text) + "," + std::to_string(width) + ");";
    return hl_lines;
}
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, void* ui, const int* position, const int* size, int width, int a, int b) {
    trace += "init(" + hl_off(window) + "," + hl_off(ui) + ",pos=" + std::to_string(position[0]) + "/" + std::to_string(position[1]) +
             ",size=" + hl_off(size) + "," + std::to_string(width) + "," + std::to_string(a) + "," + std::to_string(b) + ");";
}
void Open__10VIUIWindowb(void* window, bool open) { trace += "open(" + hl_off(window) + "," + std::to_string(open) + ");"; }
void Begin2D__4VIUI(void* ui) { trace += "begin(" + hl_off(ui) + ");"; }
void End2D__4VIUI(void* ui) { trace += "end(" + hl_off(ui) + ");"; }
void Draw__10VIUIWindow(void* window) { trace += "draw(" + hl_off(window) + ");"; }
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d, int color, unsigned e) {
    trace += "box(" + hl_off(window) + "," + hl_tid(text) + "," + std::to_string(a) + "," +
             std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(color) + "," + std::to_string(e) + ");";
}
void DrawPrompt__10VIWndMcErriPCUsT2(void* self, int a, const unsigned short* text, const unsigned short* text2) {
    trace += "prompt(" + hl_off(self) + "," + std::to_string(a) + "," + hl_tid(text) + "," + hl_tid(text2) + ");";
}
int snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    const unsigned short* a = va_arg(args, const unsigned short*);
    const unsigned short* b = va_arg(args, const unsigned short*);
    const unsigned short* c = nullptr;
    int specifiers = 0;
    for (const char* f = format; *f; ++f) specifiers += f[0] == '%' && f[1] == 'S';
    if (specifiers >= 3) c = va_arg(args, const unsigned short*);
    va_end(args);
    std::string f = format;
    for (std::string::size_type i; (i = f.find('\n')) != std::string::npos;) f.replace(i, 1, "\\n");
    trace += "snp(" + std::to_string(size) + "," + f + "," + hl_tid(a) + "," + hl_tid(b) + (specifiers >= 3 ? "," + hl_tid(c) : std::string()) + ");";
    buffer[0] = 'x';
    buffer[1] = 0;
    return 1;
}
void DrawProgress__10VIUIWindowiiif(void* window, int a, int b, int c, float fraction) {
    unsigned bits;
    std::memcpy(&bits, &fraction, 4);
    trace += "progress(" + hl_off(window) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ",0x" + [&] {
        char buffer[16]; std::snprintf(buffer, sizeof buffer, "%08x", bits); return std::string(buffer); }() + ");";
}
int VIMemCardStatus__FPi(int* value) {
    trace += std::string("status(") + (value ? "ptr" : "0") + ");";
    if (value) *value = hl_card;
    return hl_status_next < hl_status_count ? hl_status[hl_status_next++] : hl_status[hl_status_count - 1];
}
}
