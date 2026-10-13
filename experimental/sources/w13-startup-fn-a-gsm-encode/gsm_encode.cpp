// Observed gsm_encode at 0x002ec2c0, 2316 bytes: encodes 160 16-bit samples into one 33-byte GSM 06.10 full-rate frame.
// Arguments: the encoder state, the 160 input samples and the output frame pointer. Returns 33.
// In order, as read from the code:
//  1. Clear 50 words of the extended prediction-error array (memset of 400 bytes).
//  2. Pre-emphasis / offset removal of the 160 samples into 64-bit words, using three state fields (a previous scaled
//     sample at +2240, a 64-bit accumulator at +2248, a 32-bit previous output at +2256): the sample is shifted right
//     by 3 and left by 2, the accumulator gains the difference to the previous scaled sample shifted left by 15, and
//     the output is (accumulator + 16384) >> 15 plus the previous output times -28672 shifted right by 15.
//  3. Gsm_LPC_Analysis(samples, LARc) (no state argument), Gsm_Short_Term_Analysis_Filter(state, LARc, samples).
//  4. For each of four 40-sample sub-frames: Gsm_Long_Term_Predictor(state, sub-frame, dp, e+5, dp, &Nc[j], &bc[j]) with
//     dp the state's word array at offset 960; Gsm_RPE_Encoding(e+5, &xmaxc[j], &Mc[j], &xMc[13*j]) (no state
//     argument); then add the 40 words of e+5 into dp and advance dp by 40 words.
//  5. Move the 120 words at state+1280 down to the start of the state.
//  6. Pack the frame: first byte 0xd0 with the top four bits of LARc[0] shifted, then the eight log-area ratios as
//     6,6,5,5,4,4,3,3 bits, then per sub-frame 7 (lag), 2 (gain), 2 (grid), 6 (peak) bits and thirteen 3-bit pulses,
//     the fields running across byte boundaries; this is the inverse of the gsm_decode unpack.
// Inferred: this is the GSM 06.10 encoder with 64-bit word type; field names are mine.
// Not authored here: the four Gsm_ helpers and memset.
extern "C" {
typedef long long word;

void* memset(void*, int, unsigned long);
void Gsm_LPC_Analysis(word* so, word* LARc);
void Gsm_Short_Term_Analysis_Filter(void* state, word* LARc, word* so);
void Gsm_Long_Term_Predictor(void* state, word* d, word* dp, word* e, word* dpp, word* Nc, word* bc);
void Gsm_RPE_Encoding(word* e, word* xmaxc, word* Mc, word* xMc);

int gsm_encode(void* state, const short* source, unsigned char* c) {
    word LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xMc[52], e[50], so[160];
    unsigned char* const s = static_cast<unsigned char*>(state);
    word* const dp0 = reinterpret_cast<word*>(s);
    word* dp = reinterpret_cast<word*>(s + 960);
    memset(e, 0, 400);
    word z1 = *reinterpret_cast<word*>(s + 2240);
    word L_z2 = *reinterpret_cast<word*>(s + 2248);
    word mp = *reinterpret_cast<int*>(s + 2256);
    for (int k = 0; k < 160; ++k) {
        const word scaled = (source[k] >> 3) << 2;
        L_z2 += (scaled - z1) << 15;
        z1 = scaled;
        const word product = (mp * -28672) >> 15;
        mp = (L_z2 + 16384) >> 15;
        so[k] = mp + product;
    }
    *reinterpret_cast<word*>(s + 2240) = z1;
    *reinterpret_cast<word*>(s + 2248) = L_z2;
    *reinterpret_cast<int*>(s + 2256) = static_cast<int>(mp);
    Gsm_LPC_Analysis(so, LARc);
    Gsm_Short_Term_Analysis_Filter(state, LARc, so);
    for (int j = 0; j < 4; ++j) {
        Gsm_Long_Term_Predictor(state, so + 40 * j, dp, e + 5, dp, Nc + j, bc + j);
        Gsm_RPE_Encoding(e + 5, xmaxc + j, Mc + j, xMc + 13 * j);
        for (int i = 0; i < 40; ++i) dp[i] += e[5 + i];
        dp += 40;
    }
    for (int i = 0; i < 120; ++i) dp0[i] = dp0[160 + i];
    *c++ = 0xd0 | ((LARc[0] >> 2) & 0xf);
    *c++ = ((LARc[0] & 0x3) << 6) | (LARc[1] & 0x3f);
    *c++ = ((LARc[2] & 0x1f) << 3) | ((LARc[3] >> 2) & 0x7);
    *c++ = ((LARc[3] & 0x3) << 6) | ((LARc[4] & 0xf) << 2) | ((LARc[5] >> 2) & 0x3);
    *c++ = ((LARc[5] & 0x3) << 6) | ((LARc[6] & 0x7) << 3) | (LARc[7] & 0x7);
    for (int j = 0; j < 4; ++j) {
        const word* const x = xMc + 13 * j;
        *c++ = ((Nc[j] & 0x7f) << 1) | ((bc[j] >> 1) & 0x1);
        *c++ = ((bc[j] & 0x1) << 7) | ((Mc[j] & 0x3) << 5) | ((xmaxc[j] >> 1) & 0x1f);
        *c++ = ((xmaxc[j] & 0x1) << 7) | ((x[0] & 0x7) << 4) | ((x[1] & 0x7) << 1) | ((x[2] >> 2) & 0x1);
        *c++ = ((x[2] & 0x3) << 6) | ((x[3] & 0x7) << 3) | (x[4] & 0x7);
        *c++ = ((x[5] & 0x7) << 5) | ((x[6] & 0x7) << 2) | ((x[7] >> 1) & 0x3);
        *c++ = ((x[7] & 0x1) << 7) | ((x[8] & 0x7) << 4) | ((x[9] & 0x7) << 1) | ((x[10] >> 2) & 0x1);
        *c++ = ((x[10] & 0x3) << 6) | ((x[11] & 0x7) << 3) | (x[12] & 0x7);
    }
    return 33;
}
}
