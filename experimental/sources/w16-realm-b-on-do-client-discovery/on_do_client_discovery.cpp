#include "../w9-animation-object-c-common/mem.hpp"
// Observed onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv at 0x01108af8, 356 bytes. With (this, a, status, host, port, extra);
// word and byte offsets are in this:
//  1. printf("VIRealmInterface::onDoClientDiscovery(%i, %i)\n", a, status).
//  2. status == 0: the word at +2324 = port (the fifth argument, stored before the host test). When host is not null,
//     strcmp(host, "sdrealm-test"); on a match snprintf(this+1300, 1023, "sdrealm-test.station.sony.com"), otherwise
//     snprintf(this+1300, 1023, host) (the host string itself is the format). A null host leaves +1300 untouched.
//     In every status == 0 case the word at +3468 is then set to 5.
//  3. status == 1: SetText__11VIGUIDialogPCUsb(dialog, GetString(mUI, 1861, mUI's word at +48), 0), where dialog is the word at
//     (the word at this+12) + 56, then return.
//  4. any other status: GoBack__20VIWndGenericRenderer(the word at this+12); Show__11VIGUIDialogPCUsPCc(dialog, string 1840, 0);
//     SetText__11VIGUIDialogPCUsb(dialog, string 1841, 0); the word at +3468 = 0; Reset__16VIRealmInterface(this).
//  Strings come from GetString__13VIStringTableii(the word _5VIWnd$mUI, id, the word at that pointer + 48); the dialog word is re-read
//  from the renderer before each use.
// Inferred: status 0 is success of the discovery step, and the 5 in +3468 is a state number of a connection sequence (0 after a failure
// and a reset). The extra argument is never read. A host named "sdrealm-test" is replaced by a longer fixed name.
// Unknown: what the state value 5 stands for; the type of mUI beyond a table-like object whose +48 word is passed as the
// language-like third argument; the meaning of the port-like word at +2324; the meaning of the string ids.
extern "C" {
extern unsigned vi_wnd_mui __asm__("_5VIWnd$mUI");
int printf(const char* format, ...);
int strcmp(const char* a, const char* b);
int snprintf_call(char* out, unsigned size, const char* format, ...) __asm__("snprintf");
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, int flag);
void Show__11VIGUIDialogPCUsPCc(void* dialog, const unsigned short* text, const char* extra);
void GoBack__20VIWndGenericRenderer(void* renderer);
void Reset__16VIRealmInterface(void* self);

static const unsigned short* realm_string(int id) {
    using namespace norrath_aoc;
    unsigned char* table = load_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui));
    return GetString__13VIStringTableii(table, id, static_cast<int>(load_word(table + 48)));
}

void onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv(void* self, unsigned a, int status, const char* host, int port, void* extra) {
    using namespace norrath_aoc;
    (void)extra;
    unsigned char* object = static_cast<unsigned char*>(self);
    printf("VIRealmInterface::onDoClientDiscovery(%i, %i)\n", a, status);
    if (status == 0) {
        store_word(object + 2324, static_cast<unsigned>(port));
        if (host != 0) {
            if (strcmp(host, "sdrealm-test") == 0)
                snprintf_call(reinterpret_cast<char*>(object + 1300), 1023, "sdrealm-test.station.sony.com");
            else
                snprintf_call(reinterpret_cast<char*>(object + 1300), 1023, host);
        }
        store_word(object + 3468, 5);
    } else if (status == 1) {
        void* dialog = load_pointer(load_pointer(object + 12) + 56);
        SetText__11VIGUIDialogPCUsb(dialog, realm_string(1861), 0);
    } else {
        GoBack__20VIWndGenericRenderer(load_pointer(object + 12));
        Show__11VIGUIDialogPCUsPCc(load_pointer(load_pointer(object + 12) + 56), realm_string(1840), 0);
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1841), 0);
        store_word(object + 3468, 0);
        Reset__16VIRealmInterface(self);
    }
}
}
