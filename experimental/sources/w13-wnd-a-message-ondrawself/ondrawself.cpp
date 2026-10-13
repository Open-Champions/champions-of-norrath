// Observed: OnDrawSelf__12VIWndMessage at 0x010c0f20 is 628 bytes. With (this), ui being the word _5VIWnd$mUI
// (0x01192c70) and w = this + 40 (an embedded UI window) it runs: Begin2D__4VIUI(ui); Draw__10VIUIWindow(w); then
// TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w, this + 472, 40, 1, width - 80, (word at +1000) - 2, 0, 8) where
// width is the word at w + 20 (= this + 60). With third = width / 3 (signed) and the row b = (word at +1000) - 1,
// the word at +1004 selects what follows:
//  - 0: TextBox(w, PromptText__12VIWndMessagePCUsi(this, GetString__13VIStringTableii(ui, 786, the word at ui + 48),
//    1), 0, b, width, 1, 0, 8);
//  - 1: two prompts. TextBox(w, PromptText(this, GetString(ui, 786, col), word at +984 == 0), third - 100, b,
//    200, 1, 0, 8), then TextBox(w, PromptText(this, GetString(ui, 753, col), (word at +984 ^ 1) == 0),
//    2 * third - 100, b, 200, 1, 0, 8);
//  - 2: the same two prompts with the texts 21 and 22 in place of 786 and 753;
//  - any other value: nothing more.
// Then End2D__4VIUI(ui). The word at ui + 48 is re-read for every GetString call; ui is read once.
// Inferred: this window draws a message and one or two buttons; the word at +984 is the highlighted button
// (set to 0 or 1 by OnMessage and the key handler) and +1004 the layout; the TextBox integer arguments are
// positions and sizes (the order is only the register order).
// Unknown: the meaning of the TextBox arguments and of 8 and 0; the names of the fields; the return type (void).
extern "C" {
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void Draw__10VIUIWindow(void* window);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
const unsigned short* PromptText__12VIWndMessagePCUsi(void* self, const unsigned short* text, int flag);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
extern unsigned _5VIWnd$mUI;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline const unsigned short* string_for(int id) {
    const void* table = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(table, id, static_cast<const int*>(table)[12]);
}

void OnDrawSelf__12VIWndMessage(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    void* ui = pointer_of(_5VIWnd$mUI);
    void* window = bytes + 40;
    Begin2D__4VIUI(ui);
    const int first = words[15] / 3;
    const int second = first * 2;
    Draw__10VIUIWindow(window);
    const int width = words[15];
    TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, reinterpret_cast<const unsigned short*>(bytes + 472), 40, 1,
                                              width - 80, words[250] - 2, 0, 8);
    switch (words[251]) {
    case 0: {
        const unsigned short* text = PromptText__12VIWndMessagePCUsi(self, string_for(786), 1);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, text, 0, words[250] - 1, words[15], 1, 0, 8);
        break;
    }
    case 1:
    case 2: {
        const int id_first = words[251] == 1 ? 786 : 21;
        const int id_second = words[251] == 1 ? 753 : 22;
        const unsigned short* a = PromptText__12VIWndMessagePCUsi(self, string_for(id_first), words[246] == 0);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, a, first - 100, words[250] - 1, 200, 1, 0, 8);
        const unsigned short* b = PromptText__12VIWndMessagePCUsi(self, string_for(id_second), (words[246] ^ 1) == 0);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, b, second - 100, words[250] - 1, 200, 1, 0, 8);
        break;
    }
    default:
        break;
    }
    End2D__4VIUI(ui);
}
}
