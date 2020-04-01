#include <cassert>
#include <cctype>

#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "range_tracker.h"
#include "trivial.h"
#include "set_rt.h"
#include "segtree_rt.h"

int main(int argc, char **argv) {
  using int_type = long long;
  assert(argc > 1);
  std::string mode(argv[1]);
  std::transform(mode.begin(), mode.end(), mode.begin(), tolower);
  if(mode.find("cmd") != std::string::npos) {
    assert(argc == 3);
    std::string requested_impl_name = argv[2];

    std::unique_ptr<rt::RangeTracker<int_type>> rt;

    std::transform(requested_impl_name.begin(), requested_impl_name.end(),
        requested_impl_name.begin(), tolower);

    if(requested_impl_name.find("seg") != std::string::npos) {
      rt = std::make_unique<rt::SegTree<int_type>>();
    } else if(requested_impl_name.find("set") != std::string::npos) {
      rt = std::make_unique<rt::Set<int_type>>();
    } else {
      rt = std::make_unique<rt::Trivial<int_type>>();
    }

    std::string cmd;
    int_type st, en;
    while(std::cin >> cmd >> st >> en) {
      std::transform(cmd.begin(), cmd.end(), cmd.begin(), tolower);
      if(cmd.find("a") != std::string::npos) { // add
        rt->Add(st, en);
      } else if(cmd.find("d") != std::string::npos) { // delete
        rt->Delete(st, en);
      } else { // get
        auto ret = rt->Get(st, en);
        std::cout << "[";
        bool is_first = true;
        for(auto& it: ret) {
          if(!is_first) {
            std::cout << " ";
          }
          is_first = false;
          std::cout << "(" << it.first << ", " << it.second << ")";
        }
        std::cout << "]";
        std::cout << std::endl;
      }
    }
  } else if(mode.find("perf") != std::string::npos) {

    std::vector<std::string> impl_names = {"set based", "seg tree"};

    srand(1231231231);

    std::vector<int> ranges = {1000, 100000, 1000000000};
    std::vector<std::string> type_name = {"1/3 uniform", "    90% get", "90% add/del"};
    std::vector<std::function<int()>> op = {
      []{ return rand() % 3 ; },
      []{ return std::min(rand() % 20, 2); },
      []{ auto val = rand(); if(val <= 1) return 2 ; else return val % 2 ; },
    };
    auto generator = [](int rng) { return static_cast<int>(rand() % (rng * 2) - rng) ; };
    auto interval_generator = [&generator] (int rng) {
      auto x = std::make_pair(generator(rng), generator(rng));
      if(x.first == x.second) {
        x.second += 1;
      }
      if(x.first > x.second) {
        std::swap(x.first, x.second);
      }
      return x;
    };

    int num_queries = 1e6;

    std::size_t ig = 0;

    std::cout << "number of queries: " << num_queries << std::endl;
    for(std::size_t i = 0; i < ranges.size(); i++) {
      for(std::size_t j = 0; j < op.size(); j++) {
        std::vector<std::tuple<int, int, int>> queries;
        for(int k = 0; k < num_queries; k++) {
          auto interval = interval_generator(ranges[i]);
          queries.emplace_back(op[j](), interval.first, interval.second);
        }
        std::cout << "|a|,|b| <= "
          << std::setw(10) << std::fixed << ranges[i]
          << ",    workload type:   " << type_name[j] << ":     ";

        std::vector<std::shared_ptr<rt::RangeTracker<int>>> impl = {
          std::make_shared<rt::Set<int>>(),
          std::make_shared<rt::SegTree<int>>()
        };
        for(std::size_t l = 0; l < impl.size(); l++) {
          auto& im = impl[l];
          auto start_time = std::chrono::high_resolution_clock::now();
          for(auto& q: queries) {
            if(std::get<0>(q) == 0) {
              im->Add(std::get<1>(q), std::get<2>(q));
            } else if(std::get<0>(q) == 1) {
              im->Delete(std::get<1>(q), std::get<2>(q));
            } else {
              ig ^= im->Get(std::get<1>(q), std::get<2>(q)).size();
            }
          }
          auto end_time = std::chrono::high_resolution_clock::now();
          std::cout << impl_names[l] << " : "
            << std::setprecision(5) << std::fixed
            << std::setw(3) << std::fixed
            << std::chrono::duration<double>(end_time - start_time).count()
            << "s" << "     ";
        }
        std::cout << std::endl;
      }
    }

    if(ig&1) {
      std::cout << std::endl;
    }
  } else {
    std::cout << "usage: " << std::endl;
    std::cout << argv[0] << " cmdline (segtree|set|trivial)" << std::endl;
  }
  return 0;
}
