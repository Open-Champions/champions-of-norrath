#include "scanner.hpp"
extern "C" int norrath_player_string_eof_read(unsigned char*,char*,int){
    // Whole original LOCAL eofread: JR RA, MOVE V0,ZERO. The genuine refill
    // interprets a zero-byte read as EOF and sets the stream's flags itself.
    return 0;
}
extern "C" int norrath_player_sscanf(const char* input,const char* format,...){
    using namespace norrath_scanner_detail;
    alignas(8) unsigned char stream[96]; // Unwritten original fields stay unwritten.
    const Word file=address(stream);
    write_half(file+12u,4u);
    write_word(file+16u,address(input));
    write_word(file,address(input));
    const Word size=norrath_factory_strlen(input);
    const Word reent=read_word(address(norrath_factory_impure_pointer_word));
    write_word(file+20u,size);
    write_word(file+32u,Word(reinterpret_cast<__UINTPTR_TYPE__>(&norrath_player_string_eof_read)));
    write_word(file+84u,reent);write_word(file+4u,size);
    write_word(file+48u,0u);write_word(file+68u,0u);
    __builtin_va_list arguments;__builtin_va_start(arguments,format);
    // Actual fixed single-float EABI flat cursor,8-byte GPR save slots.
    const int result=norrath_player_stream_scan(stream,format,static_cast<const unsigned char*>(arguments));
    __builtin_va_end(arguments);return result;
}
