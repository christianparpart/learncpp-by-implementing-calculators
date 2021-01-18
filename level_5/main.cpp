#include "interpreter.hpp"
#include "parser.hpp"

auto main(int argc, const char* const argv[]) -> int {
        while(true) {
                try {
                        std::string expression;
                        std::cout << "calc> ";
                        std::getline(std::cin, expression);
                        Lexer lexer(expression);
                        Parser parser(lexer);
                        Interpreter interpreter(std::move(parser));
                        std::cout << std::endl;
                        std::cout << interpreter.interpret() << std::endl;
                } catch(const std::logic_error& except) {
                        std::cout << "Syntex error" << std::endl;
                        continue;
                }
        }
        return 0;
}
