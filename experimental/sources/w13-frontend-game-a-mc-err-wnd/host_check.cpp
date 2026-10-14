// Host check for McErrWnd__Fv. Not an EE plan input.
#include "mc_err_wnd.cpp"
extern "C" unsigned char sWndMcErr[0x210] = {};
int main() {
    if (McErrWnd__Fv() != static_cast<void*>(sWndMcErr)) return 1;
    if (McErrWnd__Fv() == static_cast<void*>(sWndMcErr + 1)) return 2;
    return 0;
}
