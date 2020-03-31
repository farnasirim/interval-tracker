#ifndef RT_TRIVIAL_H_
#define RT_TRIVIAL_H_

#include <set>

#include "range_tracker.h"

namespace rt {

template<typename T>
class Trivial : public RangeTracker<T> {
  bool intersect(std::pair<int, int> a, std::pair<int, int> b) {
    return !(b.second <= a.first || a.second <= b.first);
  }
 public:
  virtual void Add(const T& a, const T& b) {
    for(T i = a; i < b; i++) {
      axis_.insert(i);
    }
  }

  virtual void Delete(const T& a, const T& b) {
    for(T i = a; i < b; i++) {
      axis_.erase(i);
    }
  }

  virtual std::vector<std::pair<T, T>> Get(const T& a, const T& b) {
    std::vector<std::pair<T, T>> candidates, ret;

    for(auto it: axis_) {
      if(!candidates.empty() && candidates.back().second == it) {
        candidates.back().second = it + 1;
      } else {
        candidates.emplace_back(it, it + 1);
      }
    }
    for(auto& it: candidates) {
      if(intersect(it, std::make_pair(a, b))) {
        ret.push_back(it);
      }
    }
    return ret;

  }
 private:
  std::set<int> axis_;
};

}  // namespace rt
#endif  // RT_TRIVIAL_H_
