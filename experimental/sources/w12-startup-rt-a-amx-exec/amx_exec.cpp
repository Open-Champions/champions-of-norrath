// Observed amx_Exec at 0x00135de8, 9176 bytes, pinned SLUS_205.65: the script machine's entry. Arguments, in
// registers (64 bits wide each): the machine record (null asks for the dispatch table), a result pointer,
// the public function index (-1 is the main entry, -2 the continue entry), the parameter count and a
// pointer to the parameters, which the routine reads at an eight-byte stride. Returns the machine's status
// code in $v0 (0 on success).
// The interpreter itself is already authored as norrath_amx_exec (script-amx-exec): a cell executor whose
// code cells are opcode numbers 0..130. This unit only supplies the original symbol name so that callers
// that import amx_Exec resolve without a symbol alias.
// DIFFERENCE THAT CANNOT BE REMOVED HERE: the original is direct-threaded and its code cells hold handler
// addresses after relocation, while the authored executor keeps opcode numbers in the cells. With a null
// machine record the original stores the address of its 131-entry handler table in *result; the authored
// executor stores the address of its own table of the numbers 0..130. A relocation step has to use the
// table of the executor that will run the code. Everything else (status codes, register state at calls to
// the host callback, memory effects) is what the paired run compared; see docs/agents/w12-startup-rt-a.md.
extern "C" {
unsigned long long norrath_amx_exec(void* amx, int* result, long long public_index, long long parameter_count,
                                    const void* parameter_data);

unsigned long long amx_Exec(void* amx, int* result, long long public_index, long long parameter_count,
                            const void* parameter_data) {
    return norrath_amx_exec(amx, result, public_index, parameter_count, parameter_data);
}
}
