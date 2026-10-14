// Observed deci2Putchar at 0x002d67b8, 176 bytes. The line buffer is the
// 128-byte object `linebuf` at 0x008de8c0 and the fill position is the word
// `count` at 0x0044495c (initially 0). Both are local to the original library.
// A position of 126 or more first terminates the buffer at byte 127, flushes it
// through kputs and restarts at 0. A newline then stores itself at the current
// position, terminates the next byte, resets the position and flushes through
// kputs. Any other character is stored and the position advances.
extern "C" {
void kputs(const char* text);

static char linebuf[128];
static int count;

void deci2Putchar(int character) {
    int position = count;
    if (position >= 126) {
        count = 0;
        linebuf[127] = 0;
        kputs(linebuf);
        position = count;
    }
    if (character == '\n') {
        count = 0;
        linebuf[position] = static_cast<char>(character);
        linebuf[position + 1] = 0;
        kputs(linebuf);
        return;
    }
    count = position + 1;
    linebuf[position] = static_cast<char>(character);
}
}
