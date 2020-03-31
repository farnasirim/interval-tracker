#include "test_common.h"

TEST_CASE("RangeTracker objects must work correctly on the given get tests",
    "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  std::vector<std::pair<int, int>> empty = {};
  for(auto& it: rts) {
    it->Add(1, 3);
    it->Add(5, 6);
  }
  std::vector<std::pair<int, int>> existing = {{1, 3}, {5, 6}};

  for(auto& it: rts) {
    REQUIRE(it->Get(-200, 200) == existing);
    REQUIRE(it->Get(1, 2) == subvec(existing, 0, 1));
    REQUIRE(it->Get(-1, 1) == empty);
    REQUIRE(it->Get(2, 3) == subvec(existing, 0, 1));
    REQUIRE(it->Get(5, 6) == subvec(existing, 1, 1));
    REQUIRE(it->Get(6, 7) == empty);
  }

  SECTION("get, scenario 1") {
    for(auto& it: rts) {
      REQUIRE(it->Get(4, 5) == empty);
    }
  }

  SECTION("get, scenario 2") {
    for(auto& it: rts) {
      REQUIRE(it->Get(4, 6) == subvec(existing, 1, 1));
    }
  }

  SECTION("get, scenario 3") {
    for(auto& it: rts) {
      REQUIRE(it->Get(2, 9) == existing);
    }
  }
}

// i) State [(1, 3), (5, 6)]. Get(4, 5). Returns []
// ii) State [(1, 3), (5, 6)]. Get(4, 6). Returns [(5, 6)]
// iii) State [(1, 3), (5, 6)]. Get(2, 9). Returns [(1, 3), (5, 6)]
