#include "test_common.h"

TEST_CASE("RangeTracker objects must truncate intervals on marginal deletes",
    "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  std::vector<std::pair<int, int>> empty = {};
  for(auto& it: rts) {
    it->Add(1, 6);
  }

  std::vector<std::pair<int, int>> existing = {{1, 6}};

  SECTION("delete, single delete: split") {
    std::vector<std::pair<int, int>> expected = {{1, 2}, {4, 6}};
    for(auto& it: rts) {
      it->Delete(2, 4);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == subvec(expected, 0, 1));
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
      REQUIRE(it->Get(5, 6) == subvec(expected, 1, 1));
      REQUIRE(it->Get(1, 5) == expected);
    }
  }

  SECTION("delete, single delete: split and delete again") {
    std::vector<std::pair<int, int>> expected1 = {{1, 3}, {4, 6}};
    std::vector<std::pair<int, int>> expected2 = {{1, 2}, {5, 6}};
    for(auto& it: rts) {
      it->Delete(3, 4);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected1);
      REQUIRE(it->Get(2, 3) == subvec(expected1, 0, 1));
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == subvec(expected1, 0, 1));
      REQUIRE(it->Get(4, 5) == subvec(expected1, 1, 1));
      REQUIRE(it->Get(1, 5) == expected1);
    }
    for(auto& it: rts) {
      it->Delete(2, 5);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected2);
      REQUIRE(it->Get(1, 2) == subvec(expected2, 0, 1));
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
      REQUIRE(it->Get(5, 6) == subvec(expected2, 1, 1));
      REQUIRE(it->Get(1, 5) == subvec(expected2, 0, 1));
    }
  }
}
