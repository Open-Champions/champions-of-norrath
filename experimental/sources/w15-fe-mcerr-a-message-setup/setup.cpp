// Observed: Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii at 0x010c1588 is 232 bytes. With (this,
// title, text, prompt, size, width), where title and text are 16-bit strings, prompt is the VIMESSAGEPROMPT
// enumeration passed as an int, size points to two ints and width is an int:
//  1. strcpy16__8VIStringPUsUiPCUs(this + 408, 64, title) and (this + 472, 512, text);
//  2. stores prompt at +1004, the two ints of *size at +988 and +992, and width at +996;
//  3. with a local pair of zeros pos, calls Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(this + 40, ui, pos,
//     this + 988, width, 20, 0) where ui is the word _5VIWnd$mUI (0x01192c70) as a pointer;
//  4. stores 0 at +984 and CountLines__10VIUIWindowPCUsi(this + 40, this + 472, width - 80) + 3 at +1000.
//     Nothing is returned.
// Inferred: this prepares a message box: title and body text, which buttons to offer, the size and a width
// to wrap the body at; the +1000 word is the line count of the body plus three rows of chrome; the width
// limit of 80 is the text margin. The local zero pair is the window position.
// Unknown: the numeric values of VIMESSAGEPROMPT, the meaning of 20, the final 0 of Init, the field names, and
// what the zero at +984 resets (it is stored after Init and CountLines have run).
extern "C" {
extern unsigned _5VIWnd$mUI;
void strcpy16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, void* ui, const int* position, const int* size, int width,
                                               int a, int b);
int CountLines__10VIUIWindowPCUsi(void* window, const unsigned short* text, int width);

void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* self, const unsigned short* title,
                                                             const unsigned short* text, int prompt, const int* size,
                                                             int width) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    strcpy16__8VIStringPUsUiPCUs(reinterpret_cast<unsigned short*>(bytes + 408), 64, title);
    strcpy16__8VIStringPUsUiPCUs(reinterpret_cast<unsigned short*>(bytes + 472), 512, text);
    words[251] = prompt;
    words[247] = size[0];
    words[248] = size[1];
    words[249] = width;
    int position[2] = {0, 0};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(bytes + 40, reinterpret_cast<void*>(static_cast<unsigned long>(_5VIWnd$mUI)),
                                             position, words + 247, width, 20, 0);
    const int lines = CountLines__10VIUIWindowPCUsi(bytes + 40, reinterpret_cast<unsigned short*>(bytes + 472), width - 80);
    words[246] = 0;
    words[250] = lines + 3;
}
}
