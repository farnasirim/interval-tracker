#include "test_common.h"

TEST_CASE("RangeTracker objects must work correctly on the given add tests",
    "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  std::vector<std::pair<int, int>> empty = {};

  SECTION("add, scenario 1") {
    for(auto& it: rts) {
      it->Add(1, 2);
    }

    std::vector<std::pair<int, int>> existing = {{1, 2}};
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == existing);
      REQUIRE(it->Get(1, 2) == existing);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
    }

    std::vector<std::pair<int, int>> expected= {{1, 2}, {3, 5}};
    for(auto& it: rts) {
      it->Add(3, 5);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 4) == expected);
      REQUIRE(it->Get(1, 2) == existing);
      REQUIRE(it->Get(1, 3) == existing);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == empty);
    }
  }

  SECTION("add, scenario 2") {
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

    for(auto& it: rts) {
      it->Add(3, 5);
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

  SECTION("add, scenario 3") {
    for(auto& it: rts) {
      it->Add(1, 4);
    }

    std::vector<std::pair<int, int>> existing = {{1, 4}};
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == existing);
      REQUIRE(it->Get(1, 2) == existing);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == existing);
      REQUIRE(it->Get(5, 6) == empty);
      REQUIRE(it->Get(4, 5) == empty);
    }

    for(auto& it: rts) {
      it->Add(3, 5);
    }

    std::vector<std::pair<int, int>> expected = {{1, 5}};
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected);
      REQUIRE(it->Get(1, 2) == expected);
      REQUIRE(it->Get(-1, 1) == empty);
      REQUIRE(it->Get(2, 3) == expected);
      REQUIRE(it->Get(5, 6) == empty);
      REQUIRE(it->Get(6, 7) == empty);
    }
  }
}
