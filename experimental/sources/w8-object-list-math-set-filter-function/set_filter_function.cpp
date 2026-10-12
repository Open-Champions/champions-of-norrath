// Observed objectSetFilterFunction__FPFPc_i at 0x143d30, extent 8. Stores its
// argument in userFilterFunction at 0x483b00 (gp-relative) and returns nothing.
// The authored view of that word is norrath_user_filter_function_word.
using ObjectFilterFunction = int (*)(char*);

extern "C" ObjectFilterFunction norrath_user_filter_function_word;

extern "C" void objectSetFilterFunction__FPFPc_i(ObjectFilterFunction function) {
    norrath_user_filter_function_word = function;
}
