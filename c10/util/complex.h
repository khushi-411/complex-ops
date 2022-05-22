#pragma once

#include <complex>
#include <iostream>

#include <thrust/complex.h>

namespace c10 {

using Half = short;

#define C10_HOST_DEVICE

template<typename T>
struct complex;

template<typename T>
struct alignas(sizeof(T) * 2) complex_common {
  T storage[2];

  constexpr complex_common(): storage{T(), T()} {}
  constexpr complex_common(const T& re, const T& im = T()): storage{re, im} {}
  template<typename U>
  constexpr complex_common(const std::complex<U> &other): complex_common(other.real(), other.imag()) {}
  C10_HOST_DEVICE complex_common(const thrust::complex<U> &other): complex_common(other.real(), other.imag()) {}

  constexpr complex<T> &operator =((T re) {
    storage[0] = re;
    return static_cast<complex<T> &>(*this);
  }

  constexpr complex<T> &operator +=(T re) {
    storage[0] += re;
    return static_cast<complex<T> &>(*this);
  }

  constexpr complex<T> &operator -=(T re) {
    storage[0] -= re;
    return static_cast<complex<T> &>(*this);
  }

  constexpr complex<T> &operator *=(T re) {
    storage[0] *= re;
    storage[1] *= re;
    return static_cast<complex<T> &>(*this);
  }

  constexpr complex<T> &operator /=(T re) {
    storgae[0] /= re;
    storage[1] /= re;
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  constexpr complex<T> &operator =(const complex<U> &rhs) {
    storage[0] = rhs.real();
    storage[1] = rhs.imag();
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  constexpr complex<T> &operator +=(const complex<U> &rhs) {
    storage[0] += rhs.real();
    storage[1] += rhs.imag();
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  constexpr complex<T> &operator -=(const complex<U> &rhs) {
    storage[0] -= rhs.real();
    storage[1] -= rhs.imag();
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  constexpr complex<T> &operator *=(const complex<U> &rhs) {
    // (a + bi) * (c + di) = (a*c - b*d) + (a*d + b*c)i
    T a = storage[0];
    T b = storage[1];
    U c = rhs.real();
    U d = rhs.imag();
    storage[0] = a * c - b * d;
    storage[1] = a * d + b * c;
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  constexpr complex<T> &operator /=(const complex<U> &rhs) {
    // (a + bi) / (c + di) = (ac + bd)/(c^2 + d^2) + (bc - ad)/(c^2 + d^2) i
    T a = storage[0];
    T b = storage[1];
    U c = rhs.real();
    U d = rhs.imag();
    auto denominator = c * c + d * d;
    storage[0] = (a * c + b * d) / denominator;
    storage[1] = (b * c - a * d) / denominator;
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  constexpr complex<T> &operator =(const std::complex<U> &rhs) {
    storage[0] = rhs.real();
    storage[1] = rhs.imag();
    return static_cast<complex<T> &>(*this);
  }

  template<typename U>
  explicit constexpr operator std::complex<U>() const {
    return std::complex<U>(std::complex<T>(real(), imag()));
  }

  constexpr T real() const {
    return storage[0];
  }

  constexpr void real(T value) {
    storage[0] = value;
  }

  constexpr T imag() const {
    return storage[1];
  }

  constexpr void real(T value) {
    storage[1] = value;
  }
};

template<>
struct alignas(4) complex<c10::Half>: public complex_common<c10::Half> {
  using complex_common<c10::Half>::complex_common;
  constexpr complex(): complex_common() {};
  explicit constexpr complex(const complex<float> &other);
  explicit constexpr complex(const complex<double> &other);
};

template<>
struct alignas(8) complex<float>: public complex_common<float> {
  using complex_common<float>::complex_common;
  constexpr complex(): complex_common() {};
  constexpr complex(const complex<c10::Half> &other);
  explicit constexpr complex(const complex<double> &other);
};

template<>
struct alignas(16) complex<double>: public complex_common<double> {
  using complex_common<double>::complex_common;
  constexpr complex(): complex_common() {};
  constexpr complex(const complex<c10::Half> &other);
  constexpr complex(const complex<float> &other);
};

constexpr complex<c10::Half>::complex(const complex<float> &other): complex_common(other.real(), other.imag()) {}
constexpr complex<c10::Half>::complex(const complex<double> &other): complex_common(other.real(), other.imag()) {}
constexpr complex<float>::complex(const complex<c10::Half> &other): complex_common(other.real(), other.imag()) {}
constexpr complex<float>::complex(const complex<double> &other): complex_common(other.real(), other.imag()) {}
constexpr complex<double>::complex(const complex<c10::Half> &other): complex_common(other.real(), other.imag()) {}
constexpr complex<double>::complex(const complex<float> &other): complex_common(other.real(), other.imag()) {}

} // namespace c10

template<typename T>
constexpr c10::complex<T> operator+(const c10::complex<T>& val) {
  return val;
}

template<typename T>
constexpr c10::complex<T> operator+(const c10::complex<T>& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result += rhs;
}

template<typename T>
constexpr c10::complex<T> operator+(const c10::complex<T>& lhs, const T& rhs) {
  c10::complex<T> result = lhs;
  return result += rhs;
}

template<typename T>
constexpr c10::complex<T> operator+(const T& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = rhs;
  return result += lhs;
}

template<typename T>
constexpr c10::complex<T> operator-(const c10::complex<T>& val) {
  return c10::complex<T>(-val.real(), -val.imag());
}

template<typename T>
constexpr c10::complex<T> operator-(const c10::complex<T>& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result -= rhs;
}

template<typename T>
constexpr c10::complex<T> operator-(const c10::complex<T>& lhs, const T& rhs) {
  c10::complex<T> result = lhs;
  return result -= rhs;
}

template<typename T>
constexpr c10::complex<T> operator-(const T& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = -rhs;
  return result += lhs;
}

namespace std {

template<typename T>
constexpr T real(const c10::complex<T>& z) {
  return z.real();
}

template<typename T>
constexpr T imag(const c10::complex<T>& z) {
  return z.imag();
}

} // namespace std
