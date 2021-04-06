#ifndef __EXPRESSION_FUNCTION__
#define __EXPRESSION_FUNCTION__

#include <vector>

// Types of functions used in the expressions
// Functions that take only one parameter
typedef double (*SingleExpressionFunction)(double);
// Functions that take two parameters
typedef double (*DoubleExpressionFunction)(double, double);


struct Expression {
  // The character that represents this expression
  char characterRepresentation;
  // The space where we store this expression's function
  union {
    // For leaf expressions
    int variableIndex;
    // For expressions that have one child
    SingleExpressionFunction singleFunction;
    // For expressions that have two children
    DoubleExpressionFunction doubleFunction;
  };

  Expression() = default;

  Expression(char representation, SingleExpressionFunction operation)
    : characterRepresentation(representation)
    , singleFunction(operation) {
  }

  Expression(char representation, DoubleExpressionFunction operation)
    : characterRepresentation(representation)
    , doubleFunction(operation) {
  }

  Expression(char representation, int variableIndex)
    : characterRepresentation(representation)
    , variableIndex(variableIndex) {
  }
};

// Defines all functions used in the expressions and provides an interface to access them
class ExpressionFactory {
public:
  static void populateExpressions(std::vector<Expression>& singleExpressions, std::vector<Expression>& doubleExpressions) {
    singleExpressions = { Expression('s', &sin), Expression('c',&cosin) };
    doubleExpressions = { Expression('*', &product), Expression('a', &mean) };
  }

  // Uninstantiatable
  ExpressionFactory() = delete;

private:
  /////////////////////// EXPRESSION FUNCTIONS
  static double sin(double);
  static double cosin(double);
  static double product(double, double);
  static double mean(double, double);
};

#endif