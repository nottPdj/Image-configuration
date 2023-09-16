#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill):width_(w),height_(h), image_(w,std::vector<Color>(h,fill)){}//initializer list for width and height
  Image::~Image()
  {
  }
  int Image::width() const
  {
    return width_;
  }
  int Image::height() const
  {
    return height_;
  }
  Color& Image::at(int x, int y)
  {
    return image_[x][y];
  }

  const Color& Image::at(int x, int y) const
  {
    return image_[x][y];
  }
}
