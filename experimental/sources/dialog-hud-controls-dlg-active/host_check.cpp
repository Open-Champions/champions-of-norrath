extern "C" {
unsigned curDlg;
int DLG_Active__Fv();
}

int main() {
    const unsigned samples[] = {0u, 1u, 2u, 0xffffffffu, 0x80000000u};
    for (unsigned sample : samples) {
        curDlg = sample;
        const int got = DLG_Active__Fv();
        const int want = sample != 0u ? 1 : 0;
        if (got != want) return 1;
    }
    return 0;
}
