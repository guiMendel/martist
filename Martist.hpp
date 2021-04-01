#ifndef __MARTIST__
#define __MARTIST__

#include "include/ExpressionTree.hpp"
#include <cstdint>
#include <stdexcept>

class Martist {
public:
  Martist(std::uint8_t* buffer, std::size_t width,
    std::size_t height, std::size_t redDepth,
    std::size_t greenDepth, std::size_t blueDepth);

  // Change the buffer and update the image's sizes
  void changeBuffer(std::uint8_t* buffer, std::size_t width, std::size_t height) {
    this->buffer = buffer;
    resize(width, height);
  }

  // Red tree depth setter
  void redDepth(std::size_t depth) { redTree.setDepth(depth); }
  // Red tree depth getter
  std::size_t redDepth() const { return redTree.getDepth(); }

  // Green tree depth setter
  void greenDepth(std::size_t depth) { greenTree.setDepth(depth); }
  // Green tree depth getter
  std::size_t greenDepth() const { return greenTree.getDepth(); }

  // Blue tree depth setter
  void blueDepth(std::size_t depth) { blueTree.setDepth(depth); }
  // Blue tree depth getter
  std::size_t blueDepth() const { return blueTree.getDepth(); }

  // Sets the seed for all the color channel trees
  void seed(unsigned int seed) {
    redTree.setSeed(seed);
    greenTree.setSeed(seed);
    blueTree.setSeed(seed);
  }

  // Generates new image and paints it to the buffer
  void paint();

  //////////////////// TEST METHODS

  // Returns the unit sizes as a string
  std::string readUnitSizes() {
    return ("X: " + std::to_string(halfUnitX) + ", Y: " + std::to_string(halfUnitY));
  }

private:
  // Sets halfUnitX and halfUnitY in respect to the provided width and height
  void resize(std::size_t width, std::size_t height);

  // The image's buffer
  std::uint8_t* buffer;

  // The first value of X in the image array
  double halfUnitX;
  // The first value of Y in the image array
  double halfUnitY;

  // The red channel expression tree
  ExpressionTree redTree;
  // The green channel expression tree
  ExpressionTree greenTree;
  // The blue channel expression tree
  ExpressionTree blueTree;
};

#endif