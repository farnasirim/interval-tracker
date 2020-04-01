#ifndef RT_SET_RT_H_
#define RT_SET_RT_H_

#include <cassert>

#include <set>

#include "range_tracker.h"
#include "debug.h"

namespace rt {

template<typename T>
class Set: public RangeTracker<T> {
 public:
  void Add(const T& old_a, const T& old_b) final override {
    auto a = old_a;
    auto b = old_b;
    if(intervals_.empty()) {
      intervals_.emplace(a, open);
      intervals_.emplace(b, close);
      return;
    }

    auto before_start = intervals_.upper_bound(std::make_pair(a, any_high));
    if(before_start != intervals_.begin()) {
      before_start --;
      assert(before_start->first <= a);
      if(before_start->second == open) {
        auto to_del = *before_start;
        a = std::min(a, to_del.first);
        before_start++;
        assert(before_start->second == close);
        assert(before_start->first > a);
        b = std::max(b, before_start->first);
        intervals_.erase(to_del);
        intervals_.erase(*before_start);
      } else if(before_start->first == a) {
        auto to_del = *before_start;
        before_start --;
        assert(before_start->second == open);
        a = before_start->first;
        intervals_.erase(to_del);
        intervals_.erase(*before_start);
      }
    }

    std::vector<std::pair<T, int>> to_dels;
    auto it = intervals_.lower_bound(std::make_pair(a, any_low));
    while(it != intervals_.end() && it->first <= b) {
      assert(it->second == open);
      to_dels.push_back(*it);
      it++;
      b = std::max(b, it->first);
      assert(it->second == close);
      to_dels.push_back(*it);
      it++;
    }
    for(auto& d: to_dels) {
      intervals_.erase(d);
    }

    intervals_.emplace(a, open);
    intervals_.emplace(b, close);
  }

  void Delete(const T& a, const T& b) final override {
    auto after_start = intervals_.upper_bound(std::make_pair(a, any_high));
    if(after_start != intervals_.end() && after_start->second == close) {
      auto maybe_del = *after_start;
      after_start --;
      if(after_start->first == a) {
        intervals_.erase(*after_start);
      } else {
        intervals_.emplace(a, close);
      }
      if(maybe_del.first > b) {
        intervals_.emplace(b, open);
      } else {
        intervals_.erase(maybe_del);
      }
    }


    auto it = intervals_.upper_bound(std::make_pair(a, any_high));
    std::vector<std::pair<T, int>> to_dels;
    while(it != intervals_.end() && it->first < b) {
      assert(it->second == open);
      to_dels.push_back(*it);
      it++;
      assert(it->second == close);
      if(it->first > b) {
        intervals_.emplace(b, open);
      } else {
        to_dels.push_back(*it);
      }
      it++;
    }
    for(auto& d: to_dels) {
      intervals_.erase(d);
    }
  }

  std::vector<std::pair<T, T>> Get(const T& a, const T& b) final override {
    std::vector<std::pair<T, T>> ret;

    auto after_start = intervals_.upper_bound(std::make_pair(a, any_high));
    if(after_start != intervals_.end()) {
      if(after_start->second == close) {
        after_start --;
      }
      while(after_start != intervals_.end() && after_start->first < b) {
        assert(after_start->second == open);
        auto st = after_start->first;
        after_start++;
        assert(after_start->second == close);
        auto en = after_start->first;
        ret.emplace_back(st, en);
        after_start++;
      }
    }

    return ret;
  }

 private:
  static constexpr int any_low = -1;
  static constexpr int open = 0;
  static constexpr int close = 1;
  static constexpr int any_high = 2;
  std::set<std::pair<T, int>> intervals_;
};

template<typename T> constexpr int Set<T>::any_low;
template<typename T> constexpr int Set<T>::any_high;
template<typename T> constexpr int Set<T>::open;
template<typename T> constexpr int Set<T>::close;

}  // namespace rt
#endif  // RT_SET_RT_H_
