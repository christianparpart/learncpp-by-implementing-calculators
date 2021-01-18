#pragma once
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <variant>

enum class Type { Integer, Plus, Minus, Mul, Div, End, Lparan, Rparan, Space };

class Token {
      private:
        Type token_type_{};
        int value_{};

      public:
        /// Token constructor for parsed tokens on the interpreter,
        /// which have integer type
        ///
        /// \param token_type Type of token, *Type*, this class holds for
        /// parsing
        /// \param value Actual value of the token
        constexpr Token(Type token_type, int value)
            : token_type_{token_type}, value_{value} {}
        /// Token constructor for parsing tokens which have *Type* operations
        /// which does not have a *int* type (and also friends)
        ///
        /// \param token_type Type of the operator
        constexpr Token(Type token_type) : token_type_{token_type} {}
        constexpr Token() {}
        constexpr Token(const Token& token) {
                token_type_= token.token_type_;
                value_= token.value_;
        }
        constexpr Token& operator=(const Token& token) {
                token_type_= token.token_type_;
                value_= token.value_;
                return *this;
        }
        constexpr Type get_token_type() const noexcept { return token_type_; }
        constexpr int get_integer_literal() const noexcept { return value_; }
};

class Lexer {
      public:
        /// Lexer constructor
        ///
        /// \param text Raw calculator expression
        explicit Lexer(const std::string& text)
            : text_{text}, current_char_{text_.at(pos_)} {}
        explicit Lexer(std::string&& text)
            : text_{std::move(text)}, current_char_{text_.at(pos_)} {}

        /// Lexucal analyzer(or scanner or token generator)
        ///
        /// This method is responsible for breaking the input expression into
        /// tokens. One token at a time. \return Current Token of the input
        /// string
        constexpr Token get_next_token() {
                while(current_char_ != '\0') {
                        Type char_type= get_type_from_char(current_char_);
                        switch(char_type) {
                        case Type::Space:
                                skip_whitespace();
                                continue;
                        case Type::Integer:
                                return Token(Type::Integer, integet());
                        case Type::Div:
                                advance();
                                return Token(Type::Div);
                        case Type::Mul:
                                advance();
                                return Token(Type::Mul);
                        case Type::Lparan:
                                advance();
                                return Token(Type::Lparan);
                        case Type::Rparan:
                                advance();
                                return Token(Type::Rparan);
                        case Type::Plus:
                                advance();
                                return Token(Type::Plus);
                        case Type::Minus:
                                advance();
                                return Token(Type::Minus);
                        default:
                                error();
                        }
                }
                return Token(Type::End, '\0');
        }

      private:
        std::string text_;
        std::size_t pos_{};
        char current_char_;
        void error() { throw std::logic_error("Lex Error"); }
        /// Advances the *pos_* pointer and sets *current_char_* variable
        ///
        /// \note Increments the position pointer and updates the current char
        ///		  seen by the lexer
        void advance() noexcept {
                pos_++;
                if(pos_ > text_.size() - 1) {
                        current_char_= '\0';
                } else {
                        current_char_= text_.at(pos_);
                }
        }
        constexpr void skip_whitespace() noexcept {
                while(current_char_ != '\0' && std::isspace(current_char_)) {
                        advance();
                }
        }
        constexpr int integet() noexcept {
                int result{0};
                while(current_char_ != '\0' && std::isdigit(current_char_)) {
                        result*= 10;
                        result+= current_char_ - '0';
                        advance();
                }
                return result;
        }
        /// Get a token of type *Type* from a char for switch-case during
        /// parsing
        ///
        /// \param value Current char
        constexpr Type get_type_from_char(char value) {
                switch(value) {
                case ' ':
                        return Type::Space;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                        return Type::Integer;
                case '+':
                        return Type::Plus;
                case '-':
                        return Type::Minus;
                case '*':
                        return Type::Mul;
                case '/':
                        return Type::Div;
                case ')':
                        return Type::Rparan;
                case '(':
                        return Type::Lparan;
                default:
                        error();
                }
        }
};
