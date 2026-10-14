#include "../w9-animation-object-c-common/mem.hpp"
// Observed onCreatePublicGame__16VIRealmInterfaceUiiPCciPv at 0x01107348, 564 bytes (a 4400-byte frame: a 4096-byte wide-character
// buffer, then a 256-byte narrow buffer). With (this, a, status, host, port, extra); offsets are in this:
//  1. printf("VIRealmInterface::onCreatePublicGame(%i, %i)\n", a, status); memset(this+5088, 0, 4576).
//  2. status == 0: snprintf16__8VIStringPUsiPCUse(wide, 4095, string 1848, this+276) (the length bound passed is 4095 although the wide buffer
//     holds 2048 units); SetText__11VIGUIDialogPCUsb(dialog, wide, 0) with dialog = the word at (the word at this+12) + 56;
//     the word at +2324 = port; when host is not null, as in onDoClientDiscovery: strcmp(host, "sdrealm-test"), then snprintf(this+1300,
//     1023, "sdrealm-test.station.sony.com") on a match or snprintf(this+1300, 1023, host) otherwise. Then the word at +3468 = 1;
//     AddPage__20VIWndGenericRendererPCc(the word at this+12, "data/ui/GameServ.xml"); strcpy(narrow, this+276); strstr(narrow, "[")
//     and, when found, a zero byte is written over the match; SetStringValue__9VIGUIPagePCcT1b(the word at (the word at this+12)+52,
//     "GameName", narrow, 0); UpdateNumPlayers__16VIRealmInterfacei(this, 1).
//  3. status 36: snprintf16 with string 1847 and this+276 into the wide buffer, then SetText of the dialog with it.
//  4. status 40: SetText of the dialog with string 1849. Status 1: string 1861. Any other status: string 1850.
//  Strings: GetString__13VIStringTableii(the word _5VIWnd$mUI, id, the word at that pointer + 48).
// Inferred: this is the reply to a request to create a public game; this+276 holds the game name (it is cut at the first '[' for the page
// field "GameName"); the "-test" host is replaced by a fixed Sony name, as in the discovery reply.
// Unknown: the status meanings; what +3468 = 1 stands for; the origin of the +276 string; the extra argument (never read).
extern "C" {
extern unsigned vi_wnd_mui __asm__("_5VIWnd$mUI");
int printf(const char* format, ...);
void* memset(void* dest, int value, unsigned long size);
int strcmp(const char* a, const char* b);
int snprintf(char* out, unsigned long size, const char* format, ...);
char* strcpy(char* dest, const char* source);
char* strstr_call(const char* hay, const char* needle) __asm__("strstr");
int snprintf16__8VIStringPUsiPCUse(unsigned short* out, int count, const unsigned short* format, ...);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int third);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, int flag);
void SetStringValue__9VIGUIPagePCcT1b(void* page, const char* name, const char* value, int flag);
void AddPage__20VIWndGenericRendererPCc(void* renderer, const char* path);
void UpdateNumPlayers__16VIRealmInterfacei(void* self, int value);

static const unsigned short* realm_string(int id) {
    using namespace norrath_aoc;
    unsigned char* table = load_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui));
    return GetString__13VIStringTableii(table, id, static_cast<int>(load_word(table + 48)));
}

void onCreatePublicGame__16VIRealmInterfaceUiiPCciPv(void* self, unsigned a, int status, const char* host, int port, void* extra) {
    using namespace norrath_aoc;
    (void)extra;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned short wide[2048];
    char narrow[256];
    printf("VIRealmInterface::onCreatePublicGame(%i, %i)\n", a, status);
    memset(object + 5088, 0, 4576);
    if (status == 0) {
        snprintf16__8VIStringPUsiPCUse(wide, 4095, realm_string(1848), object + 276);
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), wide, 0);
        store_word(object + 2324, static_cast<unsigned>(port));
        if (host != 0) {
            if (strcmp(host, "sdrealm-test") == 0)
                snprintf(reinterpret_cast<char*>(object + 1300), 1023, "sdrealm-test.station.sony.com");
            else
                snprintf(reinterpret_cast<char*>(object + 1300), 1023, host);
        }
        store_word(object + 3468, 1);
        AddPage__20VIWndGenericRendererPCc(load_pointer(object + 12), "data/ui/GameServ.xml");
        strcpy(narrow, reinterpret_cast<const char*>(object + 276));
        char* bracket = strstr_call(narrow, "[");
        if (bracket != 0) *bracket = 0;
        SetStringValue__9VIGUIPagePCcT1b(load_pointer(load_pointer(object + 12) + 52), "GameName", narrow, 0);
        UpdateNumPlayers__16VIRealmInterfacei(self, 1);
    } else if (status == 36) {
        snprintf16__8VIStringPUsiPCUse(wide, 4095, realm_string(1847), object + 276);
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), wide, 0);
    } else if (status == 40) {
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1849), 0);
    } else if (status == 1) {
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1861), 0);
    } else {
        SetText__11VIGUIDialogPCUsb(load_pointer(load_pointer(object + 12) + 56), realm_string(1850), 0);
    }
}
}
