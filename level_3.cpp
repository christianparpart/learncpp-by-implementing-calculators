#include <iostream>
#include <string>
#include <cstdlib>
#include <optional>
#include <cassert>

// XXX
// - Adds error handling
// - Adds custom number type.
// - Adds optional<T>
// - Adds assert()

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::optional;
using std::nullopt;

using number_type = int;

optional<number_type> get_number(std::string const& _prompt)
{
	cout << _prompt << ": ";
	cout.flush();

	if (cin.fail())
	{
		cin.clear(); // clear fail-bit
		cin.ignore();
	}

	number_type value;
	cin >> value;
	if (!cin.fail())
		return {value};
	else
		return nullopt;
}

optional<char> get_operator()
{
	cout << "Operator [+-*/]: ";
	cout.flush();

	char value; // TODO: assert validity

	if (cin.fail())
	{
		cin.clear();
		cin.ignore();
	}

	if (cin >> value)
		return value;
	else
		return nullopt;
}

optional<number_type> evaluate(number_type a, number_type b, char op)
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
			return nullopt;
		default:
			assert(!"Oh noez! I do not know this operator.");
			return 0;
	}
}

int main()
{
	optional<number_type> const a = get_number("First number");
	if (!a.has_value())
	{
		cerr << "Failed to get operand.\n";
		return EXIT_FAILURE;
	}

	optional<number_type> const b = get_number("Second number");
	if (!b.has_value())
	{
		cerr << "Failed to get operand.\n";
		return EXIT_FAILURE;
	}

	optional<char> const op = get_operator();
	if (!op.has_value())
	{
		cerr << "Failed to get operator.\n";
		return EXIT_FAILURE;
	}

	optional<number_type> const c = evaluate(a.value(), b.value(), op.value());
	if (c.has_value())
		cout << "Result of " << a.value() << " " << op.value() << " " << b.value() << " = " << c.value() << '\n';
	else
		cout << "Could not evaluate result. ;-(\n";

	return EXIT_SUCCESS;
}
