extern "C" unsigned char halfFrustumMode;
int main() {
    if (sizeof halfFrustumMode != 1 || halfFrustumMode != 0) return 1;
    halfFrustumMode = 1;
    return halfFrustumMode == 1 ? 0 : 2;
}
