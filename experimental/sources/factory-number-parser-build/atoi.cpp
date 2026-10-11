#include "number_parser.hpp"
extern "C" int norrath_factory_atoi(const char* input) {
    const NorrathSigned64 parsed = norrath_factory_strtol(input, nullptr, 10);
    return norrath_parser_detail::signed_word(static_cast<unsigned>(parsed));
}
