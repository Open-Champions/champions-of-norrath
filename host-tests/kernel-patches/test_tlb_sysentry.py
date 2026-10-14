"""EE object check for kernel_tlb_sysentry."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-tlb-sysentry/tlb_sysentry.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'kernel_tlb_sysentry', 64, content=b'Z\x00\x00\x00\x00\x00\x00\x00[\x00\x00\x00\x00P\x07\x80T\x00\x00\x00\x00\x00\x00\x00U\x00\x00\x00\x00\x00\x00\x00V\x00\x00\x00\x00\x00\x00\x00W\x00\x00\x00\x00\x00\x00\x00X\x00\x00\x00\x00\x00\x00\x00Y\x00\x00\x00\x00\x00\x00\x00',
            imports=['_kExitTLBHandler', 'kernel_tlb_pattern_kcopy'])
        row = support.defined(symbols, 'kernel_tlb_sysentry')
        self.assertEqual(sections[row["shndx"]]["name"], '.data')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 64)
        self.assertEqual(found, sorted([('kernel_tlb_pattern_kcopy', 4), ('_kExitTLBHandler', 20)]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
