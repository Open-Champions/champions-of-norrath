"""EE object check for SysExecPS2Entry."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-execps2-entry/exec_ps2_entry.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'SysExecPS2Entry', 24, content=b'Z\x00\x00\x00\x00\x00\x00\x00[\x00\x00\x00\x00@\x07\x80\x02\xc4\xff\xff\x00\x00\x00\x00',
            imports=['kernel_execps2_pattern_kcopy'])
        row = support.defined(symbols, 'SysExecPS2Entry')
        self.assertEqual(sections[row["shndx"]]["name"], '.data')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 24)
        self.assertEqual(found, sorted([('kernel_execps2_pattern_kcopy', 4)]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
