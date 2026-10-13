// Observed SetIopFunction__FUii at 0x01127040, 152 bytes, a local (file-static) function of the
// sound/IOP interface code loaded above the main image. Arguments: a function code and a word
// count n. It appends one command to a send buffer of 128 words:
//   i = the index word. When i + n + 3 is not below 128 nothing happens. Otherwise
//   send[i + 1] = function code, send[i + 2] = n, send[i + 3 .. i + 2 + n] = the first n words of
//   the argument array (sIopFunctionData, copied in order), the index word becomes i + 2 + n and
//   send[0] (a command count) is incremented. The index word starts at 0 in the image's
//   initial state, so the first command lands at send[1].
// The three data objects are local symbols of the image: sSend (0x011edb00, 512 bytes, followed by
// sRecv), sFunctionIndex (0x011edf00, an 8-byte slot of which the first word is the index) and
// sIopFunctionData (0x011edf08, 512 bytes up to sSifClient). A second, different local symbol
// named sSend exists at 0x011e25c0, so the authored names carry the address. Their storage is not
// authored here; the unit refers to them as imports. Nothing is returned.
// Inferred: the buffer is the argument block of an IOP remote call the caller then sends.
// Unknown: who fills sIopFunctionData and resets the index/count, and the meaning of the codes.
// Nothing here ran on the EE.
extern "C" {
extern int norrath_overlay_sSend_11edb00[];
extern int norrath_overlay_sFunctionIndex_11edf00;
extern int norrath_overlay_sIopFunctionData_11edf08[];
}

extern "C" void SetIopFunction__FUii(unsigned function, int count) {
    int index = norrath_overlay_sFunctionIndex_11edf00;
    if (index + count + 3 < 128) {
        int* send = norrath_overlay_sSend_11edb00;
        send[index + 1] = static_cast<int>(function);
        send[index + 2] = count;
        for (int i = 0; i < count; ++i) send[index + 3 + i] = norrath_overlay_sIopFunctionData_11edf08[i];
        norrath_overlay_sFunctionIndex_11edf00 = index + 2 + count;
        send[0] = send[0] + 1;
    }
}
