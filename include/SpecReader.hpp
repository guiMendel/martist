#ifndef __SPEC_READER__
#define __SPEC_READER__

#include "ExpressionTree.hpp"

class SpecReader {
public:
  // Reads an expression and pushes it to the pile
  void read(char expression);

  // Assembles a tree from the specs read
  std::unique_ptr<ExpressionNode> assembleTree() const;
};

#endif