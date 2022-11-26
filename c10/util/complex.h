#pragma once

#include <complex>
#include <iostream>


namespace c10 {

using Half = short;

#define C10_HOST_DEVICE

struct alignas(sizeof(T) * 2) complex {

  // https://stackoverflow.com/questions/44571362
  using value_type = T;

  T real_ = T(0); // TODO: Use {0}?
  T imag_ = T(0);

  // constructor declaration
  constexpr complex() = default;
  constexpr complex(const T& re, const T& im = T())
      : real_(re), imag_(im) {}
  // Using real and imag from std::complex
  // TODO: Why do we write explicit keyword?
  template<typename U>
  explicit constexpr complex(const std::complex<U> &other)
      : complex_common(other.real(), other.imag()) {}

  // Casting constructor
  // TODO: Meaning of U = T
  // I don't think we need to define using C10_HOST_DEVICE
  template <typename U = T>
  C10_HOST_DEVICE constexpr complex(
      const std::enable_if_t<std::is_same<U, float>::value, complex<double>>&
          other)
      : real_(other.real_), imag_(other.imag_) {}
  template <typename U = T>
  C10_HOST_DEVICE constexpr complex(
      const std::enable_if_t<std::is_same<U, double>::value, complex<float>>&
          other)
      : real_(other.real_), imag_(other.imag_) {}

  // TODO: Is this a template template parameter?
  // TODO: Why not return static_cast<complex<T> &>(*this)?
  constexpr complex<T> &operator=(T re) {
    real_ = re;
    imag_ = 0;
    return *this;
  }

  constexpr complex<T> &operator+=(T re) {
    real_ += re;
    return *this;
  }

  constexpr complex<T> &operator-=(T re) {
    real_ -= re;
    return *this;
  }

  constexpr complex<T> &operator*=(T re) {
    real_ *= re;
    imag_ *= re;
    return *this;
  }

  constexpr complex<T> &operator/=(T re) {
    real_ /= re;
    imag_ /= re;
    return *this;
  }

  template<typename U>
  constexpr complex<T> &operator=(const complex<U> &rhs) {
    real_ = rhs.real();
    imag_ = rhs.imag();
    return *this;
  }

  template<typename U>
  constexpr complex<T> &operator+=(const complex<U> &rhs) {
    real_ += rhs.real();
    imag_ += rhs.imag();
    return *this;
  }

  template<typename U>
  constexpr complex<T> &operator-=(const complex<U> &rhs) {
    real_ -= rhs.real();
    imag_ -= rhs.imag();
    return *this;
  }

  template<typename U>
  constexpr complex<T> &operator*=(const complex<U> &rhs) {
    // (a + bi) * (c + di) = (a*c - b*d) + (a*d + b*c)i
    T a = real_;
    T b = imag_;
    U c = rhs.real();
    U d = rhs.imag();
    real_ = a * c - b * d;
    imag_ = a * d + b * c;
    return *this;
  }

  // TODO: Why use <typename U>?
  template<typename U>
  constexpr complex<T> &operator/=(const complex<U> &rhs) {
    // (a + bi) / (c + di) = (ac + bd)/(c^2 + d^2) + (bc - ad)/(c^2 + d^2) i
    T a = real_;
    T b = imag_;
    U c = rhs.real();
    U d = rhs.imag();
    auto denominator = c * c + d * d;
    real_ = (a * c + b * d) / denominator;
    imag_ = (b * c - a * d) / denominator;
    return *this;
  }

  template <typename U>
  constexpr complex<T> &operator=(const std::complex<U> &rhs) {
    real_ = rhs.real();
    imag_ = rhs.imag();
    return *this;
  }

  // TODO
  template <typename U>
  explicit constexpr operator std::complex<U>() const {
    return std::complex<U>(std::complex<T>(real(), imag()));
  }

  constexpr T real() const {
    return real_;
  }

  constexpr void real(T value) {
    real_ = value;
  }

  constexpr T imag() const {
    return imag_;
  }

