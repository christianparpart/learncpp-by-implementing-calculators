#pragma once
#include <iostream>
#include <stdexcept>
#include <variant>
#include <cctype>

enum class Type {
	INTEGER, PLUS, MINUS, MUL, DIV, END, LPARAN, RPARAN
};

class Token{
	private:
	Type token_type_;
	std::variant<int, char> value_;
	public:
		Token(Type token_type, int value) : token_type_{token_type}, value_{value} {}
		Token(Type token_type, char value) : token_type_{token_type}, value_{value} {}
		Type getToken() const noexcept
		{ return this->token_type_; }
		std::variant<int, char> getValue() const noexcept
		{ return this->value_; }
};

class Lexer{
	private:
		std::string text_;
		std::size_t pos_{};
		char current_char_;
		void error_()
		{ throw std::logic_error("Lex Error"); }
		/// Advances the *pos_* pointer and sets *current_char_* variable
		///
		///
		void advance_(){
			this->pos_++;
			if(this->pos_ > text_.size()-1)
			{ this->current_char_ = '\0'; }
			else
			{ this->current_char_ = this->text_.at(pos_); }
		}
		void skip_whitespace_(){
			while(this->current_char_ != '\0' && std::isspace(this->current_char_))
			{ this->advance_(); }
		}
		int get_integet(){
			int result{0};
			while(this->current_char_ != '\0' && std::isdigit(this->current_char_)){
				result *= 10;
				result += this->current_char_-'0';
				this->advance_();
			}
			return result;
		}
	public:
		/// Lexer constructor
		///
		/// \param text Raw calculator expression
		Lexer(const std::string& text) : text_{text}
		{ this->current_char_ = this->text_.at(pos_); }

		/// Lexucal analyzer(or scanner or token generator)
		///
		/// This method is responsible for breaking the input expression into tokens. One token at a time.
		/// \return Current Token of the input string
		Token getNextToken(){
			while(this->current_char_ != '\0'){
				if(std::isspace(this->current_char_))
				{ this->skip_whitespace_(); continue; }
				else if(std::isdigit(this->current_char_))
				{ return Token(Type::INTEGER, this->get_integet()); }
				else if(this->current_char_ == '+')
				{ this->advance_(); return Token(Type::PLUS, '+'); }
				else if(this->current_char_ == '-')
				{ this->advance_(); return Token(Type::MINUS, '-'); }
				else if(this->current_char_ == '*')
				{ this->advance_(); return Token(Type::MUL, '*'); }
				else if(this->current_char_ == '/')
				{ this->advance_(); return Token(Type::DIV, '/'); }
				else if(this->current_char_ == '(')
				{ this->advance_(); return Token(Type::LPARAN, '('); }
				else if(this->current_char_ == ')')
				{ this->advance_(); return Token(Type::RPARAN, ')'); }
				this->error_();
			}
			return Token(Type::END, '\0');
		}
};
