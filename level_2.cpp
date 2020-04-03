#include <iostream>
#include <string>
#include <cstdlib>

// XXX Let's you chose the operation.
// - Adds custom functions
// - Adds using declaration
// - Adds ability to choose operation

using std::string;
using std::cin;
using std::cout;
using std::cerr;

int get_number(std::string const& _prompt)
{
	cout << _prompt << ": ";
	cout.flush();
	int value;
	cin >> value;
	return value;
}

char get_sym(string const& _prompt)
{
	cout << _prompt << ": ";
	cout.flush();
	char value;
	cin >> value;
	return value;
}

int evaluate(int a, int b, char op)
{
	switch (op)
	{
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			if (b != 0)
				return a / b;
			cerr << "Oh, you cannot device by 0\n";
			break;
		default:
			cerr << "Oh noez! I do not know this operator. " << op << '\n';
			break;
	}
	return 0; // failure case
}

int main()
{
	int const a = get_number("First number");
	int const b = get_number("Second number");
	char const op = get_sym("Operator [+ - * /]");
	int const c = evaluate(a, b, op);

	cout << "Result of " << a << " " << op << " " << b << " = " << c << '\n';

	return EXIT_SUCCESS;
}
