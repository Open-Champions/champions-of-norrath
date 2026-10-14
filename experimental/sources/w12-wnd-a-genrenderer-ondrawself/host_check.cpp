// Host check only; not a build input. The renderer, UI word, application block, page and dialog live in the
// arena. Callees are logged; the expected log comes from a separate model of the step list in the source
// comment, over the child-removal, page-only, post-init, error, page, render-counter, EULA-state, dialog and
// network-result combinations. printf is the logged callee here, so this check reports through puts only.
#define NORRATH_HOST_ARENA 1
#include "ondrawself.cpp"
#include "../w12-wnd-a-genrenderer-locals/render_count.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

unsigned _5VIWnd$mUI;
int mcerror;
static const unsigned renderer_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x8000, app_g = hw::BASE + 0x10000, page_g = hw::BASE + 0x6000,
                      dialog_g = hw::BASE + 0x6400, child_g = hw::BASE + 0x6800, holder_g = hw::BASE + 0x6c00;
static std::string trace;
static bool post_init_sets;
static int dialog_open, network_result;
static void* status_seen;
static unsigned short strings[4000][1];
static std::string rel(const void* p) { return p ? std::to_string(hw::guest(p) - hw::BASE) : "0"; }

extern "C" void* GetAppPtr__5VIWnd(void* self) { trace += "A" + rel(self) + ";"; return hw::at(app_g); }
extern "C" int printf(const char* format, ...) { trace += std::string("printf:") + format + ";"; return 0; }
extern "C" void RemoveChild__5VIWndP5VIWnd(void* self, void* child) { trace += "X" + rel(self) + "," + rel(child) + ";"; }
extern "C" void Render__9VIGUIPage(void* p) { trace += "RP" + rel(p) + ";"; }
extern "C" void PostInit__20VIWndGenericRenderer(void* self) {
    trace += "I" + rel(self) + ";";
    if (post_init_sets) hw::w(hw::guest(self) + 1404) = 1;
}
extern "C" int IsOpen__11VIGUIDialog(void* d) { trace += "O" + rel(d) + ";"; return dialog_open; }
extern "C" void Render__11VIGUIDialog(void* d) { trace += "RD" + rel(d) + ";"; }
extern "C" void StopEula__9VIStation(void* station) { trace += "Z" + std::to_string(hw::guest(station) - app_g) + ";"; }
extern "C" void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) {
    trace += "F" + std::to_string(hw::guest(self) - app_g) + "," + rel(window) + "," + (force ? "1" : "0") + ";";
}
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    trace += std::string("G") + (hw::guest(table) == ui_g ? "" : "?") + std::to_string(id) + "," + std::to_string(arg) + ";";
    return strings[id];
}
static std::string id_of(const unsigned short* t) { return std::to_string(reinterpret_cast<unsigned short(*)[1]>(const_cast<unsigned short*>(t)) - strings); }
extern "C" void SetText__11VIGUIDialogPCUsb(void* d, const unsigned short* text, bool flag) { trace += "ST" + rel(d) + "," + id_of(text) + "," + (flag ? "1" : "0") + ";"; }
extern "C" void Show__11VIGUIDialogPCUsPCc(void* d, const unsigned short* text, const char* action) { trace += "SH" + rel(d) + "," + id_of(text) + "," + (action ? action : "0") + ";"; }
extern "C" void __15VINetworkStatus(void* s) { status_seen = s; trace += "NC;"; }
extern "C" void Initialize__15VINetworkStatus(void* s) { trace += std::string("NI") + (s == status_seen ? "" : "?") + ";"; }
extern "C" int CheckNetwork__15VINetworkStatus(void* s) { trace += std::string("NK") + (s == status_seen ? "" : "?") + ";"; return network_result; }
extern "C" void process__16VIRealmInterface(void* realm) { trace += "PR" + rel(realm) + ";"; }
extern "C" void _$_15VINetworkStatus(void* s, int flags) { trace += std::string("ND") + (s == status_seen ? "" : "?") + std::to_string(flags) + ";"; }
static unsigned& W(unsigned off) { return hw::w(renderer_g + off); }

