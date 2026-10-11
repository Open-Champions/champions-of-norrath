#include <cstring>

extern "C" unsigned boxTex;
extern "C" unsigned DLG_ShowDialogText;
extern "C" unsigned curDlg;

static int zero_word(unsigned* object) {
    const unsigned char want[4] = {};
    return std::memcmp(object, want, 4) == 0 ? 0 : 1;
}

int main() {
    if (zero_word(&boxTex) || zero_word(&DLG_ShowDialogText) || zero_word(&curDlg)) return 1;
    curDlg = 1u;
    if (curDlg != 1u || boxTex != 0u) return 2;
    curDlg = 0u;
    return 0;
}
