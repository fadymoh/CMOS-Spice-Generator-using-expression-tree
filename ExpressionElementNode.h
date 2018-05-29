
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
	virtual double value();
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

	virtual double value();
};