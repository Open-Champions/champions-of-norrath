// Host check only; not a build input. The renderer, page and dialog live in the arena with target-layout
// class tables (8-byte entries; the key-release handler is the entry at byte offset 16 of the table word at
// object+120). The expected call is derived from the documented decision order for every combination of the
// block word, page, dialog presence/open state and edit-active word.
#define NORRATH_HOST_ARENA 1
#include "onkeyup.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

static const unsigned renderer = hw::BASE + 0x100, page = hw::BASE + 0x2000, dialog = hw::BASE + 0x3000, table_page = hw::BASE + 0x4000, table_dialog = hw::BASE + 0x4100;
static std::string trace;
static std::string rel(const void* p) { return std::to_string(hw::guest(p) - hw::BASE); }
extern "C" int IsOpen__11VIGUIDialog(void* d) { trace += "O" + rel(d) + ";"; return hw::w(hw::guest(d) + 1000) ; }
extern "C" void OnKeyUp__9VIWndEditi(void* edit, int key) { trace += "E" + rel(edit) + "," + std::to_string(key) + ";"; }
static void page_up(void* self, int key) { trace += "P" + rel(self) + "," + std::to_string(key) + ";"; }
static void dialog_up(void* self, int key) { trace += "D" + rel(self) + "," + std::to_string(key) + ";"; }
static void wrong(void*, int) { trace += "WRONG;"; }

int main() {
    bool ok = true;
    for (int block : {0, 1}) for (int has_page : {0, 1}) for (int has_dialog : {0, 1}) for (int open : {0, 1}) for (int editing : {0, 1}) for (int key : {-1, 0, 19, 135, 1000}) {
        hw::clear();
        for (int slot = 0; slot < 6; ++slot) {
            hw::put_entry(table_page + 8 * slot, 5, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
            hw::put_entry(table_dialog + 8 * slot, 5, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
        }
        hw::put_entry(table_page + 16, 12, 2, reinterpret_cast<void*>(page_up));
        hw::put_entry(table_dialog + 16, -4, 2, reinterpret_cast<void*>(dialog_up));
        hw::w(page + 120) = table_page;
        hw::w(dialog + 120) = table_dialog;
        hw::w(dialog + 1000) = static_cast<unsigned>(open);
        hw::w(renderer + 1400) = block ? 3 : 0;
        hw::w(renderer + 52) = has_page ? page : 0;
        hw::w(renderer + 56) = has_dialog ? dialog : 0;
        hw::w(renderer + 1420) = editing ? 5 : 0;
        trace.clear();
        OnKeyUp__20VIWndGenericRendereri(hw::at(renderer), key);
        std::string want;
        const std::string k = std::to_string(key);
        if (!block && has_page) {
            if (has_dialog && open) want = "O" + std::to_string(dialog - hw::BASE) + ";D" + std::to_string(dialog - hw::BASE - 4) + "," + k + ";";
            else {
                if (has_dialog) want = "O" + std::to_string(dialog - hw::BASE) + ";";
                if (editing) want += "E" + std::to_string(renderer + 1424 - hw::BASE) + "," + k + ";";
                else want += "P" + std::to_string(page - hw::BASE + 12) + "," + k + ";";
            }
        }
        if (trace != want) { std::printf("block %d page %d dialog %d open %d editing %d key %d got '%s' want '%s'\n", block, has_page, has_dialog, open, editing, key, trace.c_str(), want.c_str()); ok = false; break; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
