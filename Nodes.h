// Inspired from code at: http://math.hws.edu/eck/cs225/s03/binary_trees/
// Represents a node of any type in an expression tree.
// This is an "abstract" class, since it contains an undefined
// function, value(), that must be defined in subclasses.
#ifndef NODES_h
#define NODES_h
class ExpressionElementNode {
public:
	virtual double value() = 0;  // Return the value of this node.
};

class NumericElementNode : public ExpressionElementNode {

private:
	double number;
	NumericElementNode(const NumericElementNode& n);
	NumericElementNode();
	NumericElementNode&operator=(const NumericElementNode& n);
public:

	NumericElementNode(double val);
	//virtual double value();
};

inline NumericElementNode::NumericElementNode(double val) : number(val) {}

inline double NumericElementNode::value()
{
	return number;
}

class BinaryOperationNode : public ExpressionElementNode {

private:

	char binary_op;

	ExpressionElementNode *left;
	ExpressionElementNode *right;

	BinaryOperationNode(const BinaryOperationNode& n);
	BinaryOperationNode();
	BinaryOperationNode &operator=(const BinaryOperationNode& n);

public:
	BinaryOperationNode(char op, ExpressionElementNode *l, ExpressionElementNode *r);

	double BinaryOperationNode::value()
	{
		// To get the value, compute the value of the left and
		// right operands, and combine them with the operator.
		double leftVal = left->value();

		double rightVal = right->value();

		double result;

		switch (binary_op) {

		case '+':  result = leftVal + rightVal;
			break;

		case '-':  result = leftVal - rightVal;
			break;

		case '*':  result = leftVal * rightVal;
			break;

		case '/':  result = leftVal / rightVal;
			break;
		}

		return result;
	}
};

inline BinaryOperationNode::BinaryOperationNode(char op, ExpressionElementNode *l, ExpressionElementNode *r) :
binary_op(op), left(l), right(r) {}
#endif