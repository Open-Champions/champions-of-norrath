#include "../w9-animation-object-c-common/mem.hpp"
// Observed notifyClientDiscovered__16VIRealmInterfacePCci at 0x01109000, 196 bytes, 48-byte frame. With (this, address,
// port), this being the VIRealmInterface object and ui the word _5VIWnd$mUI (used as in the message window: a string
// table whose column word is at ui + 48):
//  1. printf("VIRealmInterface::notifyClientDiscovered(%s, %i)\n", address, port).
//  2. strcpy(g_localIp, address); the word g_localPort = port.
//  3. The 64-bit value at this + 9720 = GetTime__7VITimer(); the word at this + 3468 = 11.
//  4. page = the word at (the pointer at this + 12) + 52; SetStringValue__9VIGUIPagePCcPCUsb(page, "ServerStatus",
//     GetString__13VIStringTableii(ui, 1856, the word at ui + 48), true).
//  5. GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(this + 3396, this + 38012, g_localIp, 1000).
// The original stores g_localPort in the delay slot of the GetTime call, and loads the page pointer before the
// GetString call; neither affects what the callees see. Unlike notifyGameDiscovered it does not copy the address into
// the object.
// Inferred: the callback of a client discovered over UDP; the object's word at +12 (the renderer passed to the
// constructor) has a page pointer at +52 which receives the text of string 1856 under the key "ServerStatus".
// Unknown: what string 1856 says, what state 11 means, the time-stamp's role at +9720, and the type behind +52.
extern "C" {
int printf(const char* format, ...);
char* strcpy(char* dest, const char* src);
unsigned long long GetTime__7VITimer();
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
void SetStringValue__9VIGUIPagePCcPCUsb(void* page, const char* key, const unsigned short* value, bool flag);
int GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(void* socket, void* address, const char* host, int port);
extern char g_localIp[24];
extern int g_localPort;
extern unsigned _5VIWnd$mUI;

void notifyClientDiscovered__16VIRealmInterfacePCci(void* self, const char* address, int port) {
    using namespace norrath_aoc;
    unsigned char* bytes = static_cast<unsigned char*>(self);
    printf("VIRealmInterface::notifyClientDiscovered(%s, %i)\n", address, port);
    strcpy(g_localIp, address);
    g_localPort = port;
    *reinterpret_cast<unsigned long long*>(bytes + 9720) = GetTime__7VITimer();
    store_word(bytes + 3468, 11);
    unsigned char* renderer = load_pointer(bytes + 12);
    void* page = load_pointer(renderer + 52);
    const unsigned char* ui = load_pointer(reinterpret_cast<const unsigned char*>(&_5VIWnd$mUI));
    const unsigned short* text = GetString__13VIStringTableii(ui, 1856, static_cast<int>(load_word(ui + 48)));
    SetStringValue__9VIGUIPagePCcPCUsb(page, "ServerStatus", text, true);
    GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(bytes + 3396, bytes + 38012, g_localIp, 1000);
}
}
