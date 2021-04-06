#include "Martist.hpp"
#include "include/ExpressionTree.hpp"
#include <time.h>

#include <iostream>

// local functions

// Converts from -1,1 range to 0,255 range
static std::uint8_t convertFromRange(double number);

Martist::Martist(
  std::uint8_t* buffer,
  std::size_t width,
  std::size_t height,
  std::size_t redDepth,
  std::size_t greenDepth,
  std::size_t blueDepth
) : buffer(buffer) {
  resize(width, height);
  ExpressionTree::init({ 'x', 'y' });
  redTree = ExpressionTree(redDepth);
  greenTree = ExpressionTree(greenDepth);
  blueTree = ExpressionTree(blueDepth);
}

void Martist::resize(std::size_t width, std::size_t height) {
  if (width == 0 || height == 0) throw std::domain_error("Width and height must be greater than 0");
  halfUnitX = 1.0 / width;
  halfUnitY = 1.0 / height;
}

void Martist::paint() {
  // DEBUG
  // std::cout << "RED TREE" << std::endl;
  redTree.build();
  // std::cout << "GREEN TREE" << std::endl;
  greenTree.build();
  // std::cout << "BLUE TREE" << std::endl;
  blueTree.build();

  auto bufferIterator = buffer;

  // Steps throgh each pixel of the image. Does calculation to have the -1,1 range representation of this pixel's position
  // First pixel position is halfUnit - 1
  for (double xPosition = halfUnitX - 1; xPosition < 1.0; xPosition += 2 * halfUnitX) {
    for (double yPosition = halfUnitY - 1; yPosition < 1.0; yPosition += 2 * halfUnitY) {
      *bufferIterator++ = convertFromRange(redTree.plugVariables({ xPosition, yPosition }));
      *bufferIterator++ = convertFromRange(greenTree.plugVariables({ xPosition, yPosition }));
      *bufferIterator++ = convertFromRange(blueTree.plugVariables({ xPosition, yPosition }));
    }
  }
}

std::ostream& operator<<(std::ostream& out, const Martist& martist) {
  out << martist.redTree << '\n' << martist.greenTree << '\n' << martist.blueTree << '\n';

  return out;
}

static std::uint8_t convertFromRange(double number) {
  return (std::uint8_t)((number + 1.0) * 127.5);
}
