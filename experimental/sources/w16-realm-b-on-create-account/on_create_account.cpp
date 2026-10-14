#include "../w9-animation-object-c-common/mem.hpp"
// Observed onCreateAccount__16VIRealmInterfaceUiiPCUsPv at 0x01107120, 548 bytes. With (this, a, status, name, extra); offsets are
// in this:
//  1. printf("VIRealmInterface::onCreateAccount(%i, %i)\n", a, status); then UnLock__11VIGUIDialog(dialog), where dialog is the word
//     at (the word at this+12) + 56.
//  2. status == 0: SetText__11VIGUIDialogib(dialog, 1384, 0); strncpyWide(this+16, name, 256); the word at +272 =
//     strlenWide(this+16); app = GetAppPtr__5VIWnd(the word at this+12); when the first word of *app is 1,
//     AddPage__20VIWndGenericRendererPCc(the word at this+12, "data/ui/SRSMains.xml"), otherwise the same call with
//     "data/ui/SRSMainc.xml".
//  3. status 36: SetText__11VIGUIDialogib(dialog, 1371, 0). Status 37, 38, 39 and 1: SetText__11VIGUIDialogPCUsb(dialog, text, 0)
//     with text = GetString__13VIStringTableii(the word _5VIWnd$mUI, id, the word at that pointer + 48) and id 1844, 1845, 1846 and
//     1861 respectively. Any other status does nothing further.
// Inferred: a reply to an account-creation request; the copied wide string is an account name kept at +16 (256 units) with its
// length at +272. The renderer page that follows the success reply differs by a mode word read from the application object.
// Unknown: what each status code means (the negative, error-looking nature of 36 to 39 is a guess); the meaning of the first word of the
// application object; the string ids' text; the size of the name buffer beyond the 256 passed to the copy.
extern "C" {
extern unsigned vi_wnd_mui __asm__("_5VIWnd$mUI");
int printf(const char* format, ...);
void UnLock__11VIGUIDialog(void* dialog);
void SetText__11VIGUIDialogib(void* dialog, int id, int flag);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, int flag);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third);
void strncpyWide__FPUsPCUsUi(unsigned short* out, const unsigned short* in, unsigned count);
int strlenWide__FPCUs(const unsigned short* text);
void* GetAppPtr__5VIWnd(void* renderer);
void AddPage__20VIWndGenericRendererPCc(void* renderer, const char* path);

static void set_message(void* dialog, int id) {
    using namespace norrath_aoc;
    unsigned char* table = load_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui));
    SetText__11VIGUIDialogPCUsb(dialog, GetString__13VIStringTableii(table, id, static_cast<int>(load_word(table + 48))), 0);
}

void onCreateAccount__16VIRealmInterfaceUiiPCUsPv(void* self, unsigned a, int status, const unsigned short* name, void* extra) {
    using namespace norrath_aoc;
    (void)extra;
    unsigned char* object = static_cast<unsigned char*>(self);
    printf("VIRealmInterface::onCreateAccount(%i, %i)\n", a, status);
    UnLock__11VIGUIDialog(load_pointer(load_pointer(object + 12) + 56));
    if (status == 0) {
        SetText__11VIGUIDialogib(load_pointer(load_pointer(object + 12) + 56), 1384, 0);
        unsigned short* buffer = reinterpret_cast<unsigned short*>(object + 16);
        strncpyWide__FPUsPCUsUi(buffer, name, 256);
        store_word(object + 272, static_cast<unsigned>(strlenWide__FPCUs(buffer)));
        unsigned char* app = static_cast<unsigned char*>(GetAppPtr__5VIWnd(load_pointer(object + 12)));
        if (load_word(app) == 1)
            AddPage__20VIWndGenericRendererPCc(load_pointer(object + 12), "data/ui/SRSMains.xml");
        else
            AddPage__20VIWndGenericRendererPCc(load_pointer(object + 12), "data/ui/SRSMainc.xml");
    } else if (status == 36) {
        SetText__11VIGUIDialogib(load_pointer(load_pointer(object + 12) + 56), 1371, 0);
    } else if (status == 37) {
        set_message(load_pointer(load_pointer(object + 12) + 56), 1844);
    } else if (status == 38) {
        set_message(load_pointer(load_pointer(object + 12) + 56), 1845);
    } else if (status == 39) {
        set_message(load_pointer(load_pointer(object + 12) + 56), 1846);
    } else if (status == 1) {
        set_message(load_pointer(load_pointer(object + 12) + 56), 1861);
    }
}
}
