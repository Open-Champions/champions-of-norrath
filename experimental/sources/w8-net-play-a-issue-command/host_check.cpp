#include "issue_command.cpp"
unsigned command[4];
unsigned char CDmode[4];
signed char powerOffMode;
static char log[64];
static int n, failuresLeft;
static unsigned a[4];
static void put(char c) { log[n++] = c; }
extern "C" {
void enginePushBorderColor__FUi(unsigned c) { put(c == 0xffffffu ? 'P' : '?'); }
void enginePopBorderColor__Fv() { put('O'); }
void engineSyncVBlank__Fv() { put('V'); }
int sceCdRead(unsigned f, unsigned s, unsigned t, void* m) {
    put('R'); a[0] = f; a[1] = s; a[2] = t; a[3] = m == CDmode;
    return failuresLeft-- <= 0;
}
int sceCdReadIOPm(unsigned f, unsigned s, unsigned t, void* m) {
    put('I'); a[0] = f; a[1] = s; a[2] = t; a[3] = m == CDmode;
    return failuresLeft-- <= 0;
}
}
static int is(const char* s) { log[n] = 0; int i = 0; for (; s[i] && log[i] == s[i]; ++i) {} return s[i] == 0 && log[i] == 0; }
int main() {
    command[1] = 11; command[2] = 22; command[3] = 33;
    command[0] = 0; issueCommand__Fv();
    if (n != 0) return 1;
    command[0] = 3; issueCommand__Fv();
    if (n != 0) return 2;
    command[0] = 1; issueCommand__Fv();
    if (!is("PRO") || a[0] != 11 || a[1] != 22 || a[2] != 33 || a[3] != 1) return 3;
    n = 0; failuresLeft = 2; command[0] = 2; issueCommand__Fv();
    if (!is("PIVIVIO")) return 4;
    if (a[0] != 11 || a[1] != 22 || a[2] != 33 || a[3] != 1) return 5;
    return 0;
}
