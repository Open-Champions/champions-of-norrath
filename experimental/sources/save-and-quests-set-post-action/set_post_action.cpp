// Observed scriptSetPostAction__FPc at 0x001d4060, 36 bytes. It copies the
// argument string, with its terminator, into the 200-byte buffer postAction.
// The buffer is the existing norrath_script_post_action_view. The copy is not
// bounded by the length.
extern "C" {
char* strcpy(char* destination, const char* source);
extern unsigned char norrath_script_post_action_view[200];

void scriptSetPostAction__FPc(char* text) {
    strcpy(reinterpret_cast<char*>(norrath_script_post_action_view), text);
}
}
