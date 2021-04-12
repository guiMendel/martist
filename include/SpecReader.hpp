#ifndef __SPEC_READER__
#define __SPEC_READER__

#include <map>
#include <stdexcept>
#include "./ExpressionTree.hpp"

struct NodeStackElement {
  NodeStackElement(std::unique_ptr<NodeStackElement>& head, std::unique_ptr<ExpressionNode>& node)
    : next(head.release()), node(node.release()) {
  }

  // Pointer to next element in stack
  std::unique_ptr<NodeStackElement> next;

  // Pointer to the created node
  std::unique_ptr<ExpressionNode> node;

  void print(int layer = 0) {
    auto nodeRepresentation = node->toString();
    std::cout << "Layer " << layer << ": " << nodeRepresentation << std::endl;
    if (next)
      next->print(layer + 1);
  }
};

class SpecReader {
public:
  typedef std::function<void()> NodeMaker;

  SpecReader(ExpressionTree& tree) { makeStackRecipes(tree); stackHead = nullptr; }

  // Reads an expression and pushes it to the pile
  void read(char expression) {
    try { stackRecipeEntries.at(expression)(); }
    catch (std::out_of_range& error) {
      std::cerr << "ERROR: No such expression '" << expression << "'" << std::endl;
      throw std::invalid_argument("No such expression " + expression);
    }
  };

  // Returns assembled tree and empties the stack
  ExpressionNode* assembleTree() { auto tree = stackHead->node.release(); stackHead.reset(); return tree; }

private:
  // Passes through the tree's variables, single and double expressions and populates stackRecipeEntries
  // with recipes to make each one of them, indexed by their char representations
  void makeStackRecipes(ExpressionTree& tree);

  // Creates a stack element and puts it on the stack
  void stack(std::unique_ptr<ExpressionNode>&& node) { stackHead = std::make_unique<NodeStackElement>(stackHead, node); }

  // Pops the head node
  std::unique_ptr<ExpressionNode> pop() {
    // Makes sure stack not empty
    if (!stackHead) {
      std::cerr << "ERROR: Syntax error in the expressions" << std::endl;
      throw std::invalid_argument("Bad expression syntax");
    }

    // Take the head node
    std::unique_ptr<ExpressionNode> child(stackHead->node.release());

    // Pop this node from the stack
    auto next = stackHead->next.release();

    stackHead.reset(next);

    return child;
  }

  // Head of the stack of read expressions
  std::unique_ptr<NodeStackElement> stackHead;

  // Holds functions that can create nodes from their char representations and automatically stack them
  std::map<char, NodeMaker> stackRecipeEntries;
};

#endif