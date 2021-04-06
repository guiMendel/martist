#ifndef __EXPRESSION_TREE__
#define __EXPRESSION_TREE__

#include <cstdint>
#include <time.h>
#include <random>
#include <memory>
#include <functional>
#include "./ExpressionFactory.hpp"

#include <iostream>

struct ExpressionNode {
  // The expression to which this node corresponds
  Expression expression;

  ExpressionNode() = default;

  // Returns the result of evaluating this expression over the provided variables
  virtual double evaluate(std::vector<double>& variables) = 0;
};

struct NullNode : ExpressionNode {
  virtual double evaluate(std::vector<double>&) { return 0; }
};

struct LeafNode : ExpressionNode {
  LeafNode(std::vector<char>& variableRepresentations, std::default_random_engine& engine);

  virtual double evaluate(std::vector<double>& variables) { return variables[expression.variableIndex]; }
};

struct SingleNode : ExpressionNode {
  std::unique_ptr<ExpressionNode> child;

  SingleNode(std::vector<Expression>& availableExpressions, std::default_random_engine& engine,
    std::unique_ptr<ExpressionNode> child);

  virtual double evaluate(std::vector<double>& variables) { return expression.singleFunction(child->evaluate(variables)); }
};

struct DoubleNode : ExpressionNode {
  std::unique_ptr<ExpressionNode> child1;
  std::unique_ptr<ExpressionNode> child2;

  DoubleNode(std::vector<Expression>& availableExpressions, std::default_random_engine& engine,
    std::unique_ptr<ExpressionNode> child1, std::unique_ptr<ExpressionNode> child2);

  virtual double evaluate(std::vector<double>& variables) {
    return expression.doubleFunction(child1->evaluate(variables), child2->evaluate(variables));
  }
};

class ExpressionTree {
public:
  ExpressionTree() = default;
  ExpressionTree(std::size_t depth) : depth(depth) {}

  static void init(std::vector<char> variables, unsigned int seed = time(NULL));

  // Get the tree's depth
  std::size_t getDepth() const { return depth; }

  // Overwrite the tree's depth. It returns the new depth.
  std::size_t setDepth(std::size_t newDepth) { return depth = newDepth; }

  // Set the seed for all trees.
  static void setSeed(unsigned int seed) { randomEngine.seed(seed); }

  // Set the variable representations to be used by all trees
  static void setVariables(std::vector<char>& newVariables) { variables = newVariables; }

  // Builds a randomly generated tree
  void build();

  // Performs the tree's expressions on the provided variables
  double plugVariables(std::vector<double> variables);

private:
  // Recursively builds a node and its children
  std::unique_ptr<ExpressionNode> grow(std::size_t remainingDepth);

  /////////// NODE MAKERS

  // Makes a leaf node expression
  std::unique_ptr<ExpressionNode> makeLeafExpression(std::size_t) {
    std::cout << "-> makeLeafExpression" << std::endl;
    return std::make_unique<LeafNode>(variables, randomEngine);
  }

  // Makes a single branch node expression
  std::unique_ptr<ExpressionNode> makeSingleExpression(std::size_t remainingDepth) {
    std::cout << "-> makeSingleExpression" << std::endl;
    return std::make_unique<SingleNode>(singleExpressions, randomEngine, grow(remainingDepth - 1));
  }

  // Makes a double branch node expression
  std::unique_ptr<ExpressionNode> makeDoubleExpression(std::size_t remainingDepth) {
    std::cout << "-> makeDoubleExpression" << std::endl;
    return std::make_unique<DoubleNode>(doubleExpressions, randomEngine, grow(remainingDepth - 1), grow(remainingDepth - 1));
  }

  /////////// PROBABILITY CALCULATORS

  // Has chance% chance to return true
  bool likelihood(double chance);

  // Returns a double representing the proportion of the current depth in relation to total depth
  double progress(std::size_t remainingDepth) { return 1.0 - double(remainingDepth) / double(depth); }

  ////////// ATTRIBUTES

  // The root node
  std::unique_ptr<ExpressionNode> head;

  // The tree's depth
  std::size_t depth;

  // Indicates whether the class has been initialized
  inline static bool initialized = false;

  // All the trees' variable representations
  inline static std::vector<char> variables;

  // All the trees' random engine
  inline static std::default_random_engine randomEngine;

  // List of possible single expressions
  inline static std::vector<Expression> singleExpressions;

  // List of possible double expressions
  inline static std::vector<Expression> doubleExpressions;

  //////////////////////////////// PROBABILITY CONFIGURATION

  // Chance upon construction of each node to be a leaf node
  double leafLikelihood(std::size_t remainingDepth) { return progress(remainingDepth); }

  // Chance upon construction of each node to be a single branch node
  double singleBranchLikelihood(std::size_t remainingDepth) { return 1.0 - progress(remainingDepth) * 0.75; }

  // Chance upon construction of each node to be a double branch node
  double doubleBranchLikelihood(std::size_t remainingDepth) { return 1.0 - progress(remainingDepth); }

  ////////////////// TYPES
public:
  typedef decltype(&ExpressionTree::makeLeafExpression) nodeMaker;
};

#endif