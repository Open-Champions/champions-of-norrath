// Observed fadeDownVal at 0x00484afc. One word between hudBustCovered/hudNetWaiting byte
// objects; the loaded image holds zero there. Callers declare it as a signed int.
extern "C" {
int fadeDownVal = 0;
}
