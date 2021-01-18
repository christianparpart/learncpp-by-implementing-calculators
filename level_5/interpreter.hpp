#pragma once
#include "parser.hpp"
#include <memory>
#include <stack>
#include <variant>

class Interpreter {
      public:
        /// Interpreter takes the Parser object which parses the expression
        /// and generates the AST
        explicit Interpreter(const Parser& parser) : parser_{parser} {}
        explicit Interpreter(Parser&& parser) : parser_{std::move(parser)} {}
        /// public API which returns the final result of the expression
        ///
        /// \return Calculated result of the user input expression
        int interpret() {
                ASTVisitor(parser_.parse().get());
                const int result= interpreter_stack_.top();
                interpreter_stack_.pop();
                return result;
        }

      private:
        Parser parser_;
        std::stack<int> interpreter_stack_;
        /// Let's parse the AST using Post-Order Traversal method
        /// which in-turn uses the Interpreter Stack for the computation
        void ASTVisitor(ASTNode* ast_root) {
                if(ast_root != nullptr) {
                        ASTVisitor(ast_root->get_left());
                        ASTVisitor(ast_root->get_right());
                        visit_node(ast_root);
                }
        }
        /// Check if the current node is a Num or BinOp, If num, Push it to the
        /// stack, If not, pop two elements from the stack and perform the
        /// operation(reverse polish) On the Post-Order traversal
        ///
        /// \param ast_node Current node seen by the ASTVisitor
        void visit_node(ASTNode* ast_node) {
                if(!ast_node->is_numeric()) {
                        int value_one= interpreter_stack_.top();
                        interpreter_stack_.pop();
                        int value_two= interpreter_stack_.top();
                        interpreter_stack_.pop();
                        Type interpret_operator=
                            ast_node->get_token().get_token_type();
                        switch(interpret_operator) {
                        case Type::Plus:
                                interpreter_stack_.push(value_one + value_two);
                                break;
                        case Type::Minus:
                                interpreter_stack_.push(value_one - value_two);
                                break;
                        case Type::Mul:
                                interpreter_stack_.push(value_one * value_two);
                                break;
                        case Type::Div:
                                interpreter_stack_.push(value_one / value_two);
                                break;
                        default:
                                error();
                        }
                } else {
                        interpreter_stack_.push(
                            ast_node->get_token().get_integer_literal());
                }
        }
        void error() { throw std::logic_error("Interpreter error"); }
};
