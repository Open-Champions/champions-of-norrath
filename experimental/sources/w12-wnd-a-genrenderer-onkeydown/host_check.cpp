// Host check only; not a build input. The renderer, dialog and page objects live in the arena with target-layout
// class tables (8-byte entries; the key-down handler is the entry at byte offset 24 of the table word at
// object+120). The string library helpers are small host models of the reference-counted representation described
// in the source comment (16-byte header: length, capacity, reference count, shared flag; characters follow), so the
// inline widen/narrow/copy/release sequence can be checked by its observable results: the printed text, the
// matchGame arguments, the balance of every reference count, and the free-list/free calls. The expected call log
// comes from a separate step-by-step model of the numbered list in the source comment.
#define NORRATH_HOST_ARENA 1
#include "onkeydown.cpp"
#include "../w16-wnd-a-common/host_world.hpp"
#include <cstring>
#include <vector>

unsigned _5VIWnd$mUI;
long long lastRefreshTime;
unsigned char norrath_nil_rep_ushort[16] __asm__("_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep");
unsigned char norrath_nil_rep_char[16] __asm__("_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep");
unsigned norrath_free_list[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");

static const unsigned renderer_g = hw::BASE + 0x100, page_g = hw::BASE + 0x6000, dialog_g = hw::BASE + 0x6400, object_g = hw::BASE + 0x6800,
                      table_page_g = hw::BASE + 0x6c00, table_dialog_g = hw::BASE + 0x6d00, table_object_g = hw::BASE + 0x6e00, ui_g = hw::BASE + 0x7000, combo_entry_g = hw::BASE + 0x7100;
static std::string trace;
static int dialog_open, list_size, edit_flag;
static long long time_now;
static const char* page_name = "";
static bool object_present, resize_shared;
static const unsigned short* session_text;
static unsigned short strings[3000][2];

static std::string Rg(unsigned g) { return std::to_string(g - hw::BASE); }
static std::string R(const void* p) { return p ? Rg(hw::guest(p)) : "0"; }
static std::string N(long long v) { return std::to_string(v); }
static void note(const std::string& s) { trace += s; }

// A pool of representations; every one must end up released.
struct Rep { unsigned char* base; unsigned elem; bool freed; bool extra_owner; };
static std::vector<Rep> reps;
static unsigned char pool[64][512];
static int pool_used;
static unsigned char* new_rep(unsigned length, unsigned element) {
    unsigned char* rep = pool[pool_used++];
    std::memset(rep, 0x5a, 512);
    std::memset(rep, 0, 16);
    const unsigned capacity = length + 2;
    std::memcpy(rep, &length, 4); std::memcpy(rep + 4, &capacity, 4);
    const unsigned one = 1; std::memcpy(rep + 8, &one, 4);
    reps.push_back(Rep{rep, element, false, false});
    return rep + 16;
}
static unsigned& field(unsigned char* chars, int off) { return *reinterpret_cast<unsigned*>(chars - 16 + off); }
static void dispose(unsigned char* chars) {
    unsigned& count = field(chars, 8);
    --count;
    if (count == 0)
        for (Rep& r : reps) if (r.base + 16 == chars) r.freed = true;
}
static void mark_freed(unsigned char* rep) { for (Rep& r : reps) if (r.base == rep) r.freed = true; }

extern "C" int IsOpen__11VIGUIDialog(void* d) { note("O" + R(d) + ";"); return dialog_open; }
extern "C" void OnKeyDown__9VIWndEditi(void* e, int key) { note("E" + R(e) + "," + N(key) + ";"); }
extern "C" int Size__Ct6VIList1ZP9VIGUIPage(const void* l) { note("Z" + R(l) + ";"); return list_size; }
extern "C" void ActionOccured__20VIWndGenericRendererPCc(void* self, const char* a) { note("A" + R(self) + "," + a + ";"); }
extern "C" const char* GetName__9VIGUIPage(void* p) { note("N" + R(p) + ";"); return page_name; }
extern "C" long long GetTime__7VITimer() { note("T;"); return time_now; }
extern "C" void* GetObjectByName__9VIGUIPagePCc(void* p, const char* name) { note("GO" + R(p) + "," + name + ";"); return object_present ? hw::at(object_g) : nullptr; }
extern "C" void __tf13VIGUIComboBox() {}
extern "C" void __tf11VIGUIObject() {}
extern "C" void __dynamic_cast_2(void* function, const void* target, int offset, void* adjusted, const void* source, void* object) {
    note(std::string("DC") + (function == hw::at(combo_entry_g) ? "f" : "?") + (target == reinterpret_cast<const void*>(__tf13VIGUIComboBox) ? "c" : "?") + N(offset) + "," + R(adjusted) +
         (source == reinterpret_cast<const void*>(__tf11VIGUIObject) ? "o" : "?") + "," + R(object) + ";");
}
extern "C" const unsigned short* GetSessionId__16VIRealmInterface(void* realm) { note("SID" + R(realm) + ";"); return session_text; }
extern "C" unsigned char* clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep(void* rep) {
    note("CU;");
    unsigned char* r = static_cast<unsigned char*>(rep);
    return new_rep(*reinterpret_cast<unsigned*>(r), 2);
}
extern "C" unsigned char* clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep(void* rep) {
    note("CC;");
    unsigned char* r = static_cast<unsigned char*>(rep);
    const unsigned length = *reinterpret_cast<unsigned*>(r);
    unsigned char* chars = new_rep(length, 1);
    std::memcpy(chars, r + 16, length);
    return chars;
}
extern "C" void replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi(void* string, unsigned position, unsigned count, const unsigned short* source, unsigned length) {
    note("RU" + N(position) + "," + N(count) + "," + N(length) + ";");
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned char* old = *handle;
    unsigned char* chars = new_rep(length, 2);
    std::memcpy(chars, source, length * 2);
    *handle = chars;
    dispose(old);
}
extern "C" void resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic(void* string, unsigned length, char fill) {
    note("RS" + N(length) + "," + N(fill) + ";");
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned char* old = *handle;
    unsigned char* chars = new_rep(length, 1);
    if (resize_shared) { field(chars, 8) = 2; reps.back().extra_owner = true; }  // still shared with an owner outside this routine
    *handle = chars;
    dispose(old);
}
extern "C" void alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib(void* string, unsigned length, bool flag) {
    note("AL" + N(length) + "," + (flag ? "1" : "0") + ";");
    unsigned char** handle = static_cast<unsigned char**>(string);
    unsigned char* old = *handle;
    unsigned char* chars = new_rep(length, 1);
    std::memcpy(chars, old, length);
    *handle = chars;
    dispose(old);
}
extern "C" void __wrap_free(void* block) { note("WF;"); mark_freed(static_cast<unsigned char*>(block)); }
static std::string printed;
extern "C" int printf(const char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    std::string text = format;
    if (text.find("%s") != std::string::npos) {
        const char* s = __builtin_va_arg(args, const char*);
        printed = s;
        text += std::string("|") + s;
    }
    __builtin_va_end(args);
    note("PR:" + text + ";");
    return 0;
}
extern "C" void __Q212RealmService14MatchGameAttrs(void* a) { note("MC;"); (void)a; }
extern "C" void _$_Q212RealmService14MatchGameAttrs(void* a, int flags) { note("MD" + N(flags) + ";"); (void)a; }
extern "C" void matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(void* api, const unsigned short* session, short a, unsigned b, unsigned c, int d, const void*, bool e, unsigned f, unsigned g, void* h) {
    note("MG" + R(api) + "," + (session == session_text ? "s" : "?") + "," + N(a) + "," + N(b) + "," + N(c) + "," + N(d) + "," + (e ? "1" : "0") + "," + N(f) + "," + N(g) + "," + R(h) + ";");
}
extern "C" void logout__Q212RealmService15RealmGatewayAPIPCUsPv(void* api, const unsigned short* session, void* data) { note("LO" + R(api) + "," + (session == session_text ? "s" : "?") + "," + R(data) + ";"); }
extern "C" void clientLeaveGame__16VIRealmInterface(void* realm) { note("CL" + R(realm) + ";"); }
extern "C" void GoBack__20VIWndGenericRenderer(void* self) { note("GB" + R(self) + ";"); }
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    note(std::string("GS") + (hw::guest(table) == ui_g ? "" : "?") + N(id) + "," + N(arg) + ";");
    return strings[id];
}
extern "C" void Show__11VIGUIDialogPCUsPCc(void* d, const unsigned short* t, const char* action) {
    note("SH" + R(d) + "," + N(reinterpret_cast<unsigned short(*)[2]>(const_cast<unsigned short*>(t)) - strings) + "," + action + ";");
}
extern "C" void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* t, bool f) {
    note("ST" + R(d) + "," + N(reinterpret_cast<unsigned short(*)[2]>(const_cast<unsigned short*>(t)) - strings) + "," + (f ? "1" : "0") + ";");
}
static void page_down(void* self, int key) { note("PD" + R(self) + "," + N(key) + ";"); }
static void dialog_down(void* self, int key) { note("DD" + R(self) + "," + N(key) + ";"); }
static void wrong(void*, int) { note("WRONG;"); }
static unsigned& W(unsigned off) { return hw::w(renderer_g + off); }

