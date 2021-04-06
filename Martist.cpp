#include "Martist.hpp"
#include "include/ExpressionTree.hpp"
#include <time.h>

#include <iostream>

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
  std::cout << "RED TREE" << std::endl;
  redTree.build();
  std::cout << "GREEN TREE" << std::endl;
  greenTree.build();
  std::cout << "BLUE TREE" << std::endl;
  blueTree.build();

  double redResult = redTree.plugVariables({ 0.5, -0.5 });
  double greenResult = greenTree.plugVariables({ 0.5, -0.5 });
  double blueResult = blueTree.plugVariables({ 0.5, -0.5 });

  printf("Red: %.2f, Green: %.2f, Blue: %.2f\n", redResult, greenResult, blueResult);
}

std::ostream& operator<<(std::ostream& out, const Martist& martist) {
  out << martist.redTree << '\n' << martist.greenTree << '\n' << martist.blueTree << '\n';

  return out;
}