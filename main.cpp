#include "Martist.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>


void makeImage(uint8_t* buffer, size_t w, size_t h) {
  std::ofstream file("test_image.ppm");

  file << "P6 " << w << " " << h << " 255\n";
  for (std::size_t i = 0; i < w * h * 3; ++i)
    file << buffer[i];

  file.close();
}

int main() {
  constexpr std::size_t WIDTH = 2, HEIGHT = 2;
  std::uint8_t buf[WIDTH * HEIGHT * 3];
  Martist martist(buf, WIDTH, HEIGHT, 1, 1, 1);
  std::string spec("xya\nxya\nxya\n");
  std::istringstream in(spec);
  in >> martist;

  makeImage(buf, WIDTH, HEIGHT);

  std::cout << int(buf[0]) << std::endl;
  std::cout << int(buf[1]) << std::endl;
  std::cout << int(buf[2]) << std::endl;
  // std::cout << buf[0] << std::endl;

  assert(std::abs(int(buf[0]) - 128) <= 1);
  assert(std::abs(int(buf[1]) - 128) <= 1);
  assert(std::abs(int(buf[2]) - 128) <= 1);

  assert(std::abs(int(buf[3]) - 191) <= 1);
  assert(std::abs(int(buf[4]) - 191) <= 1);
  assert(std::abs(int(buf[5]) - 191) <= 1);

  assert(std::abs(int(buf[6]) - 64) <= 1);
  assert(std::abs(int(buf[7]) - 64) <= 1);
  assert(std::abs(int(buf[8]) - 64) <= 1);

  assert(std::abs(int(buf[9]) - 128) <= 1);
  assert(std::abs(int(buf[10]) - 128) <= 1);
  assert(std::abs(int(buf[11]) - 128) <= 1);
  return 0;
}
