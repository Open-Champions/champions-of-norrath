// Host contract for resetTextBody__Fv. Not an EE plan input.
extern "C" unsigned short textBody[1024];
extern "C" int textBodyLen;
unsigned short textBody[1024];
int textBodyLen;
#include "reset_text_body.cpp"
int main() {
    for (int i = 0; i < 1024; ++i) textBody[i] = 0xffff;
    textBodyLen = 17;
    resetTextBody__Fv();
    if (textBodyLen != 0) return 1;
    for (int i = 0; i < 1024; ++i) if (textBody[i] != 0) return 2;
    return 0;
}
