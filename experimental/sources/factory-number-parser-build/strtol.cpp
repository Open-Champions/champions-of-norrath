#include "number_parser.hpp"
extern "C" NorrathSigned64 norrath_factory_strtol(const char* input, char** end_pointer, int base) {
    using namespace norrath_parser_detail;
    const Word reent = read_word(address(norrath_factory_impure_pointer_word));
    return norrath_factory_strtol_reentrant(bytes(reent), input, end_pointer, base);
}
