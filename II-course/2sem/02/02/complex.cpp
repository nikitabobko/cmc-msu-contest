#include <iostream>
#include <math.h>

using namespace std;

namespace numbers {
    class complex {
        double re, im;
    public:
        complex(double a = 0., double b = 0.) : re(a), im(b) {

        }

        explicit complex(const char *str) {
            if (str == nullptr || str == NULL) {
                re = 0;
                im = 0;
                return;
            }
            sscanf(str, "(%lf,%lf)", &re, &im);
        }

        operator string() const {
            return "(" + std::to_string(re) + "," + std::to_string(im) + ")";
        }

        double get_re() const {
            return re;
        }

        double get_im() const {
            return im;
        }

        double abs2() const {
            return re*re + im*im;
        }

        double abs() const {
            return sqrt(abs2());
        }

        char *to_string(char *buf, size_t size) const {
            if (buf == nullptr || buf == NULL) {
                return buf;
            }
            snprintf(buf, size, "(%.10g,%.10g)", re, im);
            return buf;
        }

        friend complex operator+(const complex &a, const complex &b);

        friend complex operator-(const complex &a, const complex &b);

        friend complex operator*(const complex &a, const complex &b);

        friend complex operator/(const complex &a, const complex &b);

        friend std::ostream &operator<<(std::ostream &out, const complex &elem) {
            out << "(" << elem.re << "," << elem.im << ")";
            return out;
        }

        complex operator~() const {
            return complex(re, -im);
        }

        complex operator-() const {
            return complex(-re, -im);
        }
    };

    complex operator+(const complex &a, const complex &b) {
        return complex(a.re + b.re, a.im + b.im);
    }

    complex operator-(const complex &a, const complex &b) {
        return complex(a.re - b.re, a.im - b.im);
    }

    complex operator*(const complex &a, const complex &b) {
        return complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
    }

    complex operator/(const complex &a, const complex &b) {
        complex tmp = ~b;
        complex ans = a * tmp;
        tmp = b * tmp;
        ans.re /= tmp.re;
        ans.im /= tmp.re;
        return ans;
    }
}