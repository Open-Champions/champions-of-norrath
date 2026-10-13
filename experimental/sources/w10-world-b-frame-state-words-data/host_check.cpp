// Host check for the four state words. Not an EE plan input.
#include <cstdio>
#include "frame_state_words_data.cpp"
int main() {
    if (framedone || inesc || lastCE || framestart) return 1;
    if (sizeof framedone != 4 || sizeof inesc != 4 || sizeof lastCE != 4 || sizeof framestart != 4) return 2;
    framedone = -1; if (framedone >= 0) return 3;
    std::puts("state words check passed");
    return 0;
}