int main() {
    bool ok = true;
    long scenarios = 0;
    auto R = [](unsigned g) { return std::to_string(g - hw::BASE); };
    for (int child = 0; child < 2; ++child) for (int w1088 = 0; w1088 < 2; ++w1088) for (int w1404 = 0; w1404 < 2; ++w1404)
    for (int sets = 0; sets < 2; ++sets) for (int err : {0, 1, 2}) for (int has_page = 0; has_page < 2; ++has_page)
    for (int count : {0, 5, 999, 1000}) for (int eula : {0, 1}) for (int dialog_state : {0, 1, 2}) for (int net : {0, 1}) {
        hw::clear();
        _5VIWnd$mUI = ui_g;
        mcerror = err;
        norrath_generic_renderer_render_count = count;
        hw::w(ui_g + 48) = 900;
        W(1412) = child ? child_g : 0; W(1088) = w1088; W(1404) = w1404;
        W(52) = has_page ? page_g : 0;
        W(56) = dialog_state ? dialog_g : 0;
        W(36) = holder_g; hw::w(holder_g + 28) = 0x99;
        hw::w(app_g + 0xb4c8) = static_cast<unsigned>(eula);
        post_init_sets = sets;
        dialog_open = dialog_state == 2;
        network_result = net;
        status_seen = nullptr;
        unsigned char before[0x8000];
        std::memcpy(before, hw::at(hw::BASE), sizeof before);
        trace.clear();
        OnDrawSelf__20VIWndGenericRenderer(hw::at(renderer_g));

        std::string want = "A" + R(renderer_g) + ";";
        const std::string dialog = dialog_state ? R(dialog_g) : "0";
        auto dialog_step = [&]() { if (dialog_state) { want += "O" + dialog + ";"; if (dialog_state == 2) want += "RD" + dialog + ";"; } };
        auto stop = [&]() { want += "Z0;F" + std::to_string(0xb798) + "," + R(renderer_g) + ",1;"; };
        int expect_count = count;
        bool holder_cleared = false;
        bool wrote_app_b81c = false, cleared_child = false;
        if (child) { want += "printf:!!!Removing the child\n;X" + R(renderer_g) + "," + R(child_g) + ";"; cleared_child = true; }
        else if (w1088) { want += "RP" + (has_page ? R(page_g) : std::string("0")) + ";"; dialog_step(); }
        else {
            int flag1404 = w1404;
            if (!flag1404) { want += "I" + R(renderer_g) + ";"; flag1404 = sets; }
            if (flag1404) {
                if (err == 1) dialog_step();
                else if (has_page) {
                    if (expect_count < 1000) ++expect_count;
                    want += "RP" + R(page_g) + ";";
                    if (!eula) {
                        stop(); wrote_app_b81c = true; holder_cleared = true;
                        want += "G1862,900;ST" + dialog + ",1862,0;G1863,900;SH" + dialog + ",1863,0;";
                    }
                    dialog_step();
                    want += "NC;NI;NK;";
                    if (net) {
                        stop(); wrote_app_b81c = true; holder_cleared = true;
                        want += "printf:Network disconnected\n;G1395,900;ST" + dialog + ",1395,0;G1306,900;SH" + dialog + ",1306,DisconnectConfirmed;";
                    } else want += "PR" + std::to_string(1880 + (renderer_g - hw::BASE)) + ";";
                    want += "ND2;";
                }
            }
        }
        if (trace != want) {
            std::fputs(("MISMATCH child=" + std::to_string(child) + " w1088=" + std::to_string(w1088) + " w1404=" + std::to_string(w1404) + " sets=" + std::to_string(sets) + " err=" + std::to_string(err) + " page=" + std::to_string(has_page) + " count=" + std::to_string(count) + " eula=" + std::to_string(eula) + " dialog=" + std::to_string(dialog_state) + " net=" + std::to_string(net) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout);
            return 1;
        }
        if (norrath_generic_renderer_render_count != expect_count) ok = false;
        if (W(1412) != (cleared_child ? 0u : (child ? child_g : 0u))) ok = false;
        if (hw::w(holder_g + 28) != (holder_cleared ? 0u : 0x99u)) ok = false;
        const unsigned expect_b81c = wrote_app_b81c ? static_cast<unsigned>(reinterpret_cast<uintptr_t>(hw::at(renderer_g))) : 0u;
        if (hw::w(app_g + 0xb81c) != expect_b81c) ok = false;
        for (unsigned i = 0; i < sizeof before; ++i) {
            const unsigned absolute = hw::BASE + i;
            const bool known = (absolute >= renderer_g + 1412 && absolute < renderer_g + 1416) || (absolute >= renderer_g + 1404 && absolute < renderer_g + 1408) ||
                               (absolute >= holder_g + 28 && absolute < holder_g + 32) || (absolute >= app_g + 0xb81c && absolute < app_g + 0xb820);
            if (!known && hw::at(absolute)[0] != before[i]) ok = false;
        }
        ++scenarios;
        if (!ok) { std::fputs("state mismatch\n", stdout); return 1; }
    }
    std::puts(("ok (" + std::to_string(scenarios) + " scenarios)").c_str());
    return 0;
}
