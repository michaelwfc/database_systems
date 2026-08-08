
#include <cstdint>
#include <iostream>

class CountMinSketch {

public:
  CountMinSketch() : width_(0), row_(0){}
  CountMinSketch(uint32_t width, uint32_t row) : width_(width), row_(row) {}

  // move constructor
  CountMinSketch(&&other){}
  


private:
  uint32_t width_;
  uint32_t row_;
};