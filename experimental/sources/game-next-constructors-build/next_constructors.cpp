#include "next_constructors.hpp"
namespace {
using namespace norrath_constructor_detail;
struct PointBytes { unsigned x,y,z; };
static_assert(sizeof(PointBytes)==12 && alignof(PointBytes)==4,"raw point storage");
PointBytes snapshot(const void* point){
    const Address a=address(point);
    return PointBytes{read_word(a),read_word(a+4u),read_word(a+8u)};
}
unsigned base(void* object,const void* point,void* map,char** arguments){
    PointBytes local=snapshot(point);
    return norrath_game_object_construct(object,&local,map,arguments);
}
}
extern "C" unsigned norrath_construct_TriggerParams(void* object,const void* point,char** arguments){
    (void)base(object,point,nullptr,arguments);
    const Address a=address(object);
    const unsigned flags=read_word(a+12u)|0x100003u;
    write_word(a+72u,address(norrath_next_view_44c958));write_word(a+12u,flags);
    return 0x100003u; // Original V0 is the mask register, not self/updated flags.
}
extern "C" unsigned norrath_construct_PokeReflector(void* object,const void* point,char** arguments){
    (void)base(object,point,nullptr,arguments);
    const Address a=address(object);
    const unsigned flags=read_word(a+12u)|0x100003u;
    write_word(a+72u,address(norrath_next_view_449280));write_word(a+12u,flags);
    return 0x100003u;
}
extern "C" unsigned norrath_construct_Lever(void* object,const void* point,int type,char** arguments){
    (void)base(object,point,norrath_next_view_42c650,arguments);
    const Address a=address(object);
    const unsigned flags=read_word(a+12u)|0x100001u;
    bytes(a)[56]=static_cast<unsigned char>(unsigned(type));
    bytes(a)[57]=static_cast<unsigned char>(unsigned(type)>>8);
    bytes(a)[80]=1u;
    write_word(a+72u,address(norrath_next_view_4491f0));write_word(a+12u,flags);
    return 0x100001u;
}
extern "C" unsigned norrath_construct_Timer(void* object,const void* point,char** arguments){
    const unsigned result=base(object,point,nullptr,arguments);
    const Address a=address(object);
    const unsigned flags=read_word(a+12u)|3u;
    write_word(a+72u,address(norrath_next_view_4490d0));write_word(a+12u,flags);
    return result; // No later original V0 write; genuine base currently returns0.
}
extern "C" unsigned norrath_construct_Counter(void* object,const void* point,char** arguments){
    const unsigned result=base(object,point,nullptr,arguments);
    const Address a=address(object);
    const unsigned flags=read_word(a+12u)|3u;
    write_word(a+72u,address(norrath_next_view_449160));write_word(a+12u,flags);
    return result;
}
extern "C" unsigned norrath_construct_Trap(void* object,const void* point,short type,char** arguments){
    // The complete original discards incoming A2/A3; both base imports are null.
    (void)type;(void)arguments;
    (void)base(object,point,nullptr,nullptr);
    const Address a=address(object);
    const unsigned flags=read_word(a+12u)|0x400fu;
    write_word(a+72u,address(norrath_next_view_44e820));
    write_word(a+28u,0x41c00000u);write_word(a+12u,flags);
    // Actual predecrement/BGEZ loop covers all four bytes79/78/77/76.
    for(unsigned index=0u;index<4u;++index)bytes(a+79u-index)[0]=0u;
    return norrath_actor_add_slow_run_list(object);
}
