// xref: https://stackoverflow.com/questions/21281460
// xref: https://stackoverflow.com/questions/10712380

#include <iostream>

class Complex
{
    private:
        friend std::ostream& operator<<(std::ostream&, const Complex&);
        friend std::istream& operator>>(std::istream&, Complex&);
        double real;
        double imag;
    public:
        Complex(double = 0.0, double = 0.0); // Constructor
        Complex operator+(const Complex&) const;
        Complex operator-(const Complex&) const;
        const bool operator^(const Complex&) const;
	const bool operator&&(const Complex&) const;
	const bool operator||(const Complex&) const;
        // error: declaration of 'operator^' as non-function
        // const bool operator^^(const Complex&) const;
        // won't work because we can't create own operators in C++
        const bool logical_xor(const Complex& op1, const Complex& op2) const {
            return (bool(real) || bool(imag)) != (bool(op1.real) || bool(op1.imag));
        }
};

// Constructor
Complex::Complex(double r, double i)
{
    this->real = r;
    this->imag = i;
}

std::ostream& operator<<(std::ostream& output, const Complex& complex) {
    output << '(' << complex.real << ',' << complex.imag << ')';
    return output;
}

std::istream &operator>>(std::istream& input, Complex &complex) {
    input.ignore();
    input >> complex.real;
    input.ignore();
    input >> complex.imag;
    input.ignore();
    return input;
}

Complex Complex::operator+(const Complex& op) const
{
    return Complex(real + op.real, imag + op.imag);
}

Complex Complex::operator-(const Complex& op) const
{
    return Complex(real - op.real, imag - op.imag);
}

const bool Complex::operator&&(const Complex& op) const
{
    return (bool(real) || bool(imag)) && (bool(op.real) || bool(op.imag));
}

const bool Complex::operator||(const Complex& op) const
{
    return (real || imag) || (op.real || op.imag);
}

const bool Complex::operator^(const Complex& op) const
{
    return (bool(real) || bool(imag)) != (bool(op.real) || bool(op.imag));
}

// const bool Complex::operator^^(const Complex& op) const
// {
//     return (bool(real) || bool(imag)) != (bool(op.real) || bool(op.imag));
// }

//  const bool logical_xor(const Complex& op) const
//  {
//      return (bool(real) || bool(imag)) != (bool(op.real) || bool(op.imag));
//  }

int main() {
    Complex c1(1, 3);
    Complex c2(3, 5);
    Complex c3 = c1 + c2;
    Complex c4 = c1 ^ c2;
    Complex c5;
    c5.logical_xor(c1, c2);
    // Complex c6 = c1 ^^ c2;
    Complex c7 = c1 - c2;
    Complex c8 = c1 && c2;
    Complex c9 = c1 || c2;
    std::cout << c3 << std::endl;
    std::cout << c4 << std::endl;
    std::cout << c5 << std::endl;
    // std::cout << c6 << std::endl;
    std::cout << c7 << std::endl;
    std::cout << c8 << std::endl;
    std::cout << c9 << std::endl;
    return 0;
}
