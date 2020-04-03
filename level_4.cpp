#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// Level 4. Ask for a string input and evaluate the result linearly without operator precedence

// XXX
// - Add importing whole namespaces;
// - Add handling multiple operands (more than two)
// - Add handling line based input parsing
// - Add enum class
// - Add ternary operator (?:)

using namespace std;

using number_type = int;

string get_line(string const& _prompt)
{
	cout << _prompt << ": ";
	cout.flush();

	string line;
	getline(cin, line);
	return line;
}

optional<int> parse_number(string const& _input, size_t& _parseOffset)
{
	int i = 0;
	int number = 0;
	while (_input[_parseOffset] >= '0' && _input[_parseOffset] <= '9')
	{
		number *= 10;
		number += _input[_parseOffset] - '0';
		++_parseOffset;
		++i;
	}
	return i ? optional{number} : nullopt;
}

enum class Operator {
	Plus,
	Minus,
	Multiply,
	Divide,
};

optional<Operator> parse_operator(string const& _input, size_t& _parseOffset)
{
	switch (_input[_parseOffset])
	{
		case '+':
			_parseOffset++;
			return Operator::Plus;
		case '-':
			_parseOffset++;
			return Operator::Minus;
		case '*':
			_parseOffset++;
			return Operator::Multiply;
		case '/':
			_parseOffset++;
			return Operator::Divide;
		default:
			return nullopt;
	}
}

optional<number_type> parse_and_evaluate(string const& _expression)
{
	size_t parseOffset = 0;
	optional<number_type> const first_value = parse_number(_expression, parseOffset);
	if (!first_value.has_value())
	{
		cerr << "Could not parse first number.\n";
		return nullopt;
	}

	number_type result = first_value.value();

	while (parseOffset < _expression.size())
	{
		optional<Operator> const op = parse_operator(_expression, parseOffset);
		if (!op.has_value())
		{
			cerr << "Failed to parse operator at offset: " << parseOffset << '\n';
			break;
		}

		optional<number_type> const rhs = parse_number(_expression, parseOffset);
		if (!rhs.has_value())
		{
			cerr << "Failed to parse next operand at offset " << parseOffset << '\n';
			return nullopt;
		}

		switch (op.value())
		{
			case Operator::Plus:
				result = result + rhs.value();
				break;
			case Operator::Minus:
				result = result - rhs.value();
				break;
			case Operator::Multiply:
				result = result * rhs.value();
				break;
			case Operator::Divide:
				if (rhs.value() != 0)
					result = result / rhs.value();
				else
				{
					cerr << "Division by zero is undefined.\n";
					return nullopt;
				}
				break;
		}
	}
	return result;
}

int main()
{
	string const line = get_line("Enter the numerical expression with operators [+ - * /]");
	optional<number_type> const result = parse_and_evaluate(line);
	if (result.has_value())
		cout << "Result: " << result.value() << '\n';
	else
		cerr << "Failed to parse and evaluate.\n";

	return EXIT_SUCCESS;
}

