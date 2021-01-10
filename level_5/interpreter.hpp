#pragma once
#include <iostream>
#include <stdexcept>
#include "lexer.hpp"

class Interpreter{
	private:
		Lexer lexer_;
		Token current_token_;
		void error_(){
			throw std::logic_error("Invalid syntex");
		}
	public:
		/// Interpreter constructor for the calculator
		///
		/// \param lexer Lexer for getting the tokens, one at a time
		Interpreter(const Lexer& lexer) : lexer_(lexer), current_token_(lexer_.getNextToken())
		{}
		/// Eat the current token
		///
		/// Compare the *current_token_* with the one that is passed, if they both match,
		/// The symtex is currect, If not, it is a syntex error and we throw an exception
		/// \param token_type Type to check against
		void eat(Type token_type){
			if(this->current_token_.getToken() == token_type)
			{ this->current_token_ = this->lexer_.getNextToken(); }
			else
			{ this->error_(); }
		}
		/// factor: INTEGER | LPARAN expr RPARAN
		///
		/// This is the helper method useful for parsing the *factor* which is either an INT or a *expr*
		/// in the grammer
		int factor(){
			Token token = this->current_token_;
			if(token.getToken() == Type::INTEGER){
				this->eat(Type::INTEGER);
				return std::get<int>(token.getValue());
			}else if(token.getToken() == Type::LPARAN){
				this->eat(Type::LPARAN);
				int result = this->expr();
				this->eat(Type::RPARAN);
				return result;
			}
		}
		/// term: factor( (MUL|DIV)factor )*
		///
		/// This is the helper method useful for parsing the *term* in the interpreter grammer
		int term(){
			int result = this->factor();
			while(this->current_token_.getToken() == Type::MUL || this->current_token_.getToken() == Type::DIV){
				Token operation = this->current_token_;
				if(operation.getToken() == Type::MUL){
					this->eat(Type::MUL);
					result *= this->factor();
				}else if(operation.getToken() == Type::DIV){
					this->eat(Type::DIV);
					result /= this->factor();
				}
			}
			return result;
		}
		/// expr: Arithematic experssion parser / interpreter
		///
		/// \note expr	:	term( (PLUS|MINUS)term )
		///	      term	:	factor( (MUL|DIV)factor )*
		///       factor:	INTEGER | LPARAN expr RPARAN
		int expr(){
			int result = this->term();
			while(this->current_token_.getToken() == Type::PLUS || this->current_token_.getToken() == Type::MINUS){
				if(this->current_token_.getToken() == Type::PLUS){
					this->eat(Type::PLUS);
					result += this->term();
				}else if(this->current_token_.getToken() == Type::MINUS){
					this->eat(Type::MINUS);
					result -= this->term();
				}
			}
			return result;
		}
};
