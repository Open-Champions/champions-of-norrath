#include "storage.hpp"
// Observed curDlg at 0x00486b80 in .sbss. DLG_Active loads one word. The ELF size field is 0; dlgPos begins four bytes later, so the extent is inferred as four bytes. Startup zero-fill is the initial value.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_dialog_active_pointer_word_view[4] NORRATH_LOADED(".sbss") = {
    0x00, 0x00, 0x00, 0x00
};
}
