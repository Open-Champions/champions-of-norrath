extern "C" int fontLastPrintedPosition;

int main() {
    if (sizeof(fontLastPrintedPosition) != 4) return 1;
    if (fontLastPrintedPosition != 0) return 2;
    fontLastPrintedPosition = 12;
    return fontLastPrintedPosition == 12 ? 0 : 3;
}
