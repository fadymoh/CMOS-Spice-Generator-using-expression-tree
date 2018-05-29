#include<iostream>
#include<set>
#include <algorithm>
#include<string>
#include "StackType.h"
#include <unordered_map>
using namespace std;

struct block{
	string name = "M", drain, gate, source, body, type;
	int L, W;
};

struct node;
int counter = 1, wire_counter = 1;
int priority(char);
node* makeNode(char, bool _inverted = false);
void attachOperator(StackType<node*>&, StackType<node*>&);
string traverse_expression_tree(node*, block, string);
void traverse_expreesion_tree_de_morgan(node*);
//Pops operator off of the operators stack, 
//Pops 2 operands of treenode stack and attaches them to operator
int get_length(node * mynode);
void size_traverse_expression_tree(node* mynode, double, double);
struct node{
	char info;
	node* left;
	node* right;
	bool inverted = false;
};
int main(){

	string infix; // the infix expression read from the line
	StackType<char> input; // stack for input string
	StackType<node*> operators; // stack for operator pointer addresses
	StackType<node*> treenodes;  // stack for output node pointer addresses
	char temp, again = ' ';
	char eh_daa = char(39);
	cout << "-Infix to Expression Tree Creator-" << endl;
	cout << "-An expression tree is created from a user inputted infix expression-" << endl;
	cout << "-Then the expression tree is used to create the CMOS logic circuit of the expression-" << endl;
	//((a&b)|(c&d))&x
	while (again != 'n'){
		cout << endl << "Please enter an Infix Boolean Expression" << endl;
		cin >> infix;
		char mode;
		cout << endl << "Enter the Mode you want to operate in: s for simple mode or t for transistor scaling mode\n";
		cin >> mode;
		//Pushes the contents of the string into the input stack
		string temp_infix = infix;
		set<char> inverted_inputs;
		double n, p, l;
		if (mode == 't' || mode == 'T'){

			cout << " Enter N" << endl;
			cin >> n;
			cout << " Enter P" << endl;
			cin >> p;
			cout << " Enter L" << endl;
			cin >> l;
		}
		for (int i = 2; i<infix.length(); i++){
			if (infix[i] == ' ' || infix[i] == infix[0])
				return 0;
			input.Push(infix[i]);
		}
		// While the input stack is not empty...
		while (!input.IsEmpty()){
			temp = input.Top();
			input.Pop();
			//If it is operand, make it into a node, add it to output string.
			if (isalpha(temp))
			{
				treenodes.Push(makeNode(temp));
				inverted_inputs.insert(temp);
			}
			if (temp == eh_daa)
			{
				temp = input.Top();
				input.Pop();
				treenodes.Push(makeNode(temp, true));
			}
			//If it is Closing parenthesis, make into node, push it on stack.
			if (temp == ')')
				operators.Push(makeNode(temp));
			//If it is an operator, then
			if ((temp == '&') || (temp == '|')) {
				bool pushed = false;
				while (!pushed){
					//If stack is empty, make node and push operator on stack.
					if (operators.IsEmpty()){
						operators.Push(makeNode(temp));
						pushed = true;
					}
					//If the top of stack is closing parenthesis, make node and push operator on stack.
					else if (operators.Top()->info == ')'){
						operators.Push(makeNode(temp));
						pushed = true;
					}
					//If it has same or higher priority than the top of stack, make node and push operator on stack.
					else if ((priority(temp)>priority(operators.Top()->info)) || (priority(temp) == priority(operators.Top()->info))){
						operators.Push(makeNode(temp));
						pushed = true;
					}
					//Else pop the operator from the stack, perform attachOperator and add it to treenodes. repeat step 5.
					else {
						attachOperator(treenodes, operators);
					}
				}
			}
			//If it is a opening parenthesis, pop operators from stack and perform attachOperator 
			//until a closing parenthesis is encountered. Pop and discard the closing parenthesis.
			if (temp == '('){
				while (operators.Top()->info != ')')
				{
					attachOperator(treenodes, operators);
				}
				operators.Pop(); // ')' is popped and discarded
			}
		}
		//If there is no more input, unstack the remaining operators and perform attachOperator
		while (!operators.IsEmpty()){
			attachOperator(treenodes, operators);
		}
		block temp;

		//temp.name = "M1";
		temp.drain = "Y";
		temp.source = temp.body = "VDD";
		
		temp.type = "PMOS";
		counter = 1;
		cout << "THESE ARE OUR INVERTERS " << endl;
		for (auto x : inverted_inputs)
		{
			cout << "M" + to_string(counter++) + " wire_inv_" << x << " " << x << " vdd vdd PMOS" << endl;
			cout << "M" + to_string(counter++) + " wire_inv_" << x << " " << x << " gnd gnd NMOS" << endl;
		}
		cout << endl;

		cout << "THIS IS OUR CIRCUIT " << endl;
		int tmp_counter;
		string output;
		
		tmp_counter = counter;
		output = traverse_expression_tree(treenodes.Top(), temp, "PUN");
		cout << output << endl;
		if (mode == 't' || mode == 'T')
			counter = tmp_counter, size_traverse_expression_tree(treenodes.Top(), p * get_length(treenodes.Top()), l);
		cout << endl;
		
		traverse_expreesion_tree_de_morgan(treenodes.Top());
		temp.drain = "GND";
		temp.source = temp.body = "Y";
		
		tmp_counter = counter;
		output = traverse_expression_tree(treenodes.Top(), temp, "PDN");
		cout << output << endl;
		if (mode == 't' || mode == 'T')
			counter = tmp_counter, size_traverse_expression_tree(treenodes.Top(), n * get_length(treenodes.Top()), l);
		cout << endl;
		cout << "Would  you like to convert another expression? (y/n)";
		cin >> again;
		counter = 1;
		wire_counter = 1;
		free(treenodes.Top());
	}
	cout << endl;
	system("pause");
	return 0;
}

