#include "../include/SpecReader.hpp"
#include "../include/ExpressionTree.hpp"

void SpecReader::read(char expression) {
  return;
}

std::unique_ptr<ExpressionNode> SpecReader::assembleTree() const {
  return std::make_unique<NullNode>();
}
