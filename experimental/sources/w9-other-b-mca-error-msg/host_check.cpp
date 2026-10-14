// Host check for mca_error_msg.cpp; not an EE plan input.
#include "mca_error_msg.cpp"

int main() {
    if (MCA_ErrorMsg != 0) return 1;
    MCA_ErrorMsg = "x";
    if (MCA_ErrorMsg[0] != 'x') return 2;
    return 0;
}