//Determines the priority of an operator
int priority(char op){
	if ((op == '|'))
		return 1;
	if ((op == '&'))
		return 2;
}

//Places a char from the input stack into a new treenode
node* makeNode(char info, bool _inverted){
	node* childnode;
	childnode = new node;
	childnode->info = info;
	childnode->left = NULL;
	childnode->right = NULL;
	childnode->inverted = _inverted;
	return childnode;
}

//Pops an operator from a stack
//Builds a tree node with the top two nodes in the
//treenode stacks as its left and right children. 
string traverse_expression_tree(node* mynode, block myblock, string type)
{
	if (mynode == NULL) return "";
	string out;

	block myblock1, myblock2;

	if (mynode->info == '&')
	{
		myblock1.source = myblock1.body = "wire" + to_string(wire_counter);
		myblock1.drain = myblock.drain;

		myblock2.source = myblock2.body = myblock.source;
		myblock2.drain = "wire" + to_string(wire_counter++);
	
		out = traverse_expression_tree(mynode->left, myblock2, type);
		out += traverse_expression_tree(mynode->right, myblock1, type);
		return out;

	} // ((c&d)|(e&f))&a
	else if (mynode->info == '|')
	{
		myblock1.drain = myblock.drain;
		myblock1.source = myblock1.body = myblock.source ;

		myblock2.drain = myblock.drain;
		myblock2.source = myblock2.body =  myblock.source;

		out = traverse_expression_tree(mynode->left, myblock2, type);
		out += traverse_expression_tree(mynode->right, myblock1, type);
		return out;
	}
	else
	{
		if (type == "PUN")
			return "M" + to_string(counter++) + " " + myblock.drain + " " + (!mynode->inverted ? "wire_inv_" : "") + mynode->info + " " + myblock.source + " " + myblock.body + " " + "PMOS\n";
		else
			return "M" + to_string(counter++) + " " + myblock.source + " " + (!mynode->inverted ? "wire_inv_" : "") + mynode->info + " " + myblock.drain + " " + myblock.drain + " " + "NMOS\n";
	}
	return out;
}
void traverse_expreesion_tree_de_morgan(node* mynode)
{
	if (mynode == NULL) return;
	if (mynode->info == '&') mynode->info = '|';
	else if (mynode->info == '|') mynode->info = '&';
	traverse_expreesion_tree_de_morgan(mynode->left);
	traverse_expreesion_tree_de_morgan(mynode->right);
}
void attachOperator(StackType<node*>& treenodes, StackType<node*>& operators){
	node* operatornode = operators.Top();
	operators.Pop();
	operatornode->left = treenodes.Top();
	treenodes.Pop();
	operatornode->right = treenodes.Top();
	treenodes.Pop();
	treenodes.Push(operatornode);
}


int get_length(node * mynode)
{
	if (mynode == NULL) return 0;
	else if (mynode->info == '&') return get_length(mynode->left) + get_length(mynode->right);
	else if (mynode->info == '|') return max(get_length(mynode->left), get_length(mynode->right));
	else return 1;
}
void size_traverse_expression_tree(node* mynode, double factor, double den)
{
	if (mynode == NULL) return;

	if (mynode->info == '&')
	{
		size_traverse_expression_tree(mynode->left, factor, den);
		size_traverse_expression_tree(mynode->right, factor, den);
	} // ((c&d)|(e&f))&a
	else if (mynode->info == '|')
	{
		const int l_len = get_length(mynode->left), r_len = get_length(mynode->right);
		if (l_len == r_len)
		{
			size_traverse_expression_tree(mynode->left, factor, den);
			size_traverse_expression_tree(mynode->right, factor, den);
		}
		else if (l_len > r_len)
		{
			size_traverse_expression_tree(mynode->left, factor, den);
			size_traverse_expression_tree(mynode->right, factor * r_len / l_len, den);
		}
		else
		{
			size_traverse_expression_tree(mynode->left, factor * l_len / r_len, den);
			size_traverse_expression_tree(mynode->right, factor, den);
		}
	}
	else
	{
		cout << "M" + to_string(counter++) + " " << den * factor << "/" << den << endl;
	}
}
