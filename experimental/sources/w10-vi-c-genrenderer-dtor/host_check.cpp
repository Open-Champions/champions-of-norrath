// Host check only; not a build input. The list is a fake array of nodes whose first word is a
// page pointer; table entries are host-sized records, so the check follows the call order,
// the adjusted object pointers and the flag arguments rather than 32-bit layouts. The same
// scenarios (empty list, several pages, a null page, absent child, absent word at +56, flags
// 0 to 3) were compared with the original in a private interpreter run.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$20VIWndGenericRenderer[8];
static unsigned char renderer[0x800];
static unsigned char child[0x100];
static unsigned char page56[0x200];
static unsigned char pages[4][0x200];
static void* nodes[4][2];
static VtEntry child_table[2], page_table[2];
static int node_count;
static std::string trace;

static std::string name_of(void* p) {
    unsigned char* b = static_cast<unsigned char*>(p);
    if (b >= child && b < child + 0x100) return "child+" + std::to_string(b - child);
    if (b >= page56 && b < page56 + 0x200) return "page56+" + std::to_string(b - page56);
    for (int i = 0; i < 4; ++i)
        if (b >= pages[i] && b < pages[i] + 0x200) return "page" + std::to_string(i) + "+" + std::to_string(b - pages[i]);
    if (b >= renderer && b < renderer + 0x800) return "self+" + std::to_string(b - renderer);
    return "?";
}
static void child_fn(void* p, int flag) { trace += "child:" + name_of(p) + "/" + std::to_string(flag) + ";"; }
static void page_fn(void* p, int flag) { trace += "page:" + name_of(p) + "/" + std::to_string(flag) + ";"; }

extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* c) { trace += "rm:" + name_of(self) + "," + name_of(c) + ";"; }
Iterator Begin__Ct6VIList1ZP9VIGUIPage(void* list) { trace += "begin:" + name_of(list) + ";"; return reinterpret_cast<Iterator>(&nodes[0][0]); }
Iterator End__Ct6VIList1ZP9VIGUIPage(void*) { return reinterpret_cast<Iterator>(&nodes[node_count][0]); }
Iterator Next__Ct6VIList1ZP9VIGUIPagei(void*, Iterator it) { trace += "next;"; return it + static_cast<Iterator>(sizeof nodes[0]); }
void Erase__t6VIList1ZP9VIGUIPagei(void*, Iterator it) {
    trace += "erase" + std::to_string((it - reinterpret_cast<Iterator>(&nodes[0][0])) / static_cast<Iterator>(sizeof nodes[0])) + ";";
}
void _$_16VIRealmInterface(void* s, int f) { trace += "realm:" + name_of(s) + "/" + std::to_string(f) + ";"; }
void _$_9VIWndEdit(void* s, int f) { trace += "edit:" + name_of(s) + "/" + std::to_string(f) + ";"; }
void _$_19VIFlatFileStructure(void* s, int f) { trace += "ffs:" + name_of(s) + "/" + std::to_string(f) + ";"; }
void _$_t6VIList1ZP9VIGUIPage(void* s, int f) { trace += "list:" + name_of(s) + "/" + std::to_string(f) + ";"; }
void _$_5VIWnd(void* s, int f) { trace += "base:" + name_of(s) + "/" + std::to_string(f) + ";"; }
void __builtin_delete(void* p) { trace += "delete:" + name_of(p) + ";"; }
}

static bool run(int count, bool with_child, bool with_page56, int null_page, int flags) {
    std::memset(renderer, 0x5a, sizeof renderer);
    std::memset(nodes, 0, sizeof nodes);
    node_count = count;
    trace.clear();
    child_table[1] = VtEntry{16, 5, child_fn};
    page_table[1] = VtEntry{8, 7, page_fn};
    VtEntry* ct = child_table; VtEntry* pt = page_table;
    std::memcpy(child + 32, &ct, sizeof ct);
    void* p = with_child ? static_cast<void*>(child) : nullptr;
    std::memcpy(renderer + 36, &p, sizeof p);
    p = with_page56 ? static_cast<void*>(page56) : nullptr;
    std::memcpy(renderer + 56, &p, sizeof p);
    std::memcpy(page56 + 120, &pt, sizeof pt);
    for (int i = 0; i < count; ++i) {
        void* pg = i == null_page ? nullptr : static_cast<void*>(pages[i]);
        nodes[i][0] = pg;
        std::memcpy(pages[i] + 120, &pt, sizeof pt);
    }
    dtor_20VIWndGenericRenderer(renderer, flags);
    std::string want;
    if (with_child) want += "rm:self+0,child+0;child:child+16/3;";
    want += "begin:self+40;";
    for (int i = 0; i < count; ++i) {
        if (i != null_page) want += "page:page" + std::to_string(i) + "+8/3;";
        want += "next;erase" + std::to_string(i) + ";";
    }
    if (with_page56) want += "page:page56+8/3;";
    want += "realm:self+1880/2;edit:self+1424/2;ffs:self+1364/2;list:self+40/2;base:self+0/0;";
    if (flags & 1) want += "delete:self+0;";
    bool ok = trace == want;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    unsigned w;
    std::memcpy(&w, renderer + 32, 4);
    if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$20VIWndGenericRenderer))) ok = false;
    unsigned zero52; std::memcpy(&zero52, renderer + 52, 4);
    if (zero52 != 0) ok = false;
    void* q; std::memcpy(&q, renderer + 36, sizeof q);
    if (with_child && q != nullptr) ok = false;
    std::memcpy(&q, renderer + 56, sizeof q);
    if (with_page56 && q != nullptr) ok = false;
    return ok;
}

int main() {
    bool ok = true;
    ok = run(0, true, true, -1, 0) && ok;
    ok = run(1, true, true, -1, 1) && ok;
    ok = run(3, true, true, -1, 3) && ok;
    ok = run(3, true, true, 1, 0) && ok;
    ok = run(2, false, false, -1, 2) && ok;
    ok = run(2, true, false, 0, 1) && ok;
    ok = run(0, false, true, -1, 0) && ok;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
