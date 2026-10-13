// Observed .data (local) symbol batParams at 0x4858e0, 8 bytes: a pointer word to the C string
// "oneshot=1" (at 0x473770 in the image) and a zero word. The next object starts at 0x4858e8.
// No instruction in the image refers to this address by name or by a computed address that this
// package could find; Unknown: its readers.
static char text_0[] = "oneshot=1";
struct Params {
    char* text;
    unsigned unused;
};
extern "C" {
Params norrath_front_a_view_4858e0 = {text_0, 0u};
}
