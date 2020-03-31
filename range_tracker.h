#ifndef _RT_RANGE_TRACKER_H_
#define _RT_RANGE_TRACKER_H_

#include <vector>
#include <algorithm>

namespace rt {

template<typename T>
/** Tracks the binary state of being marked/unmarked for half-open
 * intervals [a, b).
 * Client must hold condition a < b in all APIs.
 */
class RangeTracker {
 public:
  /**
   * Mark the interval [a, b). Any other existing interval that intersects
   * with this one will be merged into it.
   */
  virtual void Add(const T& a, const T& b) = 0;

  /**
   * Unmark the interval [a, b). If an interval intersects with this one, the
   * intersecting part will be unmarked, resulting in the other interval being
   * shrinked down.
   * If an already marked interval [c, d) "properly" contains [a, b), meaning
   * c < a < b < d, the deletion of [a, b) will result in [c, d) being
   * partitioned into two disjoint parts, namely [c, a) and [b, d),
   * since the ranges can only be defined with integer values based on the
   * problem description.
   */
  virtual void Delete(const T& a, const T& b) = 0;

  /**
   * Return all intervals that intersect [a, b)
   *
   * Method not const qualified because the details of the implementation
   * of Get could be different based on the concrete datastructure.
   * Returns an std::vector<std::pair<T, T>> because for now I couldn't find
   * enough reason to templatize the result container.
   */
  virtual std::vector<std::pair<T, T>> Get(const T& a, const T& b) = 0;
};

}  // namespace rt
#endif  //_RT_RANGE_TRACKER_H_
