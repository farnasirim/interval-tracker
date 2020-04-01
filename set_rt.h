#ifndef RT_SET_RT_H_
#define RT_SET_RT_H_

#include <cassert>

#include <set>

#include "range_tracker.h"
#include "debug.h"

namespace rt {

/**
 * Implementation of the RangeTracker interface using a set. Each interval
 * in the set is represented by two separate set entries. A start entry and
 * and end entry: e.g. [a, b) has two entries in the set:
 * pair(a, this_is_a_start_point) and pair(b, this_is_an_end_point)
 *
 * Invariant that is maintained by every operation: There are no two intervals
 * that touch or intersect eachother. I.e. no two [a, b) and [c, d) such that
 * b = c or c <= b <= d or c <= a <= b. Intervals with these conditions will
 * immediately be simplified, merged, etc.
 * An important property that this gives us is after finding a starting point
 * in the set, we can increment the set iterator to get the endpoint. Therefore
 * we can traverse the intervals, while being able to seamlessly think in terms
 * of points themselves.
 */
template<typename T>
class Set: public RangeTracker<T> {
 public:
  /**
   * Add works in amortized O(lgn) and worst case O(nlgn), because we may
   * end up removing many intervals that fall under the new interval.
   * Amortization argument is simple using e.g. the banker's method:
   * We place a coin on an interval when it's inserted into the set, and
   * use that coin we need to remove that interval for whatever reason.
   * Each set operation is O(lgn) (where n is the total number of intervals,
   * yielding the above complexities.
   */
  void Add(const T& old_a, const T& old_b) final override {
    auto a = old_a;
    auto b = old_b;
    if(intervals_.empty()) {
      intervals_.emplace(a, open);
      intervals_.emplace(b, close);
      return;
    }

    // Do the dirty work of handling intersections at the two endpoints of the
    // interval. There might be inserts/deletes here but there's only a constant
    // number of them: not a problem.
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

    // The above process has taken care of the clashes at the start of the
    // interval. Now we can loop over all the intervals that fall under this
    // new interval and delete them, "making room" for the new interval to be
    // inserted. We still have to be careful about the intersection with the
    // final endpoint.
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

    // Very similar to Add. We need to handle the edge cases in the two
    // endpoints.
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

    // Similar to Add.
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

  /* If [a, b) intersects an existing interval [c, d), we will have to return
   * [c, d), not the truncated version of [c, d) which falls under [a, b).
   */
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

  // any_low and any_high are used for binary search purposes to show
  // what we're interested in when we're using lower_bound, upper_bound.
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
