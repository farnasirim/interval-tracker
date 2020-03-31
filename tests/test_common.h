#include "catch2/catch.hpp"

#include <memory>
#include <iostream>
#include <vector>

#include "range_tracker.h"
#include "trivial.h"

#define deb(x) std::cout << #x << " : " << x << std::endl

#define ALL_IMPLEMENTATIONS(data_type) \
  std::vector<std::shared_ptr<rt::RangeTracker<data_type>>>({ \
      std::make_shared<rt::Trivial<data_type>>(), \
    })

#define subvec(vec, from, to) \
  decltype(vec) ((vec).begin() + (from), (vec).begin() + (from) + (to))
