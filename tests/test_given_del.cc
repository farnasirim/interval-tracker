#include "test_common.h"

TEST_CASE("RangeTracker objects must work correctly on the given delete tests",
    "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  std::vector<std::pair<int, int>> empty = {};
    for(auto& it: rts) {
      it->Add(1, 6);
    }
  std::vector<std::pair<int, int>> existing = {{1, 6}};
  for(auto& it: rts) {
    REQUIRE(it->Get(-200, 200) == existing);
    REQUIRE(it->Get(1, 2) == existing);
    REQUIRE(it->Get(-1, 1) == empty);
    REQUIRE(it->Get(2, 3) == existing);
    REQUIRE(it->Get(5, 6) == existing);
    REQUIRE(it->Get(6, 7) == empty);
  }

  SECTION("delete, scenario 1") {
    for(auto& it: rts) {
      it->Delete(-3, -1);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == existing);
      REQUIRE(it->Get(1, 2) == existing);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == existing);
      REQUIRE(it->Get(5, 6) == existing);
      REQUIRE(it->Get(6, 7) == empty);
    }
  }

  SECTION("delete, scenario 2") {
    for(auto& it: rts) {
      it->Delete(-1, 10);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == empty);
      REQUIRE(it->Get(1, 2) == empty);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
      REQUIRE(it->Get(5, 6) == empty);
      REQUIRE(it->Get(6, 7) == empty);
    }
  }

  SECTION("delete, scenario 3") {
    for(auto& it: rts) {
      it->Delete(4, 10);
    }
  std::vector<std::pair<int, int>> expected = {{1, 4}};
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == expected);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == expected);
      REQUIRE(it->Get(5, 6) == empty);
      REQUIRE(it->Get(6, 7) == empty);
    }
  }

  // i) Original state [(1, 6)], Delete(-3, -1), New state: [(1, 6)]
  // ii) Original state [(1, 6)], Delete(-1, 10), New state: []
  // iii) Original state [(1, 6)], Delete(4, 10), New state: [(1, 4)]
}


