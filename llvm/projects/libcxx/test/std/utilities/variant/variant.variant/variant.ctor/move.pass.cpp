// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

// <variant>

// template <class ...Types> class variant;

// variant(variant&&) noexcept(see below);

#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

#include "test_macros.h"

struct ThrowsMove {
  ThrowsMove(ThrowsMove &&) noexcept(false) {}
};

struct NoCopy {
  NoCopy(const NoCopy &) = delete;
};

struct MoveOnly {
  int value;
  MoveOnly(int v) : value(v) {}
  MoveOnly(const MoveOnly &) = delete;
  MoveOnly(MoveOnly &&) = default;
};

struct MoveOnlyNT {
  int value;
  MoveOnlyNT(int v) : value(v) {}
  MoveOnlyNT(const MoveOnlyNT &) = delete;
  MoveOnlyNT(MoveOnlyNT &&other) : value(other.value) { other.value = -1; }
};

#ifndef TEST_HAS_NO_EXCEPTIONS
struct MakeEmptyT {
  static int alive;
  MakeEmptyT() { ++alive; }
  MakeEmptyT(const MakeEmptyT &) {
    ++alive;
    // Don't throw from the copy constructor since variant's assignment
    // operator performs a copy before committing to the assignment.
  }
  MakeEmptyT(MakeEmptyT &&) { throw 42; }
  MakeEmptyT &operator=(const MakeEmptyT &) { throw 42; }
  MakeEmptyT &operator=(MakeEmptyT &&) { throw 42; }
  ~MakeEmptyT() { --alive; }
};

int MakeEmptyT::alive = 0;

template <class Variant> void makeEmpty(Variant &v) {
  Variant v2(std::in_place_type<MakeEmptyT>);
  try {
    v = v2;
    assert(false);
  } catch (...) {
    assert(v.valueless_by_exception());
  }
}
#endif // TEST_HAS_NO_EXCEPTIONS

void test_move_noexcept() {
  {
    using V = std::variant<int, long>;
    static_assert(std::is_nothrow_move_constructible<V>::value, "");
  }
  {
    using V = std::variant<int, MoveOnly>;
    static_assert(std::is_nothrow_move_constructible<V>::value, "");
  }
  {
    using V = std::variant<int, MoveOnlyNT>;
    static_assert(!std::is_nothrow_move_constructible<V>::value, "");
  }
  {
    using V = std::variant<int, ThrowsMove>;
    static_assert(!std::is_nothrow_move_constructible<V>::value, "");
  }
}

void test_move_ctor_sfinae() {
  {
    using V = std::variant<int, long>;
    static_assert(std::is_move_constructible<V>::value, "");
  }
  {
    using V = std::variant<int, MoveOnly>;
    static_assert(std::is_move_constructible<V>::value, "");
  }
  {
    using V = std::variant<int, MoveOnlyNT>;
    static_assert(std::is_move_constructible<V>::value, "");
  }
  {
    using V = std::variant<int, NoCopy>;
    static_assert(!std::is_move_constructible<V>::value, "");
  }
}

void test_move_ctor_basic() {
  {
    std::variant<int> v(std::in_place_index<0>, 42);
    std::variant<int> v2 = std::move(v);
    assert(v2.index() == 0);
    assert(std::get<0>(v2) == 42);
  }
  {
    std::variant<int, long> v(std::in_place_index<1>, 42);
    std::variant<int, long> v2 = std::move(v);
    assert(v2.index() == 1);
    assert(std::get<1>(v2) == 42);
  }
  {
    std::variant<MoveOnly> v(std::in_place_index<0>, 42);
    assert(v.index() == 0);
    std::variant<MoveOnly> v2(std::move(v));
    assert(v2.index() == 0);
    assert(std::get<0>(v2).value == 42);
  }
  {
    std::variant<int, MoveOnly> v(std::in_place_index<1>, 42);
    assert(v.index() == 1);
    std::variant<int, MoveOnly> v2(std::move(v));
    assert(v2.index() == 1);
    assert(std::get<1>(v2).value == 42);
  }
  {
    std::variant<MoveOnlyNT> v(std::in_place_index<0>, 42);
    assert(v.index() == 0);
    std::variant<MoveOnlyNT> v2(std::move(v));
    assert(v2.index() == 0);
    assert(std::get<0>(v).value == -1);
    assert(std::get<0>(v2).value == 42);
  }
  {
    std::variant<int, MoveOnlyNT> v(std::in_place_index<1>, 42);
    assert(v.index() == 1);
    std::variant<int, MoveOnlyNT> v2(std::move(v));
    assert(v2.index() == 1);
    assert(std::get<1>(v).value == -1);
    assert(std::get<1>(v2).value == 42);
  }
}

void test_move_ctor_valueless_by_exception() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  using V = std::variant<int, MakeEmptyT>;
  V v1;
  makeEmpty(v1);
  V v(std::move(v1));
  assert(v.valueless_by_exception());
#endif
}

template <size_t Idx>
constexpr bool test_constexpr_ctor_extension_imp(
    std::variant<long, void*, const int> const& v)
{
  auto copy = v;
  auto v2 = std::move(copy);
  return v2.index() == v.index() &&
         v2.index() == Idx &&
        std::get<Idx>(v2) == std::get<Idx>(v);
}

void test_constexpr_move_ctor_extension() {
#ifdef _LIBCPP_VERSION
  using V = std::variant<long, void*, const int>;
  static_assert(std::is_trivially_copyable<V>::value, "");
  static_assert(std::is_trivially_move_constructible<V>::value, "");
  static_assert(test_constexpr_ctor_extension_imp<0>(V(42l)), "");
  static_assert(test_constexpr_ctor_extension_imp<1>(V(nullptr)), "");
  static_assert(test_constexpr_ctor_extension_imp<2>(V(101)), "");
#endif
}

int main() {
  test_move_ctor_basic();
  test_move_ctor_valueless_by_exception();
  test_move_noexcept();
  test_move_ctor_sfinae();
  test_constexpr_move_ctor_extension();
}
