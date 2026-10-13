// Host check for fontStringSize. Not an EE plan input.
#include "font_string_size.cpp"

static void* seen_font;
static const char* seen_text;
static int seen_count;
static int calls;
static int result = 77;

extern "C" int _fontStringSize__FP4FontPci(void* font, const char* text, int count) {
    seen_font = font;
    seen_text = text;
    seen_count = count;
    ++calls;
    return result;
}

int main() {
    int font_object = 0;
    const char text[] = "abc";
    if (fontStringSize__FP4FontPci(&font_object, text, 3) != 77) return 1;
    if (calls != 1 || seen_font != &font_object || seen_text != text || seen_count != 3) return 2;
    result = -5;
    if (fontStringSize__FP4FontPci(nullptr, nullptr, 100000) != -5) return 3;
    if (calls != 2 || seen_font != nullptr || seen_text != nullptr || seen_count != 100000) return 4;
    return 0;
}
