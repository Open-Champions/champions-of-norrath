// Observed VIDisplayLoadingScreen at 0x0106c640, 132 bytes. It returns.
// A zero argument stores rand() masked with 3 into s_index.220 at
// 0x0118f2a4. Any other argument stores (s_index + 1) modulo 4, using
// signed division. VIDisplayScreen16 then receives the unnamed text
// LOADING, that index, and 1 only when the argument was zero.
extern "C" {
int rand();
void VIDisplayScreen16__FPCcib(const char* text, int index, int zero_argument);

static int s_index __asm__("s_index.220");

void VIDisplayLoadingScreen__Fb(int argument) {
    if (argument == 0) {
        s_index = rand() & 3;
    } else {
        s_index = (s_index + 1) % 4;
    }
    VIDisplayScreen16__FPCcib("LOADING", s_index, argument == 0 ? 1 : 0);
}
}
