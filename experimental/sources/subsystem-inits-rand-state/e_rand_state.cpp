// eRandState at 0x00483e58, four file bytes, all zero, in .sdata.
// syncRandState begins at the next address. The reviewed -G0 profile may place
// this zero word in BSS. engineInit stores COP0 Count here; that store is not
// this definition. No EE startup stage has run.
extern "C" {
int eRandState;
}
