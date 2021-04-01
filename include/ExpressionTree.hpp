#ifndef __EXPRESSION_TREE__
#define __EXPRESSION_TREE__

#include <cstdint>
#include <time.h>
#include <random>
#include <memory>
#include "./ExpressionFactory.hpp"

#include <iostream>

struct ExpressionNode {
  // The expression to which this node corresponds
  const Expression expression;
  // The children nodes in the tree
  std::vector<std::unique_ptr<ExpressionNode>> children;

  ExpressionNode(const Expression& expression)
    : expression(expression) {
  }
};

class ExpressionTree {
public:
  ExpressionTree() = default;
  ExpressionTree(std::size_t depth) : depth(depth) {}

  // Get the tree's depth
  std::size_t getDepth() const { return depth; }

  // Overwrite the tree's depth. It returns the new depth.
  std::size_t setDepth(std::size_t newDepth) { return depth = newDepth; }

  // Set the seed for all trees.
  static void setSeed(unsigned int seed) { randomEngine.seed(seed); }

  // Set the variable representations to be used by all trees
  static void setVariables(std::vector<char> newVariables) { variables = newVariables; }

  // Builds a randomly generated tree
  void build();

  // Performs the tree's expressions on the provided variables
  double plugVariables(std::vector<double> variables);

private:
  // Recursively builds a node and is children
  std::unique_ptr<ExpressionNode> grow(std::size_t remainingDepth);

  // Makes a leaf node expression
  static std::unique_ptr<ExpressionNode> makeLeafExpression();

  // Makes a single branch node expression
  static std::unique_ptr<ExpressionNode> makeSingleExpression();

  // Makes a double branch node expression
  static std::unique_ptr<ExpressionNode> makeDoubleExpression();

  // Has chance% chance to return true
  bool likelihood(double chance);

  // Returns a double representing the proportion of the current depth in relation to total depth
  double progress(std::size_t remainingDepth) { return 1.0 - double(remainingDepth) / double(depth); }

  // The root node
  std::unique_ptr<ExpressionNode> head;

  // The tree's depth
  std::size_t depth;

  // The tree's variables
  inline static std::vector<char> variables;

  // The tree's random engine
  inline static std::default_random_engine randomEngine;

  //////////////////////////////// PROBABILITY CONFIGURATION

  // Chance upon construction of each node to be a leaf node
  double leafLikelihood(std::size_t remainingDepth) {
    double result = progress(remainingDepth);
    std::cout << "leafLikelihood: " << result << std::endl;
    return result;
  }

  // Chance upon construction of each node to be a single branch node
  double singleBranchLikelihood(std::size_t remainingDepth) {
    double result = 1.0 - progress(remainingDepth) * 0.75;
    std::cout << "singleBranchLikelihood: " << result << std::endl;
    return result;
  }

  // Chance upon construction of each node to be a double branch node
  double doubleBranchLikelihood(std::size_t remainingDepth) {
    double result = 1.0 - progress(remainingDepth);
    std::cout << "doubleBranchLikelihood: " << result << std::endl;
    return result;
  }

  ////////////////// TYPES
public:
  typedef decltype(&makeLeafExpression) nodeMaker;
};

#endif