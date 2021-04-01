#include "../include/ExpressionTree.hpp"
#include "../include/ExpressionFactory.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include <random>

// Used when constructing nodes
struct WeightedMaker {
  ExpressionTree::nodeMaker maker;
  double weight;
  WeightedMaker(ExpressionTree::nodeMaker maker, double weight) : maker(maker), weight(weight) {}
};

// local functions
static double evaluateExpression(std::unique_ptr<ExpressionNode>&, std::vector<double>&);
static double extractVariable(std::unique_ptr<ExpressionNode>&, std::vector<double>&);
static ExpressionTree::nodeMaker randomWeightedMaker(std::vector<WeightedMaker>, std::default_random_engine&);

void ExpressionTree::build() {
  if (depth > 0) head = grow(depth - 1);
  else {
    // Creates an expression that indicates to always return 0
    head = std::make_unique<ExpressionNode>(Expression('0', -1));
  }
}

std::unique_ptr<ExpressionNode> ExpressionTree::grow(std::size_t remainingDepth) {
  // If this is the bottom of the tree, always makes a leaf node
  if (remainingDepth == 0) return makeLeafExpression();

  // Gets random node maker, weighted by node type probabilities
  auto maker = randomWeightedMaker({
    WeightedMaker(makeLeafExpression, leafLikelihood(remainingDepth)),
    WeightedMaker(makeSingleExpression, singleBranchLikelihood(remainingDepth)),
    WeightedMaker(makeDoubleExpression, doubleBranchLikelihood(remainingDepth))
    },
    randomEngine
  );

  // Calls the randomly selected maker and returns its resulting node
  return maker();
}

std::unique_ptr<ExpressionNode> ExpressionTree::makeLeafExpression() {
  std::cout << "In make leaf" << std::endl;
  // Gets a random index for one of the available variables
  std::uniform_int_distribution<int> randomIndex(0, variables.size() - 1);
  int variableIndex = randomIndex(randomEngine);

  return std::make_unique<ExpressionNode>(Expression(variables[variableIndex], variableIndex));
}

std::unique_ptr<ExpressionNode> ExpressionTree::makeSingleExpression() {
  std::cout << "In make single" << std::endl;
  // Gets a random index for one of the available variables
  std::uniform_int_distribution<int> randomIndex(0, variables.size() - 1);
  int variableIndex = randomIndex(randomEngine);

  return std::make_unique<ExpressionNode>(Expression(variables[variableIndex], variableIndex));
}

std::unique_ptr<ExpressionNode> ExpressionTree::makeDoubleExpression() {
  std::cout << "In make double" << std::endl;
  // Gets a random index for one of the available variables
  std::uniform_int_distribution<int> randomIndex(0, variables.size() - 1);
  int variableIndex = randomIndex(randomEngine);

  return std::make_unique<ExpressionNode>(Expression(variables[variableIndex], variableIndex));
}

double ExpressionTree::plugVariables(std::vector<double> variables) {
  return evaluateExpression(head, variables);
}

bool ExpressionTree::likelihood(double chance) {
  // Gets a random number between 0 and 100
  std::uniform_real_distribution<double> continuousCoin(0, 1);

  return continuousCoin(randomEngine) < chance;
}

static double evaluateExpression(std::unique_ptr<ExpressionNode>& expressionNode, std::vector<double>& variables) {
  // If this expression has no children, we can evaluate it now
  if (expressionNode->children.size() == 0) return extractVariable(expressionNode, variables);
  printf("No variable to extract here!\n");
  return extractVariable(expressionNode, variables);
}

static double extractVariable(std::unique_ptr<ExpressionNode>& expressionNode, std::vector<double>& variables) {
  int variableIndex = expressionNode->expression.variableIndex;
  // If index is -1, should always return 0
  return variableIndex >= 0 ? variables[variableIndex] : 0.0;
}

static ExpressionTree::nodeMaker randomWeightedMaker(std::vector<WeightedMaker> makers, std::default_random_engine& engine) {
  // Gets total sum of weights
  double totalWeight = 0.0;
  for (auto maker : makers) totalWeight += maker.weight;

  // Sorts makers based on weight
  sort(makers.begin(), makers.end(),
    [](WeightedMaker maker1, WeightedMaker maker2) { return maker1.weight >= maker2.weight; }
  );

  // Gets random value in [0, totalWeight) range
  std::uniform_real_distribution<double> randomPoint(0, totalWeight);
  double pointInWeightRange = randomPoint(engine);

  // Finds out which maker got picked
  auto makerIterator = makers.begin();
  while ((pointInWeightRange -= makerIterator->weight) > 0) makerIterator++;

  // Returns selected maker
  return makerIterator->maker;
}