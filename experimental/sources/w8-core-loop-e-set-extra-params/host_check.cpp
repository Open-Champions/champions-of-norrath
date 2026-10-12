// Host check for setExtraParams__FP8CreaturePc. Not an EE plan input.
extern "C" {
int curDlg = 0;
void* dialogCreature = nullptr;
char* dialogLumpName = nullptr;
}
#include "set_extra_params.cpp"
int main() {
    char c = 0;
    char name[] = "x";
    setExtraParams__FP8CreaturePc(&c, name);
    if (dialogCreature != &c || dialogLumpName != name) return 1;
    curDlg = 7;
    setExtraParams__FP8CreaturePc(nullptr, nullptr);
    if (dialogCreature != &c || dialogLumpName != name) return 2;
    curDlg = 0;
    setExtraParams__FP8CreaturePc(nullptr, nullptr);
    if (dialogCreature != nullptr || dialogLumpName != nullptr) return 3;
    return 0;
}
