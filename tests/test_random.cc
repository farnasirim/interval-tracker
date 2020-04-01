#include "test_common.h"

TEST_CASE("RangeTracker objects must perform similar to the trivial "
    "implementation when given random tests", "[RangeTracker]") {

  auto rts = ALL_IMPLEMENTATIONS(int);
  auto ground_truth = rts[0];
  auto to_test = subvec(rts, 1, rts.size() - 1);

  SECTION("random 1") {
    srand(123123123);
    int mn = -20, mx = 20;
    int sz = mx - mn + 1;
    for(int i = 0; i < 1000; i++) {
      auto cmd = rand() % 3;
      int fr = rand() % sz - mn;
      int to = rand() % sz - mn;
      if(fr > to) {
        std::swap(fr, to);
      } else if(fr == to) {
        to += 1;
      }
      if(cmd == 0) {
        ground_truth->Add(fr, to);
        for(auto& it: to_test) {
          it->Add(fr, to);
        }
      } else if(cmd == 1) {
        ground_truth->Delete(fr, to);
        for(auto& it: to_test) {
          it->Delete(fr, to);
        }
      } else {
        auto res = ground_truth->Get(fr, to);
        for(auto& it: to_test) {
          REQUIRE(it->Get(fr, to) == res);
        }
      }
    }
  }

  SECTION("random 2") {
    srand(987123);
    int mn = -200, mx = 200;
    int sz = mx - mn + 1;
    for(int i = 0; i < 1000; i++) {
      auto cmd = rand() % 3;
      int fr = rand() % sz - mn;
      int to = rand() % sz - mn;
      if(fr > to) {
        std::swap(fr, to);
      } else if(fr == to) {
        to += 1;
      }
      if(cmd == 0) {
        ground_truth->Add(fr, to);
        for(auto& it: to_test) {
          it->Add(fr, to);
        }
      } else if(cmd == 1) {
        ground_truth->Delete(fr, to);
        for(auto& it: to_test) {
          it->Delete(fr, to);
        }
      } else {
        auto res = ground_truth->Get(fr, to);
        for(auto& it: to_test) {
          REQUIRE(it->Get(fr, to) == res);
        }
      }
    }
  }

  SECTION("random 3") {
    srand(100001201);
    int mn = -2000, mx = 2000;
    int sz = mx - mn + 1;
    for(int i = 0; i < 10000; i++) {
      auto cmd = rand() % 3;
      int fr = rand() % sz - mn;
      int to = rand() % sz - mn;
      if(fr > to) {
        std::swap(fr, to);
      } else if(fr == to) {
        to += 1;
      }
      if(cmd == 0) {
        ground_truth->Add(fr, to);
        for(auto& it: to_test) {
          it->Add(fr, to);
        }
      } else if(cmd == 1) {
        ground_truth->Delete(fr, to);
        for(auto& it: to_test) {
          it->Delete(fr, to);
        }
      } else {
        auto res = ground_truth->Get(fr, to);
        for(auto& it: to_test) {
          REQUIRE(it->Get(fr, to) == res);
        }
      }
    }
  }

}
