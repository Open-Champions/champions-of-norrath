#pragma once
// Native EE strict comparison, returning a canonical unsigned word 0 or 1.
// The CPU determines comparison/status behavior; no host float model is used.
extern "C" unsigned ee_c_lt_s_word(float left, float right);
