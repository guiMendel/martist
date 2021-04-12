#include "../include/SpecReader.hpp"
#include "../include/ExpressionTree.hpp"
#include <functional>
#include <iostream>

void SpecReader::makeStackRecipes(ExpressionTree& tree) {
  std::cout << "Making recipes..." << std::endl;
  std::cout << "For variables: ";
  // Get recipes for leaf nodes
  for (std::size_t index = 0; index < tree.variables.size(); index++) {
    char representation = tree.variables[index];
    std::cout << representation << " ";

    // A function that makes the leaf node corresponding to this variable
    stackRecipeEntries[representation] = [index, representation, this]() {
      std::cout << "Making variable " << representation << ". . . ";
      // Creates the leaf node and stacks it on the pile
      stack(std::make_unique<LeafNode>(index, representation));
      std::cout << "DONE" << std::endl;
    };
  }

  std::cout << std::endl << "For single expressions: ";
  // Get recipes for single nodes
  for (std::size_t index = 0; index < tree.singleExpressions.size(); index++) {
    auto expression = tree.singleExpressions[index];
    std::cout << expression.characterRepresentation << " ";

    // A function that makes the single node corresponding to this character
    stackRecipeEntries[expression.characterRepresentation] = [expression, this]() {
      std::cout << "Making single expression " << expression.characterRepresentation << ". . . ";
      // Pops head of the stack and grabs it's node
      // auto child = pop();

      // Create a single node with recovered node as child and stack it on the pile
      stack(std::make_unique<SingleNode>(expression, pop()));
      std::cout << "DONE" << std::endl;
    };
  }

  std::cout << std::endl << "For double expressions: ";
  // Get recipes for double nodes
  for (std::size_t index = 0; index < tree.doubleExpressions.size(); index++) {
    auto expression = tree.doubleExpressions[index];
    std::cout << expression.characterRepresentation << " ";

    // A function that makes the double node corresponding to this character
    stackRecipeEntries[expression.characterRepresentation] = [expression, this]() {
      std::cout << "Making double expression " << expression.characterRepresentation << ". . . ";
      // Take the two first nodes
      // std::unique_ptr<ExpressionNode> child1(stackHead->node.release());
      // std::unique_ptr<ExpressionNode> child2(stackHead->next->node.release());

      // Pop these nodes from the stack
      // stackHead.reset(stackHead->next->next.release());

      // Create a double node with 2 recovered nodes as children and stack it on the pile
      stack(std::make_unique<DoubleNode>(expression, pop(), pop()));
      std::cout << "DONE" << std::endl;
    };
  }
  std::cout << std::endl;
}