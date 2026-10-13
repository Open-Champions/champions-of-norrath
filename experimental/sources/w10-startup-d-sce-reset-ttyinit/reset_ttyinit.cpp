// Observed sceResetttyinit at 0x002d5560, 12 bytes. It stores zero into the word ttyinit
// (0x00444950) and returns nothing. write() tests that word before it initializes the tty.
// Nothing here was executed on the EE.
extern "C" {
extern int ttyinit;

void sceResetttyinit() { ttyinit = 0; }
}
