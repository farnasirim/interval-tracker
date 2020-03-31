#ifndef RT_SET_RT_H_
#define RT_SET_RT_H_

#include "range_tracker.h"

namespace rt {

template<typename T>
class Set: public RangeTracker<T> {
 public:
  void Add(const T& a, const T& b) final override {
  }

  void Delete(const T& a, const T& b) final override {
  }

  std::vector<std::pair<T, T>> Get(const T& a, const T& b) final override {
    std::vector<std::pair<T, T>> ret;

    return ret;
  }

};

}  // namespace rt
#endif  // RT_SET_RT_H_
