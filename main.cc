#include <cassert>
#include <cctype>
#include <iostream>

#include <string>
#include <memory>

#include "range_tracker.h"
#include "trivial.h"
#include "set_rt.h"
#include "segtree_rt.h"

int main(int argc, char **argv) {
  assert(argc > 1);
  std::string mode(argv[1]);
  std::transform(mode.begin(), mode.end(), mode.begin(), tolower);
  if(mode.find("cmd") != std::string::npos) {
    assert(argc == 3);
    std::string requested_impl_name = argv[2];

    std::unique_ptr<rt::RangeTracker<long long>> rt;

    std::transform(requested_impl_name.begin(), requested_impl_name.end(),
        requested_impl_name.begin(), tolower);

    if(requested_impl_name.find("seg") != std::string::npos) {
      rt = std::make_unique<rt::SegTree<long long>>();
    } else if(requested_impl_name.find("set") != std::string::npos) {
      rt = std::make_unique<rt::Set<long long>>();
    } else {
      rt = std::make_unique<rt::Trivial<long long>>();
    }

    std::string cmd;
    long long st, en;
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
  } else {
    std::cout << "usage: " << std::endl;
    std::cout << argv[0] << " cmdline (segtree|set|trivial)" << std::endl;
  }
  return 0;
}
