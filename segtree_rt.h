#ifndef RT_SEGTREE_RT_H_
#define RT_SEGTREE_RT_H_

#include <limits>

#include "range_tracker.h"

namespace rt {

template<typename T>
class STNode {
 public:
  void set(T l, T r, T ql, T qr, bool v) {
    if(ql <= l && r <= qr) {
      set_to(v);
    } else if(!(r <= ql || qr <= l)) {
      propagate();
      T md = (l + r)/2;
      left_->set(l, md, ql, qr, v);
      right_->set(md, r, ql, qr, v);
      ask();
    }
  }

  std::vector<std::pair<T, T>> get(T l, T r, T ql, T qr) {
    if(ql <= l && r <= qr && (!has_set_ || is_all_set_)) {
      if(is_all_set_) {
        return {{l, r}};
      }
      return {};
    } else if(!(r <= ql || qr <= l)) {
      propagate();
      T md = (l + r)/2;
      auto lef = left_->get(l, md, ql, qr);
      auto rig = right_->get(md, r, ql, qr);
      if(!lef.empty() &&
          !rig.empty() &&
          lef.back().second == rig.front().first) {
        lef.back().second = rig.front().second;
        rig.erase(rig.begin());
      }
      for(auto& it: rig) {
        lef.push_back(it);
      }
      ask();
      return lef;
    }
    return {};
  }

  T expand_left(T a, T b, T q) {
    auto res = q;
    if(q < a) {
      res = q;
    } else if(is_all_set_) {
      res = std::min(q, a);
    } else if(has_set_) {
      T md = (a + b)/2;
      res = std::min(res, right_->expand_left(md, b, q));
      if(res <= md) {
        res = std::min(res, left_->expand_left(a, md, q));
      }
      ask();
    }
    return res;
  }

  T expand_right(T a, T b, T q) {
    auto res = q;
    if(b < q) {
      res = q;
    } else if(is_all_set_) {
      res = std::max(q, b);
    } else if(has_set_) {
      propagate();
      T md = (a + b)/2;
      res = std::max(res, left_->expand_right(a, md, q));
      if(res >= md) {
        res = std::max(res, right_->expand_right(md, b, q));
      }
      ask();
    }
    return res;
  }


 private:
  void propagate() {
    if(left_ == nullptr) {
      left_ = std::make_unique<STNode<T>>();
      left_->set_to(is_all_set_);
    }
    if(right_ == nullptr) {
      right_ = std::make_unique<STNode<T>>();
      right_->set_to(is_all_set_);
    }
  }

  void set_to(bool v) {
    is_all_set_ = has_set_ = v;
    left_.reset(nullptr);
    right_.reset(nullptr);
  }

  void ask() {
    is_all_set_ = left_->is_all_set_ & right_->is_all_set_;
    has_set_ = left_->has_set_ | right_->has_set_;
    if(is_all_set_ || !has_set_) {
      left_.reset(nullptr);
      right_.reset(nullptr);
    }
  }
  std::unique_ptr<STNode<T>> left_;
  std::unique_ptr<STNode<T>> right_;
  bool has_set_;
  bool is_all_set_;
};

template<typename T>
class SegTree: public RangeTracker<T> {
  static constexpr T mn = std::numeric_limits<T>::min()/2 + 1;
  static constexpr T mx = std::numeric_limits<T>::max()/2 - 1;
 public:
  SegTree(): root_(std::make_unique<STNode<T>>()) {

  }

  void Add(const T& a, const T& b) final override {
    root_->set(mn, mx, a, b, true);
  }

  void Delete(const T& a, const T& b) final override {
    root_->set(mn, mx, a, b, false);
  }

  std::vector<std::pair<T, T>> Get(const T& a, const T& b) final override {
    auto ret = root_->get(mn, mx, a, b);
    if(!ret.empty()) {
      {
        auto lef = root_->expand_left(mn, mx, ret.front().first);
        ret.front().first = lef;
      }
      {
        auto rig = root_->expand_right(mn, mx, ret.back().second);
        ret.back().second = rig;
      }
    }
    return ret;
  }

 private:
  std::unique_ptr<STNode<T>> root_;
};

template<typename T> constexpr T SegTree<T>::mn;
template<typename T> constexpr T SegTree<T>::mx;

}  // namespace rt
#endif  // RT_SEGTREE_RT_H_
