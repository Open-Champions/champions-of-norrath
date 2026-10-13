// Observed cutRunFrameTask__Fv at 0x0018e428, 32 bytes.
// Calls cutRunFrame__Fb with the bool argument false (a zero argument register)
// and returns nothing. The call is the last action, so no state is kept.
// Nothing here was executed on the EE.
extern "C" void cutRunFrame__Fb(bool skip);

extern "C" void cutRunFrameTask__Fv() {
    cutRunFrame__Fb(false);
}
