#include "../w9-animation-object-c-common/mem.hpp"
// Observed onCancelGame__16VIRealmInterfaceUiiPv at 0x01108980, 372 bytes, 32-byte frame. With (this, handle, status,
// context), the last not read, ui the word _5VIWnd$mUI used as a string table (column word at ui + 48) and the dialog
// being the word at (the pointer at this + 12) + 56, re-read from this + 12 before each use:
//  1. The word at this + 3468 = 0; Reset__16VIRealmInterface(this).
//  2. printf("VIRealmInterface::onCancelGame(%i, %i)\n", handle, status).
//  3. status == 0: UnLock__11VIGUIDialog(dialog); Show__11VIGUIDialogPCUsPCc(dialog, string 1840, 0);
//     then SetText__11VIGUIDialogPCUsb(dialog, string 1854, false) as the final call of the function.
//  4. status == 1: SetText(dialog, string 1861, false) only.
//  5. any other status: Show(dialog, string 1835, 0); then SetText(dialog, string 1855, false). No UnLock.
//  Each string is GetString__13VIStringTableii(ui, id, the word at ui + 48), with ui read again for every call.
// The original stores the zero state word in the delay slot of the Reset call, so the store lands before Reset runs;
// the source stores first, then calls.
// Inferred: this is the completion handler for cancelling a game on the server; status 0 reads as success and 1
// as a distinguished failure, but neither reading is established. The dialog is a VIGUIDialog by the callee names.
// Unknown: what each string says, what the handle identifies, and what the state word means.
extern "C" {
int printf(const char* format, ...);
void Reset__16VIRealmInterface(void* self);
void UnLock__11VIGUIDialog(void* dialog);
void Show__11VIGUIDialogPCUsPCc(void* dialog, const unsigned short* text, const char* extra);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, bool flag);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
extern unsigned _5VIWnd$mUI;
}

namespace {
using namespace norrath_aoc;
inline unsigned char* dialog_of(unsigned char* self) { return load_pointer(load_pointer(self + 12) + 56); }
inline const unsigned short* string_for(int id) {
    const unsigned char* ui = load_pointer(reinterpret_cast<const unsigned char*>(&_5VIWnd$mUI));
    return GetString__13VIStringTableii(ui, id, static_cast<int>(load_word(ui + 48)));
}
}

extern "C" void onCancelGame__16VIRealmInterfaceUiiPv(void* self, unsigned handle, int status, void* context) {
    (void)context;
    unsigned char* bytes = static_cast<unsigned char*>(self);
    store_word(bytes + 3468, 0);
    Reset__16VIRealmInterface(self);
    printf("VIRealmInterface::onCancelGame(%i, %i)\n", handle, status);
    if (status == 0) {
        UnLock__11VIGUIDialog(dialog_of(bytes));
        unsigned char* dialog = dialog_of(bytes);
        Show__11VIGUIDialogPCUsPCc(dialog, string_for(1840), nullptr);
        SetText__11VIGUIDialogPCUsb(dialog_of(bytes), string_for(1854), false);
    } else if (status == 1) {
        unsigned char* dialog = dialog_of(bytes);
        SetText__11VIGUIDialogPCUsb(dialog, string_for(1861), false);
    } else {
        unsigned char* dialog = dialog_of(bytes);
        Show__11VIGUIDialogPCUsPCc(dialog, string_for(1835), nullptr);
        SetText__11VIGUIDialogPCUsb(dialog_of(bytes), string_for(1855), false);
    }
}
