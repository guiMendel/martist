#include "Martist.hpp"
#include <random>
#include <fstream>
#include <time.h>

#include <iostream>

#define width 600
#define height 600

using namespace std;

void makeImage(std::unique_ptr<uint8_t[]>& buffer, size_t w, size_t h) {
  ofstream file("test_image.ppm");

  file << "P6 " << w << " " << h << " 255\n";
  for (std::size_t i = 0; i < width * height * 3; ++i)
    file << buffer[i];

  file.close();
}

int main() {
  size_t size = width * height * 3 * sizeof(uint8_t);

  auto buffer = make_unique<uint8_t[]>(size);

  Martist martist(buffer.get(), width, height, 15, 15, 15);

  cout << martist.readUnitSizes() << endl;

  // martist.seed(2);

  martist.paint();

  cout << martist;

  makeImage(buffer, width, height);


  cout << martist.redDepth() << martist.greenDepth() << martist.blueDepth() << endl;

  // getchar();

  // auto buffer2 = make_unique<uint8_t[]>(size);

  // martist.changeBuffer(buffer2.get(), 40, 40);

  // martist.paint();

  // cout << martist;

  // makeImage(buffer2, 40, 40);


  // cin >> martist;

  // cout << martist;

  // makeImage(buffer);

  return 0;
}
