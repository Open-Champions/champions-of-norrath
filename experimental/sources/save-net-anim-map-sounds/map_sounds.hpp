#pragma once

// Maps names in one animation header onto sound ids.
// Observed callee symbols, left unresolved here:
//   soundMapSound__FPci
//   soundComputeSequenceLength__FP15AnimationHeader
// The string compare is the existing reconstructed strcmp.
extern "C" void norrath_archive_map_animation_sounds(void* header);
