#include "Martist.hpp"
#include "include/ExpressionTree.hpp"
#include <time.h>

Martist::Martist(
  std::uint8_t* buffer,
  std::size_t width,
  std::size_t height,
  std::size_t redDepth,
  std::size_t greenDepth,
  std::size_t blueDepth
) : buffer(buffer) {
  resize(width, height);
  redTree = ExpressionTree(redDepth);
  greenTree = ExpressionTree(greenDepth);
  blueTree = ExpressionTree(blueDepth);
  ExpressionTree::setVariables({ 'x', 'y' });
  ExpressionTree::setSeed(time(NULL));
}

void Martist::resize(std::size_t width, std::size_t height) {
  if (width == 0 || height == 0) throw std::domain_error("Width and height must be greater than 0");
  halfUnitX = 1.0 / width;
  halfUnitY = 1.0 / height;
}

void Martist::paint() {
  redTree.build();
  greenTree.build();
  blueTree.build();

  double redResult = redTree.plugVariables({ 0.5, -0.5 });
  double greenResult = greenTree.plugVariables({ 0.5, -0.5 });
  double blueResult = blueTree.plugVariables({ 0.5, -0.5 });

  printf("Red: %.2f, Green: %.2f, Blue: %.2f\n", redResult, greenResult, blueResult);
}