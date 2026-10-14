// Observed in the pinned image: _6VIFont$sm_FontName at 0x011c3090, 8 bytes, in the file-backed data of the VI module (listed under
// .text in the section table). The file holds the seven ASCII characters A r i a l U I followed by one zero byte.
// Inferred: a static character buffer of VIFont (the sm_ prefix and name say a static member holding a font name) that names the
// default UI face; whether it is overwritten at run time is unknown, so it is kept writable.
// Unknown: every reader and writer of this object.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
char norrath_vifont_sm_font_name[8] __asm__("_6VIFont$sm_FontName") NORRATH_LOADED(".data") = {'A', 'r', 'i', 'a', 'l', 'U', 'I', 0};
}
