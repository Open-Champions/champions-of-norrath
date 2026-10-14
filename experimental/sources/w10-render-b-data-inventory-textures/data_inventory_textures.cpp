// Observed invHeader at 0x004848e0 and marble at 0x004848ec, four-byte words in .data, zero in
// the file; the neighbours lcursor, rcursor (between them), bevel, stage and highlightBar are
// the same kind. The authored inventory frame draw passes each as the first argument of a
// texture draw, so they are texture pointers filled at load time.
extern "C" {
void* invHeader;
void* marble;
}
