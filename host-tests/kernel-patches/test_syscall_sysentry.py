"""EE object check for kernel_syscall_sysentry."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-syscall-sysentry/syscall_sysentry.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'kernel_syscall_sysentry', 16, content=b'\x83\x00\x00\x00\x00\x00\x00\x00Z\x00\x00\x00\x00\x00\x00\x00',
            imports=['kFindAddress', 'kernel_syscall_pattern_kcopy'])
        row = support.defined(symbols, 'kernel_syscall_sysentry')
        self.assertEqual(sections[row["shndx"]]["name"], '.data')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 16)
        self.assertEqual(found, sorted([('kFindAddress', 4), ('kernel_syscall_pattern_kcopy', 12)]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
