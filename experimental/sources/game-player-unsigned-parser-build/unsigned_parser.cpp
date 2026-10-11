#include "unsigned_parser.hpp"
using namespace norrath_parser_detail;

extern "C" NorrathUnsigned64 norrath_player_strtoul_reentrant(unsigned char* reent,const char* input,char** endpoint,int base) {
    const Word begin=address(input);Word cursor=begin;
    int character;
    do {character=signed_byte(*bytes(cursor));cursor+=1u;}while(classify(character)&8u);
    bool negative=false;
    if(character==45) {negative=true;character=signed_byte(*bytes(cursor));cursor+=1u;}
    else if(character==43) {character=signed_byte(*bytes(cursor));cursor+=1u;}
    if((base==0||base==16)&&character==48) {
        const int next=signed_byte(*bytes(cursor));
        if(next==120||next==88) {character=signed_byte(*bytes(cursor+1u));cursor+=2u;base=16;}
    }
    if(base==0)base=character==48?8:10;
    const NorrathUnsigned64 radix=NorrathUnsigned64(NorrathSigned64(base));
    const NorrathUnsigned64 cutoff=norrath_factory_udiv64(0xffffffffffffffffULL,radix);
    const int cutlim=signed_word(Word(norrath_factory_umod64(0xffffffffffffffffULL,radix)));
    int any=0;NorrathUnsigned64 value=0ULL;
    for(;;) {
        const Word flags=classify(character);
        int digit;
        if(flags&4u)digit=character-48;
        else if(flags&3u)digit=character-((flags&1u)?55:87);
        else break;
        if(digit>=base)break;
        if(any<0||value>cutoff||(value==cutoff&&digit>cutlim))any=-1;
        else {value=norrath_factory_mul64(value,radix)+NorrathUnsigned64(NorrathSigned64(digit));any=1;}
        character=signed_byte(*bytes(cursor));cursor+=1u;
    }
    if(any<0) {value=0xffffffffffffffffULL;write_word(address(reent),34u);}
    else if(negative)value=0ULL-value;
    if(endpoint!=nullptr)write_word(address(endpoint),any!=0?cursor-1u:begin);
    return value;
}

extern "C" NorrathUnsigned64 norrath_player_strtoul(const char* input,char** endpoint,int base) {
    const Word reent=read_word(address(norrath_factory_impure_pointer_word));
    return norrath_player_strtoul_reentrant(bytes(reent),input,endpoint,base);
}

extern "C" NorrathSigned64 norrath_player_atol(const char* input) {
    return norrath_factory_strtol(input,nullptr,10);
}
