"""EE object check for lastCardSlot."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-last-card-slot/last_card_slot.cpp"


class WordTests(support.CardTest):
    def test_object(self):
        _data, _sections, symbols, relocations = self.expect_object(
            SOURCE, 'lastCardSlot', 4, content=b'\x00\x00\x00\x00', imports=[])
        if 'lastCardSlot' == "allActs":
            names = [item["symbol"] for item in relocations]
            self.assertEqual(names, ["act1", "act2", "act3", "act4", "act5"])


if __name__ == "__main__":
    unittest.main()
