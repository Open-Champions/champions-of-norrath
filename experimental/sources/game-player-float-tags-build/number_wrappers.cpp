#include "float_tags.hpp"
extern "C" NorrathFormatUnsigned64 norrath_player_atof(const char* text){return norrath_player_strtod(text,nullptr);}
extern "C" NorrathFormatUnsigned64 norrath_player_strtod(const char* text,char** end){
    using namespace norrath_float_tags_detail;
    unsigned char* reent=bytes(read_word(address(norrath_factory_impure_pointer_word)));
    return norrath_player_strtod_reentrant(reent,text,end);
}
