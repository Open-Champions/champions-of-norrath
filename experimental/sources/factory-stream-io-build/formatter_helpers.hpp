#pragma once
#include "core_formatter.hpp"
extern "C" {
// Genuine unresolved services: none is a host printf/dtoa/FILE adapter.
int norrath_factory_scatter_write(unsigned char* stream, unsigned char* iovec_header);
int norrath_factory_stream_flush(unsigned char* stream);
NorrathFormatUnsigned64 norrath_factory_double_subtract(NorrathFormatUnsigned64 left_bits, NorrathFormatUnsigned64 right_bits);
unsigned char* norrath_factory_dtoa_reentrant(unsigned char* reent, NorrathFormatUnsigned64 bits, int mode, int precision, unsigned char* decimal_position_word, unsigned char* sign_word, unsigned char* end_pointer_word);
}