  constexpr void real(T value) {
    imag_ = value;
  }
};

// TODO: difference between template<> and template <typename T>?
template <>
struct alignas(4) complex<Half> {
  Half real_;
  Half imag_;

  // Constructors
  complex() = default;

  // Half Constructor is not a constexpr
  C10_HOST_DEVICE explicit inline complex(const Half& real, const Half& imag)
      : real_(real), imag_(imag) {}
  C10_HOST_DEVICE inline complex(const c10::complex<float> &other)
      : real_(other.real()), imag_(other.imag()) {}

  // conversion operator
  inline C10_HOST_DEVICE operator c10::complex<float>() const {
      return {real_, imag_};
  }

  constexpr C10_HOST_DEVICE Half real() const {
    return real_;
  }
  constexpr C10_HOST_DEVICE Half imag() const {
    return imag_;
  }

  complex<Half>& operator+=(const complex<Half>& other) {
    real_ = static_cast<float>(real_) + static_cast<float>(other.real_);
    imag_ = static_cast<float>(imag_) + static_cast<float?(other.imag_);
    return *this;
  }

  complex<Half>& operator-=(const complex<Half>& other) {
    real_ = static_cast<float>(real_) - static_cast<float>(other.real_);
    imag_ = static_cast<float>(imag_) - static_cast<float>(other.imag_);
    return *this;
  }

  complex<Half>& operator*=(const complex<Half>& other) {
    auto a = static_cast<float>(real_);
    auto b = static_cast<float>(imag_);
    auto c = static_cast<float>(other.real());
    auto d = static_cast<float>(other.imag());
    real_ = a * c - b * d;
    imag_ = a * d + b * c;
    return *this;
  }
};

template<>
struct alignas(8) complex<float> {
  Half real_;
  Half imag_;

  // Constructors
  complex() = default;


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


// TODO: Add ostream and istream

} // namespace c10

template <typename T>
constexpr c10::complex<T> operator+(const c10::complex<T>& val) {
  return val;
}

template <typename T>
constexpr c10::complex<T> operator+(const c10::complex<T>& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result += rhs;
}

template <typename T>
constexpr c10::complex<T> operator+(const c10::complex<T>& lhs, const T& rhs) {
  c10::complex<T> result = lhs;
  return result += rhs;
}

template <typename T>
constexpr c10::complex<T> operator+(const T& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = rhs;
  return result += lhs;
}

template <typename T>
constexpr c10::complex<T> operator-(const c10::complex<T>& val) {
  return c10::complex<T>(-val.real(), -val.imag());
}

template <typename T>
constexpr c10::complex<T> operator-(const c10::complex<T>& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result -= rhs;
}

template <typename T>
constexpr c10::complex<T> operator-(const c10::complex<T>& lhs, const T& rhs) {
  c10::complex<T> result = lhs;
  return result -= rhs;
}

template <typename T>
constexpr c10::complex<T> operator-(const T& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = -rhs;
  return result += lhs;
}

template <typename T>
constexpr c10::complex<T> operator*(const complex<T>& lhs, const complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result *= rhs;
}

template <typename T>
constexpr c10::complex<T> operator*(const complex<T>& lhs, const T& rhs) {
  c10::complex<T> result = rhs;
  return result *= lhs;
}

template <typename T>
constexpr c10::complex<T> operator*(const T& lhs, complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result *= rhs;
}

template <typename T>
constexpr c10::complex<T> operator/(const c10::complex<T>& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result = lhs;
  return result /= rhs;
}

template <typename T>
constexpr c10::complex<T> operator/(const c10::complex<T>& lhs, const T& rhs) {
  c10::complex<T> result = lhs;
  return result /= rhs;
}

template <typename T>
constexpr c10::complex<T> operator/(const T& lhs, const c10::complex<T>& rhs) {
  c10::complex<T> result(lhs, T());
  return result /= rhs;
}

namespace std {

template <typename T>
constexpr T real(const c10::complex<T>& z) {
  return z.real();
}

template <typename T>
constexpr T imag(const c10::complex<T>& z) {
  return z.imag();
}

} // namespace std
