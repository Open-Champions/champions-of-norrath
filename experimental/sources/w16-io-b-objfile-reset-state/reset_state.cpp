// Observed: ResetState__9VIObjFile at 0x010a6148 is 48 bytes and returns 0. With
// the object file as this it stores -1 in the signed word at +232 (the current
// nesting index) and zero in the words at +560, +564, +568, +572, +576, +580,
// +584 and +588. Nothing else is read or written.
// Inferred: +564/+568 are the read/write flags set by PostOpen__9VIObjFile, +572
// and +576 the nesting flags ReadBegin__9VIObjFileRUsT1RUlRUi tests and sets, and
// +584 the flag the Read functions test, so this returns the file to the idle
// state between objects. -1 at +232 is the empty nesting index.
// Unknown: member names, and what +560 and +580 hold.
extern "C" {
int ResetState__9VIObjFile(unsigned char* file) {
    *reinterpret_cast<int*>(file + 560) = 0;
    *reinterpret_cast<int*>(file + 232) = -1;
    *reinterpret_cast<int*>(file + 564) = 0;
    *reinterpret_cast<int*>(file + 568) = 0;
    *reinterpret_cast<int*>(file + 572) = 0;
    *reinterpret_cast<int*>(file + 576) = 0;
    *reinterpret_cast<int*>(file + 580) = 0;
    *reinterpret_cast<int*>(file + 584) = 0;
    *reinterpret_cast<int*>(file + 588) = 0;
    return 0;
}
}
