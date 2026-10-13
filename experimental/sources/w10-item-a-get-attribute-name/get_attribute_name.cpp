// Observed getAttributeName__4itemiPc at 0x00101a08, 548 bytes (int index, char*
// key); returns a 16-bit text pointer. Uses two 256-byte local text buffers, A and
// B. `language` is the signed byte at 0x004844b4 and undEnable is the word
// _4item$undEnable (0x00483220).
//   1. A = usprintf of the text textGetString__FPc(key) with eight 0.0f arguments.
//   2. index == -1: B = usprintf of A with eight 0.0f arguments, treated as a plain
//      result (no bar handling).
//   3. Otherwise the text A is searched for '|' (124). Without one, B = copy of A.
//      With one, B receives the index-th '|'-separated piece of A (index bars are
//      skipped first; the piece ends at the next '|' or the end of the text), and
//      when language is 3 the character three positions before the end of B is
//      cleared (shortening it by at most three characters; no length check).
//   4. If undEnable is nonzero and B begins 'd','e' at positions 0 and 1 with a
//      space at position 3: when undEnable is 2, a static buffer (und.1075,
//      0x00486e00) is made as ustrcpy__FPUsPc(buffer, "und ") followed by
//      ustrcat__FPUsT0(buffer, B) and returned; otherwise undEnable is set to 2 and
//      B is returned. In all other cases B is returned.
// Both return paths hand out stack or static text; a stack result is dead after
// return. Unknown: the purpose of undEnable beyond this and what the bar-separated
// groups mean.
extern "C" {
extern signed char language;
extern int _4item$undEnable;
unsigned short* textGetString__FPc(char*);
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float, float);
void ustrcpy__FPUsT0(unsigned short*, unsigned short*);
void ustrcpy__FPUsPc(unsigned short*, char*);
void ustrcat__FPUsT0(unsigned short*, unsigned short*);
int ustrlen__FPUs(unsigned short*);

unsigned short* getAttributeName__4itemiPc(int index, char* key) {
    static unsigned short und[256];
    unsigned short a[128];
    unsigned short b[128];
    int bars = 0;
    unsigned short* result = b;
    __asm__("" : "+r"(result));  // the image returns this stack text; keep the compiler from folding it away
    usprintf__FPUsT0ffffffff(a, textGetString__FPc(key), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    if (index == -1) {
        usprintf__FPUsT0ffffffff(b, a, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    } else {
        if (a[0] != 0) {
            for (unsigned short* p = a; ; ) {
                unsigned short c = *p++;
                if (c == 124) bars = 1;
                if (*p == 0) break;
            }
        }
        if (bars == 0) {
            ustrcpy__FPUsT0(b, a);
        } else {
            unsigned short* p = a;
            unsigned short* o = b;
            if (index != 0) {
                int n = index;
                do {
                    if (*p++ == 124) n--;
                } while (n != 0);
            }
            unsigned short c = *p;
            while (c != 0 && c != 124) {
                *o++ = c;
                c = *++p;
            }
            *o = 0;
        }
    }
    if (language == 3 && bars != 0) {
        int n = ustrlen__FPUs(b);
        b[n - 3] = 0;
    }
    if (_4item$undEnable != 0 && b[0] == 'd' && b[1] == 'e' && b[3] == ' ') {
        if (_4item$undEnable != 2) {
            _4item$undEnable = 2;
            return result;
        }
        ustrcpy__FPUsPc(und, const_cast<char*>("und "));
        ustrcat__FPUsT0(und, b);
        return und;
    }
    return result;
}
}
