#include "Color.hpp"
namespace prog {
    Color::Color(): r(0), g(0), b(0) {};
    
    Color::Color(const Color& other): r(other.r), g(other.g), b(other.b){};
    
    Color::Color(rgb_value red, rgb_value green, rgb_value blue):r(red),g(green),b(blue){};
    
    rgb_value Color::red() const {
      return r;
    }
    rgb_value Color::green() const {
      return g;
    }
    rgb_value Color::blue() const {
      return b;
    }
    rgb_value& Color::red() {
      return r;
    }
    rgb_value& Color::green() {
      return g;
    }
    rgb_value& Color::blue() {
      return b;
    }
    bool Color::operator==(const Color& c2) const {
      return r == c2.red() && g == c2.green() && b == c2.blue();
    }
}