int main() {
    bool ok = true;
    long scenarios = 0;
    const unsigned short empty_session[1] = {0};
    const unsigned short one_session[2] = {0x141, 0};
    const unsigned short long_session[6] = {'a', 0x142, 'c', 0xfff0, 'e', 0};
    static unsigned short sized[4][120];
    const int sized_lengths[4] = {54, 55, 110, 111};
    for (int i = 0; i < 4; ++i) { for (int c = 0; c < sized_lengths[i]; ++c) sized[i][c] = static_cast<unsigned short>(0x100 + 'a' + c % 26); sized[i][sized_lengths[i]] = 0; }
    const unsigned short* sessions[] = {empty_session, one_session, long_session, sized[0], sized[1], sized[2], sized[3]};
    const int keys[] = {0, 27, 19, 20, 18, 17, 13, 132, 134, 135, 136, 137, 138, 139, 140, 5, -1};
    const char* names[] = {"", "Xx", "FindGame", "GameServ", "SRSMain", "GameClnt", "SRSMainGameClnt", "FindGameGameServ", "FindGam", "SRSMai", "GameCln", "GameSer"};
    const std::string self_r = Rg(renderer_g), realm_r = Rg(renderer_g + 1880), edit_r = Rg(renderer_g + 1424), list_r = Rg(renderer_g + 40), page_r = Rg(page_g), dialog_r = Rg(dialog_g);
    for (int key_in : keys) for (int block : {0, 1}) for (int dialog_state : {0, 1, 2}) for (int editing : {0, 1}) for (int size : {1, 2}) for (const char* name : names)
    for (int has_page : {0, 1}) for (long long delta : {0LL, 4999LL, 5000LL, 123456789012LL}) for (int object : {0, 1}) for (const unsigned short* session : sessions)
    for (int nil_u : {0, 1}) for (int nil_c : {0, 1}) for (int shared : {0, 1}) {
        // Keep the case count reasonable: the string-heavy dimensions only matter for key 132 on a FindGame page.
        const bool string_path = (key_in == 132 || key_in == 135) && (std::strstr(name, "FindGame") || std::strstr(name, "SRSMain"));
        if (!string_path && (session != sessions[0] || nil_u || nil_c || shared)) continue;
        if (key_in != 132 && (delta != 0 || object)) continue;
        if (!(key_in == 132 || key_in == 135 || key_in == 138) && (size == 2 || std::strcmp(name, "Xx") == 0 || std::strcmp(name, "FindGameGameServ") == 0 || editing)) continue;
        if (std::strcmp(name, "FindGam") == 0 || std::strcmp(name, "SRSMai") == 0 || std::strcmp(name, "GameCln") == 0 || std::strcmp(name, "GameSer") == 0) { if (key_in != 132 && key_in != 135) continue; if (editing) continue; }
        hw::clear();
        std::memset(norrath_nil_rep_ushort, 0, 16); std::memset(norrath_nil_rep_char, 0, 16);
        const unsigned nil_count = 1u << 20;
        std::memcpy(norrath_nil_rep_ushort + 8, &nil_count, 4); std::memcpy(norrath_nil_rep_char + 8, &nil_count, 4);
        const unsigned flag_u = nil_u ? 1u : 0u, flag_c = nil_c ? 1u : 0u;
        std::memcpy(norrath_nil_rep_ushort + 12, &flag_u, 4); std::memcpy(norrath_nil_rep_char + 12, &flag_c, 4);
        std::memset(norrath_free_list, 0, sizeof norrath_free_list);
        reps.clear(); pool_used = 0; printed.clear();
        std::memset(strings, 0, sizeof strings);
        _5VIWnd$mUI = ui_g;
        hw::w(ui_g + 48) = 77;
        for (int slot = 0; slot < 8; ++slot) {
            hw::put_entry(table_page_g + 8 * slot, 5, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
            hw::put_entry(table_dialog_g + 8 * slot, 5, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
        }
        hw::put_entry(table_page_g + 24, 8, 3, reinterpret_cast<void*>(page_down));
        hw::put_entry(table_dialog_g + 24, -8, 3, reinterpret_cast<void*>(dialog_down));
        hw::w(page_g + 120) = table_page_g; hw::w(dialog_g + 120) = table_dialog_g;
        std::memcpy(hw::at(table_object_g), "\x10\x00\x00\x00", 4);  // adjustment 16 for the object's table
        hw::w(table_object_g + 4) = combo_entry_g;
        hw::w(object_g + 148) = table_object_g;
        W(1400) = block ? 2 : 0; W(52) = has_page ? page_g : 0; W(56) = dialog_state ? dialog_g : 0; W(1420) = editing ? 4 : 0;
        dialog_open = dialog_state == 2; list_size = size; edit_flag = editing;
        page_name = name; object_present = object; session_text = session;
        resize_shared = shared;
        lastRefreshTime = 1000;
        time_now = lastRefreshTime + delta;
        trace.clear();
        const long long last_before = lastRefreshTime;
        OnKeyDown__20VIWndGenericRendereri(hw::at(renderer_g), key_in);

        // Model.
        std::string want;
        auto add = [&](const std::string& s) { want += s; };
        int key = key_in;
        long long expect_last = last_before;
        std::string expect_print;
        bool expect_string_path = false;
        if (!block) {
            if (key == 27) key = 135;
            if (key == 19) key = 138; else if (key == 20) key = 139; else if (key == 18) key = 137; else if (key == 17) key = 136; else if (key == 13) key = 134;
            const std::string k = N(key);
            const std::string dialog_arg = dialog_state ? dialog_r : std::string("0");
            auto open_check = [&]() { add("O" + dialog_arg + ";"); return dialog_open != 0; };
            auto page_text = [&](const char* sub) { add("N" + (has_page ? page_r : std::string("0")) + ";"); return std::strstr(name, sub) != nullptr; };
            auto text_ids = [&](int id) { add("GS" + N(id) + ",77;"); };
            bool done = false;
            if (dialog_state && open_check()) { add("DD" + Rg(dialog_g - 8) + "," + k + ";"); done = true; }
            if (!done && editing) { add("E" + edit_r + "," + k + ";"); done = true; }
            if (!done) {
                add("Z" + list_r + ";");
                if (size == 1 && key == 135 && !open_check()) { add("A" + self_r + ",DisconnectConfirmed;"); done = true; }
            }
            if (!done && key == 132 && page_text("FindGame")) {
                add("T;");
                if (time_now - last_before < 5000) done = true;
                else {
                    expect_last = time_now;
                    add("GO" + (has_page ? page_r : std::string("0")) + ",GameName;");
                    if (object) add("DCfc0," + Rg(object_g + 16) + "o," + Rg(object_g) + ";");
                    add("SID" + realm_r + ";");
                    if (nil_u) add("CU;");
                    unsigned length = 0; while (session[length]) ++length;
                    add("RU0,4294967295," + N(length) + ";");
                    if (nil_c) add("CC;");
                    add("RS" + N(length) + ",0;");
                    std::string narrow;
                    for (unsigned i = 0; i < length; ++i) narrow += static_cast<char>(session[i] & 0xff);
                    if (shared && length > 0) add("AL" + N(length) + ",1;");
                    if (length > 0) add("CC;");
                    // Representations larger than 128 bytes (header, capacity and element size) go back through the allocator's free routine.
                    const bool big_narrow = length > 0 && length + 2 + 16 > 128;
                    if (big_narrow) add("WF;");
                    add("PR:Calling matchGame with id %s\n|" + narrow + ";");
                    if (big_narrow) add("WF;");
                    expect_print = narrow;
                    expect_string_path = true;
                    add("MC;SID" + realm_r + ";MG" + realm_r + ",s,2,0,0,-1,1,100,1024,0;MD2;");
                    if ((length + 2) * 2 + 16 > 128) add("WF;");
                    done = true;
                }
            }
            if (!done) {
                if (page_text("GameServ") && key == 135 && !open_check()) {
                    text_ids(1835); add("SH" + dialog_arg + ",1835,CancelSRSGame;"); text_ids(1836); add("ST" + dialog_arg + ",1836,0;");
                    done = true;
                }
            }
            if (!done) {
                if (page_text("SRSMain") && key == 135) add("SID" + realm_r + ";LO" + realm_r + ",s,0;");
                if (page_text("GameClnt") && key == 135) add("CL" + realm_r + ";GB" + self_r + ";");
                if (has_page) add("PD" + Rg(page_g + 8) + "," + k + ";");
            }
        }
        if (trace != want) {
            std::fputs(("MISMATCH key=" + N(key_in) + " block=" + N(block) + " dialog=" + N(dialog_state) + " edit=" + N(editing) + " size=" + N(size) + " name=" + name + " page=" + N(has_page) + " delta=" + N(delta) +
                        " object=" + N(object) + " nil_u=" + N(nil_u) + " nil_c=" + N(nil_c) + " shared=" + N(shared) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout);
            return 1;
        }
        if (lastRefreshTime != expect_last) ok = false;
        if (expect_string_path && printed != expect_print) ok = false;
        // Reference counts balance: the nil representations return to their starting count, and every representation made here is released.
        unsigned nu, nc;
        std::memcpy(&nu, norrath_nil_rep_ushort + 8, 4); std::memcpy(&nc, norrath_nil_rep_char + 8, 4);
        if (nu != nil_count || nc != nil_count) { ok = false; std::fputs("nil reference count unbalanced\n", stdout); }
        for (const Rep& r : reps) {
            unsigned count; std::memcpy(&count, r.base + 8, 4);
            if (count != (r.extra_owner ? 1u : 0u)) { ok = false; std::fputs("leaked representation\n", stdout); break; }
        }
        // Released representations go onto the free list of their size class (index ((bytes + 16 + 7) / 8) - 1, linked
        // through the first header word), unless they were released inside the library stubs.
        for (unsigned cls = 0; cls < 16; ++cls) {
            std::vector<unsigned char*> expected_members;
            for (const Rep& r : reps) {
                unsigned count, capacity;
                std::memcpy(&count, r.base + 8, 4); std::memcpy(&capacity, r.base + 4, 4);
                const unsigned size = capacity * r.elem + 16;
                if (count == 0 && !r.freed && ((size + 7) >> 3) - 1 == cls) expected_members.push_back(r.base);
            }
            size_t walked = 0;
            unsigned link = norrath_free_list[cls];
            while (link != 0 && walked <= expected_members.size()) {
                bool found = false;
                for (unsigned char* member : expected_members)
                    if (static_cast<unsigned>(reinterpret_cast<uintptr_t>(member)) == link) { found = true; std::memcpy(&link, member, 4); break; }
                if (!found) break;
                ++walked;
            }
            if (walked != expected_members.size() || link != 0) { ok = false; std::fputs("free list mismatch\n", stdout); }
        }
        ++scenarios;
        if (!ok) { std::fputs("state mismatch\n", stdout); return 1; }
    }
    std::puts(("ok (" + std::to_string(scenarios) + " scenarios)").c_str());
    return 0;
}
