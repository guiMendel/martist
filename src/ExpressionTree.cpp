#include "../include/ExpressionTree.hpp"
#include "../include/ExpressionFactory.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>

// Used when constructing nodes
struct WeightedMaker {
  ExpressionTree::nodeMaker maker;
  double weight;
  WeightedMaker(ExpressionTree::nodeMaker maker, double weight) : maker(maker), weight(weight) {}
};

// local functions

// Randomly selects one of the nodeMaker attributes from the struct in the vector, weighted by the struct's weight
static ExpressionTree::nodeMaker randomWeightedMaker(std::vector<WeightedMaker>, std::default_random_engine&);
// Simply returns a random index that is valid for the provided vector
template <class T> static int randomIndex(std::vector<T>& container, std::default_random_engine& engine);

void ExpressionTree::init(std::vector<char> variables, unsigned int seed) {
  setVariables(variables);
  setSeed(seed);
  ExpressionFactory::populateExpressions(singleExpressions, doubleExpressions);
}

//////////////////////////////// TREE BUILDING

void ExpressionTree::build() {
  if (depth > 0) head = grow(depth - 1);
  else {
    // Creates a node that always evaluates to 0
    head = std::make_unique<NullNode>();
  }
}

std::unique_ptr<ExpressionNode> ExpressionTree::grow(std::size_t remainingDepth) {
  // DEBUG
  // std::cout << "Growing layer " << remainingDepth << std::endl;
  // If this is the bottom of the tree, always makes a leaf node
  if (remainingDepth == 0) return makeLeafExpression(remainingDepth);
  // Gets random node maker, weighted by node type probabilities for current depth
  auto maker = randomWeightedMaker({
    WeightedMaker(&ExpressionTree::makeLeafExpression, leafLikelihood(remainingDepth)),
    WeightedMaker(&ExpressionTree::makeSingleExpression, singleBranchLikelihood(remainingDepth)),
    WeightedMaker(&ExpressionTree::makeDoubleExpression, doubleBranchLikelihood(remainingDepth))
    },
    randomEngine
  );

  // Calls the randomly selected maker and returns its resulting node
  return (this->*maker)(remainingDepth);
}

/////////////////////////////// TREE EVALUATING

double ExpressionTree::plugVariables(std::vector<double> variables) {
  return head->evaluate(variables);
}

std::ostream& operator<<(std::ostream& out, const ExpressionTree& tree) {
  out << tree.head->toString();

  return out;
}

std::istream& operator>>(std::istream& in, const ExpressionTree& tree) {
  char expression;

  // Discard empty spaces
  while (in.get(expression) && isspace(expression));

  // If there's still content left
  if (in) {
    do {
      std::cout << expression << std::endl;
    } while (in.get(expression) && !isspace(expression));

    // If read a whitespace, puts it back in
    if (in) in.unget();
  }

  return in;
}

/////////////////////////////// PROBABILITY STUFF

bool ExpressionTree::likelihood(double chance) {
  // Gets a random number between 0 and 100
  std::uniform_real_distribution<double> continuousCoin(0, 1);

  return continuousCoin(randomEngine) < chance;
}

//////////////////////////////// NODE INITIALIZERS

LeafNode::LeafNode(std::vector<char>& variableRepresentations, std::default_random_engine& engine) {
  // Grabs a random index from the vector
  int variableIndex = randomIndex<char>(variableRepresentations, engine);
  expression = Expression(variableRepresentations[variableIndex], variableIndex);
}

SingleNode::SingleNode(std::vector<Expression>& availableExpressions, std::default_random_engine& engine,
  std::unique_ptr<ExpressionNode> child
) : child(child.release()) {
  expression = availableExpressions[randomIndex<Expression>(availableExpressions, engine)];
}

DoubleNode::DoubleNode(std::vector<Expression>& availableExpressions, std::default_random_engine& engine,
  std::unique_ptr<ExpressionNode> child1, std::unique_ptr<ExpressionNode> child2
) : child1(child1.release()), child2(child2.release()) {
  expression = availableExpressions[randomIndex<Expression>(availableExpressions, engine)];
}

/////////////////////////////// LOCAL FUNCTIONS

static ExpressionTree::nodeMaker randomWeightedMaker(std::vector<WeightedMaker> makers, std::default_random_engine& engine) {
  // Gets total sum of weights
  double totalWeight = 0.0;
  for (auto maker : makers) totalWeight += maker.weight;

  // Sorts makers based on weight
  sort(makers.begin(), makers.end(),
    [](WeightedMaker& maker1, WeightedMaker& maker2) { return maker1.weight >= maker2.weight; }
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

template <class T> static int randomIndex(std::vector<T>& container, std::default_random_engine& engine) {
  // Gets a random index
  std::uniform_int_distribution<int> index(0, container.size() - 1);
  return index(engine);
}