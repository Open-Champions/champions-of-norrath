// Observed gsm_decode at 0x002eb7e0, 2324 bytes: decodes one 33-byte GSM 06.10 full-rate frame into 160 samples.
// Arguments: the decoder state (the object made by gsm_create), the frame bytes and the output sample pointer.
// Returns 160 for a good frame, 0 when the high nibble of the first byte is not 13 (nothing else is done then).
// In order, as read from the code:
//  1. Unpack the 260 payload bits: the low nibble of byte 0 and the next bytes give eight log-area ratios
//     (6, 6, 5, 5, 4, 4, 3, 3 bits), then four sub-frames of 7 (lag), 2 (gain), 2 (grid), 6 (peak) bits and thirteen
//     3-bit pulse values, the bit fields running on across byte boundaries (the layout the public GSM 06.10 packer
//     uses). Every parameter is held in a 64-bit word (the arrays are 8 bytes per element).
//  2. For each sub-frame j = 0..3: Gsm_RPE_Decoding(xmaxc[j], Mc[j], &xmc[13*j], erp) (no state argument),
//     Gsm_Long_Term_Synthesis_Filtering(state, Nc[j], bc[j], erp, drp) with drp the state's word array at offset 960
//     (120 words past the start of the state), then 40 words are copied from drp into the work array wt[40*j].
//  3. Gsm_Short_Term_Synthesis_Filter(state, LARc, wt, target).
//  4. De-emphasis over the 160 output samples with the 64-bit word at state +2552 (msr): each step takes
//     msr*28672 shifted right by 15 (no rounding term; plain arithmetic shift), adds the sample, stores that as the new
//     msr, doubles it with saturation to the 16-bit range, clears the low three bits and stores the sample. The
//     final msr is stored back.
// The image has the decoder's body (steps 2 to 4) inline here and also as the separate Gsm_Decoder at 0x002ec0f8; this
// unit writes it inline and calls the three helpers directly, as the code does.
// Inferred: this is the GSM 06.10 decoder with 64-bit word type; field names are mine.
// Not authored here: Gsm_RPE_Decoding, Gsm_Long_Term_Synthesis_Filtering, Gsm_Short_Term_Synthesis_Filter.
extern "C" {
typedef long long word;

void Gsm_RPE_Decoding(word xmaxc, word Mc, word* xmc, word* erp);
void Gsm_Long_Term_Synthesis_Filtering(void* state, word Nc, word bc, word* erp, word* drp);
void Gsm_Short_Term_Synthesis_Filter(void* state, word* LARc, word* wt, short* target);

int gsm_decode(void* state, const unsigned char* c, short* target) {
    word LARc[8], Nc[4], bc[4], Mc[4], xmaxc[4], xmc[52];
    if (((*c >> 4) & 0x0f) != 13) return 0;
    LARc[0] = (*c++ & 0xf) << 2;
    LARc[0] |= (*c >> 6) & 0x3;
    LARc[1] = *c++ & 0x3f;
    LARc[2] = (*c >> 3) & 0x1f;
    LARc[3] = (*c++ & 0x7) << 2;
    LARc[3] |= (*c >> 6) & 0x3;
    LARc[4] = (*c >> 2) & 0xf;
    LARc[5] = (*c++ & 0x3) << 2;
    LARc[5] |= (*c >> 6) & 0x3;
    LARc[6] = (*c >> 3) & 0x7;
    LARc[7] = *c++ & 0x7;
    for (int j = 0; j < 4; ++j) {
        word* const x = xmc + 13 * j;
        Nc[j] = (*c >> 1) & 0x7f;
        bc[j] = (*c++ & 0x1) << 1;
        bc[j] |= (*c >> 7) & 0x1;
        Mc[j] = (*c >> 5) & 0x3;
        xmaxc[j] = (*c++ & 0x1f) << 1;
        xmaxc[j] |= (*c >> 7) & 0x1;
        x[0] = (*c >> 4) & 0x7;
        x[1] = (*c >> 1) & 0x7;
        x[2] = (*c++ & 0x1) << 2;
        x[2] |= (*c >> 6) & 0x3;
        x[3] = (*c >> 3) & 0x7;
        x[4] = *c++ & 0x7;
        x[5] = (*c >> 5) & 0x7;
        x[6] = (*c >> 2) & 0x7;
        x[7] = (*c++ & 0x3) << 1;
        x[7] |= (*c >> 7) & 0x1;
        x[8] = (*c >> 4) & 0x7;
        x[9] = (*c >> 1) & 0x7;
        x[10] = (*c++ & 0x1) << 2;
        x[10] |= (*c >> 6) & 0x3;
        x[11] = (*c >> 3) & 0x7;
        x[12] = *c++ & 0x7;
    }
    word erp[40], wt[160];
    word* const drp = reinterpret_cast<word*>(static_cast<unsigned char*>(state) + 960);
    for (int j = 0; j < 4; ++j) {
        Gsm_RPE_Decoding(xmaxc[j], Mc[j], xmc + 13 * j, erp);
        Gsm_Long_Term_Synthesis_Filtering(state, Nc[j], bc[j], erp, drp);
        for (int k = 0; k < 40; ++k) wt[j * 40 + k] = drp[k];
    }
    Gsm_Short_Term_Synthesis_Filter(state, LARc, wt, target);
    word* const msr = reinterpret_cast<word*>(static_cast<unsigned char*>(state) + 2552);
    word history = *msr;
    short* sample = target;
    for (int k = 160; k--; ++sample) {
        const word scaled = (history * 28672) >> 15;
        history = *sample + scaled;
        word doubled = history + history;
        if (doubled >= 32767) doubled = 32767;
        else if (doubled < -32768) doubled = -32768;
        *sample = static_cast<short>(doubled & 0xfff8);
    }
    *msr = history;
    return 160;
}
}
