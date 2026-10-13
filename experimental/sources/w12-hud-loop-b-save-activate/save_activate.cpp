// Observed saveActivate__9Savepoint at 0x0021c318, 12 bytes: the word at object offset 204 becomes 1.
// Nothing else is read or written and nothing is returned.
// Unknown: what the word at offset 204 controls, and the full Savepoint layout.
extern "C" void saveActivate__9Savepoint(void* self) {
    *reinterpret_cast<int*>(static_cast<char*>(self) + 204) = 1;
}
