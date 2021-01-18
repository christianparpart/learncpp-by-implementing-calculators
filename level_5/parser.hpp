#pragma once
#include "lexer.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <variant>

enum class NodeType { Num= 0, BinOp, UnaryOp };
class BinOp;
class Num;

/// Base AST class from which all other operations inherit from
class ASTNode {
      public:
        constexpr virtual const Token& get_token() const noexcept= 0;
        constexpr virtual const NodeType get_node_type() const noexcept= 0;
        constexpr virtual ASTNode* get_left() noexcept= 0;
        constexpr virtual ASTNode* get_right() noexcept= 0;
        virtual ~ASTNode()= default;
};

/// Numeric type node on the AST, which will be used by
/// the BinOp during construction
class Num final : public ASTNode {
      public:
        Num(const Token& token) : token_{token} {}
        const Token& get_token() const noexcept override { return token_; }
        const NodeType get_node_type() const noexcept override {
                return NodeType::Num;
        }
        ASTNode* get_left() noexcept override { return left_node_.get(); }
        ASTNode* get_right() noexcept override { return right_node_.get(); }
        static std::unique_ptr<ASTNode> make(const Token& token) {
                return std::make_unique<Num>(token);
        }

      private:
        Token token_;
        std::unique_ptr<ASTNode> left_node_{nullptr};
        std::unique_ptr<ASTNode> right_node_{nullptr};
};

/// <Operand> Operator <Operand> Binary operator class
/// with two (left and right) operands
class BinOp final : public ASTNode {
      public:
        BinOp(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right,
              const Token& token)
            : left_node_{std::move(left)},
              right_node_{std::move(right)}, token_{token} {}
        const Token& get_token() const noexcept override { return token_; }
        const NodeType get_node_type() const noexcept override {
                return NodeType::BinOp;
        }
        ASTNode* get_left() noexcept override { return left_node_.get(); }
        ASTNode* get_right() noexcept override { return right_node_.get(); }
        static std::unique_ptr<ASTNode> make(std::unique_ptr<ASTNode> left,
                                             std::unique_ptr<ASTNode> right,
                                             const Token& token) {
                return std::make_unique<BinOp>(std::move(left),
                                               std::move(right), token);
        }

      private:
        Token token_;
        std::unique_ptr<ASTNode> left_node_{nullptr};
        std::unique_ptr<ASTNode> right_node_{nullptr};
};

/// <Operator> Operand Unary operator class
/// with one operand
class UnaryOp final : public ASTNode {
      public:
        UnaryOp(std::unique_ptr<ASTNode> left, const Token& token)
            : token_{token}, left_node_{std::move(left)} {}
        const Token& get_token() const noexcept override { return token_; }
        const NodeType get_node_type() const noexcept override {
                return NodeType::UnaryOp;
        }
        ASTNode* get_left() noexcept override { return left_node_.get(); }
        ASTNode* get_right() noexcept override { return right_node_.get(); }
        static std::unique_ptr<ASTNode> make(std::unique_ptr<ASTNode> left,
                                             std::unique_ptr<ASTNode>,
                                             const Token& token) {
                return std::make_unique<UnaryOp>(std::move(left), token);
        }

      private:
        Token token_;
        std::unique_ptr<ASTNode> left_node_{nullptr};
        std::unique_ptr<ASTNode> right_node_{nullptr};
};

class Parser {
      public:
        /// Interpreter constructor for the calculator
        ///
        /// \param lexer Lexer for getting the tokens, one at a time
        explicit Parser(const Lexer& lexer)
            : lexer_(lexer), current_token_(lexer_.get_next_token()) {}
        /// Let's Lex the input expression, parse it and build an AST
        ///
        /// \return Handler to the root node of the AST
        std::unique_ptr<ASTNode> parse() { return std::move(expr()); }

      private:
        Lexer lexer_;
        Token current_token_;
        void error() { throw std::logic_error("Invalid syntax"); }
        /// Eat the current token
        ///
        /// Compare the *current_token_* with the one that is passed, if they
        /// both match, The syntax is currect, If not, it is a syntax error and
        /// we throw an exception \param token_type Type to check against
        constexpr void eat(Type token_type) {
                if(current_token_.get_token_type() == token_type) {
                        current_token_= lexer_.get_next_token();
                } else {
                        error();
                }
        }
        /// factor: ( PLUS | MINUS ) factor | INTEGER | LPARAN expr RPARAN
        ///
        /// This is the helper method useful for parsing the *factor* which is
        /// either an INT or a *expr* in the grammer
        std::unique_ptr<ASTNode> factor() {
                Token token= current_token_;
                if(token.get_token_type() == Type::Integer) {
                        eat(Type::Integer);
                        return Num::make(token);
                } else if(token.get_token_type() == Type::Lparan) {
                        eat(Type::Lparan);
                        std::unique_ptr<ASTNode> result= expr();
                        eat(Type::Rparan);
                        return result;
                } else if(token.get_token_type() == Type::Minus) {
                        eat(Type::Minus);
                        return UnaryOp::make(factor(), nullptr,
                                             Token(Type::Minus));
                } else if(token.get_token_type() == Type::Plus) {
                        eat(Type::Plus);
                        return UnaryOp::make(factor(), nullptr,
                                             Token(Type::Plus));
                } else {
                        error();
                }
        }
        /// term: factor( ( MUL | DIV ) factor )*
        ///
        /// This is the helper method useful for parsing the *term* in the
        /// interpreter grammer, this builds the BinOp node for the AST.
        /// Since * and / have higher precedence than + and - we are calling
        /// *term()* before building + and - AST node for BinOp
        std::unique_ptr<ASTNode> term() {
                std::unique_ptr<ASTNode> result= factor();
                while(current_token_.get_token_type() == Type::Mul ||
                      current_token_.get_token_type() == Type::Div) {
                        Token operation= current_token_;
                        if(operation.get_token_type() == Type::Mul) {
                                eat(Type::Mul);
                        } else if(operation.get_token_type() == Type::Div) {
                                eat(Type::Div);
                        }
                        result=
                            BinOp::make(std::move(result), factor(), operation);
                }
                return result;
        }
        /// expr: Arithematic experssion parser / interpreter
        ///
        /// \note expr	 : term ( ( PLUS | MINUS ) term )*
        ///	      term	 : factor ( ( MUL | DIV ) factor )*
        ///       factor : ( PLUS | MINUS ) factor | INTEGER | LPARAN expr RPARAN
        std::unique_ptr<ASTNode> expr() {
                std::unique_ptr<ASTNode> result= term();
                while(current_token_.get_token_type() == Type::Plus ||
                      current_token_.get_token_type() == Type::Minus) {
                        Token operation= current_token_;
                        if(current_token_.get_token_type() == Type::Plus) {
                                eat(Type::Plus);
                        } else if(current_token_.get_token_type() ==
                                  Type::Minus) {
                                eat(Type::Minus);
                        }
                        result=
                            BinOp::make(std::move(result), term(), operation);
                }
                return result;
        }
};
