#pragma once
#include "number_parser.hpp"
extern "C" {
NorrathUnsigned64 norrath_player_strtoul(const char*,char**,int);
NorrathUnsigned64 norrath_player_strtoul_reentrant(unsigned char*,const char*,char**,int);
NorrathSigned64 norrath_player_atol(const char*);
}
