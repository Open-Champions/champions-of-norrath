// Observed compositeAlpha at 0x00484428: 4 bytes of initialized data, value 128 in the file. doSuperSample, engineSetBlur and engineSetVideoMode access
// it as a word.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
int compositeAlpha = 128;
}
