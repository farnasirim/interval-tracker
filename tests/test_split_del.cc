#include "test_common.h"

TEST_CASE("RangeTracker objects must split intervals on nested deletes",
    "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  std::vector<std::pair<int, int>> empty = {};
  for(auto& it: rts) {
    it->Add(1, 6);
  }

  std::vector<std::pair<int, int>> existing = {{1, 6}};

  SECTION("delete, marginal right") {
    std::vector<std::pair<int, int>> expected = {{1, 4}};
    for(auto& it: rts) {
      it->Delete(4, 6);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == expected);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == expected);
      REQUIRE(it->Get(5, 6) == empty);
      REQUIRE(it->Get(1, 5) == expected);
    }
  }

  SECTION("delete, marginal right, exceeding") {
    std::vector<std::pair<int, int>> expected = {{1, 4}};
    for(auto& it: rts) {
      it->Delete(4, 7);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == expected);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == expected);
      REQUIRE(it->Get(5, 6) == empty);
      REQUIRE(it->Get(1, 5) == expected);
    }
  }

  SECTION("delete, marginal left") {
    std::vector<std::pair<int, int>> expected = {{3, 6}};
    for(auto& it: rts) {
      it->Delete(1, 3);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == empty);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
      REQUIRE(it->Get(5, 6) == expected);
      REQUIRE(it->Get(1, 5) == expected);
    }
  }

  SECTION("delete, marginal left, exceeding") {
    std::vector<std::pair<int, int>> expected = {{3, 6}};
    for(auto& it: rts) {
      it->Delete(-1, 3);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == empty);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
      REQUIRE(it->Get(5, 6) == expected);
      REQUIRE(it->Get(1, 5) == expected);
    }
  }
}
