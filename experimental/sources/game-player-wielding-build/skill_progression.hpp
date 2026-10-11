#pragma once
#include "constructor.hpp"
using NorrathSkillIdentity = long long;
static_assert(sizeof(NorrathSkillIdentity)==8 && sizeof(unsigned short)==2,
              "original full64 feat identity and signed16 skill records");
extern "C" {
int norrath_player_get_skill_rank(int player_index,int skill_index);
void norrath_player_set_skill_rank(int player_index,int skill_index,int rank);
void norrath_inventory_adjust_skill(int player_index,char skill_mode,NorrathSkillIdentity identity);
// Complete genuine original services remain imports, not answers or adapters.
int norrath_player_get_feat_rank(int player_index,NorrathSkillIdentity identity);
void norrath_player_set_feat_rank(int player_index,NorrathSkillIdentity identity,int rank);
void norrath_player_give_feat(NorrathSkillIdentity identity,int player_index,int count);
void norrath_player_give_skill(int skill_index,int player_index);
// Sized original GLOBAL evidence: player95936@408600, skills252@34b440.
// No candidate owner, initializer, inferred player class or descriptor allocation.
extern unsigned char norrath_player_progression_records_view[];
extern unsigned char norrath_player_skill_catalog_view[];
}
namespace norrath_skill_progression_detail {
using namespace norrath_constructor_detail;
// Logical original-width effects through byte views. Volatile preserves fresh
// overlapping reads; native atomic/fault/MMIO byte-versus-half/word traces differ.
inline unsigned read_half_fresh(Address a) {
    const volatile unsigned char* p=bytes(a);
    return unsigned(p[0])|(unsigned(p[1])<<8);
}
inline int read_signed_half_fresh(Address a) {
    const unsigned value=read_half_fresh(a);
    return value<32768u?int(value):int(value)-65536;
}
inline unsigned read_word_fresh(Address a) {
    const volatile unsigned char* p=bytes(a);
    return unsigned(p[0])|(unsigned(p[1])<<8)|(unsigned(p[2])<<16)|(unsigned(p[3])<<24);
}
inline void write_half_ordered(Address a,unsigned value) {
    volatile unsigned char* p=bytes(a);
    p[0]=static_cast<unsigned char>(value);p[1]=static_cast<unsigned char>(value>>8);
}
inline void write_word_ordered(Address a,unsigned value) {
    volatile unsigned char* p=bytes(a);
    p[0]=static_cast<unsigned char>(value);p[1]=static_cast<unsigned char>(value>>8);
    p[2]=static_cast<unsigned char>(value>>16);p[3]=static_cast<unsigned char>(value>>24);
}
inline Address player_record(int index) {
    return address(norrath_player_progression_records_view)+unsigned(index)*23984u;
}
inline int increment_rank(int rank) {return signed_word(unsigned(rank)+1u);}
}
