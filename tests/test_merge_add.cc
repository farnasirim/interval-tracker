#include "test_common.h"

TEST_CASE("RangeTracker objects must merge intervals that align",
    "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  std::vector<std::pair<int, int>> empty = {};
  for(auto& it: rts) {
    it->Add(1, 6);
  }

  SECTION("add, merge right") {
    std::vector<std::pair<int, int>> expected1 = {{1, 7}};
    std::vector<std::pair<int, int>> expected2 = {{1, 9}};
      for(auto& it: rts) {
        it->Add(6, 7);
      }
      for(auto& it: rts) {
        REQUIRE(it->Get(-200, 200) == expected1);
      }

      for(auto& it: rts) {
        it->Add(7, 9);
      }
      for(auto& it: rts) {
        REQUIRE(it->Get(-200, 200) == expected2);
      }
  }

  SECTION("add, merge left") {
    std::vector<std::pair<int, int>> expected1 = {{0, 6}};
    std::vector<std::pair<int, int>> expected2 = {{-2, 6}};
      for(auto& it: rts) {
        it->Add(0, 1);
      }
      for(auto& it: rts) {
        REQUIRE(it->Get(-200, 200) == expected1);
      }

      for(auto& it: rts) {
        it->Add(-2, 0);
      }
      for(auto& it: rts) {
        REQUIRE(it->Get(-200, 200) == expected2);
      }
  }

  SECTION("add, merge mid small") {
    std::vector<std::pair<int, int>> expected1 = {{1, 6}, {7, 8}};
    std::vector<std::pair<int, int>> expected2 = {{1, 8}};
    for(auto& it: rts) {
      it->Add(7, 8);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected1);
    }

    for(auto& it: rts) {
      it->Add(6, 7);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected2);
    }
  }


  SECTION("add, merge mid medium") {
    std::vector<std::pair<int, int>> expected1 = {{1, 6}, {8, 9}};
    std::vector<std::pair<int, int>> expected2 = {{1, 9}};
    for(auto& it: rts) {
      it->Add(8, 9);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected1);
    }

    for(auto& it: rts) {
      it->Add(6, 8);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected2);
    }
  }


  SECTION("add, merge mid large") {
    std::vector<std::pair<int, int>> expected1 = {{1, 6}, {9, 11}};
    std::vector<std::pair<int, int>> expected2 = {{1, 11}};
    for(auto& it: rts) {
      it->Add(9, 11);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected1);
    }

    for(auto& it: rts) {
      it->Add(6, 9);
    }
    for(auto& it: rts) {
      REQUIRE(it->Get(-200, 200) == expected2);
    }
  }

}
