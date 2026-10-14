#include "../w9-animation-object-c-common/mem.hpp"
// Observed onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv at 0x01109318, 124 bytes. With (this, a, b, c, ...) where the
// remaining arguments are never read:
//  1. printf("VIRealmInterface::onGetRealmStats(%i, %i)\n", a, b) (the first two arguments pass straight through).
//  2. Only when b == 0: sprintf(buf, "%i", c) into a stack buffer, then SetStringValue__9VIGUIPagePCcT1b(page, "PlayersConnected",
//     buf, false) where page = the word at (the word at this+12) + 52.
// Inferred: b is a status where 0 means success, c is the number of players connected, and the word at this+12 (the renderer the
// constructor stores) holds a pointer to a GUI page at +52. The buffer size is not visible from the code (it is only the frame).
// Unknown: the buffer length; the meaning of the eight trailing unsigned arguments; the type behind this+12.
extern "C" {
int printf(const char* format, ...);
int sprintf(char* out, const char* format, ...);
void SetStringValue__9VIGUIPagePCcT1b(void* page, const char* name, const char* value, int flag);

void onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv(void* self, unsigned a, int b, unsigned c, unsigned d, unsigned e,
                                                              unsigned f, unsigned g, unsigned h, unsigned i, unsigned j,
                                                              void* k) {
    using namespace norrath_aoc;
    (void)d; (void)e; (void)f; (void)g; (void)h; (void)i; (void)j; (void)k;
    unsigned char* object = static_cast<unsigned char*>(self);
    char buffer[32];
    printf("VIRealmInterface::onGetRealmStats(%i, %i)\n", a, b);
    if (b == 0) {
        sprintf(buffer, "%i", c);
        unsigned char* page = load_pointer(load_pointer(object + 12) + 52);
        SetStringValue__9VIGUIPagePCcT1b(page, "PlayersConnected", buffer, 0);
    }
}
}
