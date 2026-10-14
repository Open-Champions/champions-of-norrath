"""allActs points at the five act tables."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-all-acts/all_acts.cpp"


class WordTests(support.CardTest):
    def test_object(self):
        _data, _sections, symbols, relocations = self.expect_object(
            SOURCE, "allActs", 20, content=None,
            imports=["act1", "act2", "act3", "act4", "act5"])
        names = [item["symbol"] for item in relocations]
        self.assertEqual(names, ["act1", "act2", "act3", "act4", "act5"])

    def test_host_pointers(self):
        self.host([SOURCE], r'''
extern "C" char act1[4] = {1};
extern "C" char act2[4] = {2};
extern "C" char act3[4] = {3};
extern "C" char act4[4] = {4};
extern "C" char act5[4] = {5};
extern "C" char* allActs[5];
int main() {
    if (allActs[0] != act1 || allActs[1] != act2 || allActs[2] != act3) return 1;
    if (allActs[3] != act4 || allActs[4] != act5) return 2;
    return 0;
}
''')


if __name__ == "__main__":
    unittest.main()
