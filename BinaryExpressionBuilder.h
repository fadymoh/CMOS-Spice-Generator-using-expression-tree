
#ifndef BinaryExpressionBuilder_H
#define BinaryExpressionBuilder_H
#include <string>
#include <stack>
#include <exception>
#include "Nodes.h"

class BinaryExpressionBuilder {

private:
	// holds either (, +, -, /, or *
	std::stack<char> operatorStack;

	// operandStack is made up of BinaryOperationNodes and NumericElementNode
	std::stack<ExpressionElementNode *> operandStack;

	void processOperator(char op);
	void processRightParenthesis();

	void doBinary(char op);

	int precedence(char op);

public:

	class NotWellFormed : public std::exception {

	public:
		virtual const char* what() const throw()
		{
			return "The expression is not valid";
		}
	};

	BinaryOperationNode *parse(std::string& istr) throw(NotWellFormed);
};
#endif