#include "Martist.hpp"
#include <iostream>
#include <cassert>
int main() {
  constexpr std::size_t WIDTH = 320, HEIGHT = 240;
  std::uint8_t buf[WIDTH * HEIGHT * 3];
  for (std::size_t i = 0; i < WIDTH * HEIGHT * 3; ++i)
    buf[i] = i % 256; // Fill buf with mostly non-zero bytes
  Martist martist(buf, WIDTH, HEIGHT, 0, 0, 0);
  martist.paint();
  // buf should be filled black (i.e. zero-filled)
  for (std::size_t i = 0; i < WIDTH * HEIGHT * 3; ++i) {
    assert(buf[i] == 0);
  }
  return 0;
}