#include "interpreter.hpp"

auto main(int argc, const char*const argv[]) -> int{
	while(true){
		try{
			std::string expression;
			std::cout << "calc> ";
			std::getline(std::cin, expression);
			Interpreter interpreter{Lexer(expression)};
			std::cout << std::endl;
			std::cout << interpreter.expr() << std::endl;
		}catch(const std::logic_error& except){
			std::cout << "Syntex error" << std::endl;
			continue;
		}
	}
	return 0;
}
