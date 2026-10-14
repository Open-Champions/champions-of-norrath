// Observed findAndStripGender__4itemPUs at 0x001017d8, 556 bytes (a 16-bit text
// string, no `this`). n = ustrlen__FPUs(text); `language` is the signed byte at
// 0x004844b4. Letters are compared in both cases. Only two languages do anything:
//   * language 4: when text[n-2] is '_' the final letter text[n-1] selects a result:
//     m -> 1, f -> 2, n -> 3, p -> 4, anything else -> 0; in every one of those
//     cases (also "anything else") text[n-2] is overwritten with 0, cutting the
//     suffix off. No '_' there: 0 and the text is left alone.
//   * language 3: when text[n-3] is '_' the two letters text[n-2], text[n-1] select
//     a result: "ms" -> 1, "fs" -> 2, "mp" -> 3, "fp" -> 4, anything else -> 0; in
//     all of those cases text[n-3] is overwritten with 0. No '_' there: -1 and the
//     text is left alone.
//   * any other language: 0, text untouched.
// Lengths shorter than the suffix are not checked (the image reads before the
// start). Unknown: which languages the numbers 3 and 4 are, and what the result
// codes mean to the caller beyond the gender code of the item name.
extern "C" {
extern signed char language;
int ustrlen__FPUs(unsigned short*);

int findAndStripGender__4itemPUs(unsigned short* text) {
    int n = ustrlen__FPUs(text);
    if (language == 4) {
        unsigned short* underscore = text + n - 2;
        if (*underscore != '_') return 0;
        unsigned short c = text[n - 1];
        *underscore = 0;
        if (c == 'm' || c == 'M') return 1;
        if (c == 'f' || c == 'F') return 2;
        if (c == 'n' || c == 'N') return 3;
        if (c == 'p' || c == 'P') return 4;
        return 0;
    }
    if (language == 3) {
        unsigned short* underscore = text + n - 3;
        if (*underscore != '_') return -1;
        unsigned short a = text[n - 2];
        unsigned short b = text[n - 1];
        int result = 0;
        if ((a == 'm' || a == 'M') && (b == 's' || b == 'S')) result = 1;
        else if ((a == 'f' || a == 'F') && (b == 's' || b == 'S')) result = 2;
        else if ((a == 'm' || a == 'M') && (b == 'p' || b == 'P')) result = 3;
        else if ((a == 'f' || a == 'F') && (b == 'p' || b == 'P')) result = 4;
        *underscore = 0;
        return result;
    }
    return 0;
}
}
