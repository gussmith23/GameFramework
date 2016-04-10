#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cctype>

using namespace std;

enum token_types
{
	TOKEN_KEYWORD,
	TOKEN_DELIMITER,
	TOKEN_OPERATOR,
	TOKEN_STATE_HEADER,
	TOKEN_VARIABLE,
	TOKEN_STRING,
	TOKEN_PYTHON_CODE,
	TOKEN_NUMBER,
	TOKEN_EOF,
	TOKEN_INVALID
};

int token_type;
string token_value;

int tokenizer_cur;

void advance_char()
{
	tokenizer_cur = cin.get();
}

int cur_char()
{
	return tokenizer_cur;
}

void next_token()
{
	static string temp, temp_lower;
	temp.clear();
	temp_lower.clear();

	token_type = TOKEN_INVALID;
	token_value = "";

	// Ignore leading whitespace
	while(isspace(cur_char()))
		advance_char();

	// Check for end of input
	if(cur_char() == EOF)
	{
		token_type = TOKEN_EOF;
		return;
	}

	// Check if delimiter
	if(cur_char() == '.' || cur_char() == ',')
	{
		token_type = TOKEN_DELIMITER;
		token_value = cur_char();
		advance_char();
		return;
	}

	// Check if operator
	if(cur_char() == '+' || cur_char() == '-' ||
	   cur_char() == '(' || cur_char() == ')' ||
	   cur_char() == '/' || cur_char() == '%')
	{
		token_type = TOKEN_OPERATOR;
		token_value = cur_char();
		advance_char();
		return;
	}

	// Check if string literal or python expression
	if(cur_char() == '"' || cur_char() == '`')
	{
		char match = cur_char();
		advance_char();
		while(cur_char() != match)
		{
			if(cur_char() == EOF)
		    {
				// TODO: Error handling
				return;
			}
			else if(cur_char() == '\\')
			{
				// Handle escaped characters
				advance_char();
				temp.push_back(cur_char());
			}
			else
			{
				temp.push_back(cur_char());
			}
			advance_char();
		}
		// Discard delimiter
		advance_char();

		token_value = temp;
		if(match == '"')
			token_type = TOKEN_STRING;
		else
			token_type = TOKEN_PYTHON_CODE;

		return;
	}

	// Check if number literal
	if(isdigit(cur_char()))
	{
	    do
		{
			temp.push_back(cur_char());
			advance_char();
		} while(isdigit(cur_char()));

		token_type = TOKEN_NUMBER;
		token_value = temp;
		return;
	}

	// Read in string
	while(isalnum(cur_char()) || cur_char() == '_')
	{
		temp.push_back(cur_char());
		temp_lower.push_back(tolower(cur_char()));
		advance_char();
	}

	// Check if state header
	if(cur_char() == ':')
	{
		advance_char();
		token_type = TOKEN_STATE_HEADER;
		token_value = temp;
		return;
	}

	// Check if keyword
	if(temp_lower == "get" || temp_lower == "say" ||
	   temp_lower == "if" || temp_lower == "else" ||
	   temp_lower == "is" || temp_lower == "finish")
	{
		token_type = TOKEN_KEYWORD;
		token_value = temp_lower;
		return;
	}

	// Otherwise, is a variable name
	if(!temp.empty())
	{
		token_type = TOKEN_VARIABLE;
		token_value = temp;
		return;
	}
	else
	{
		token_type = TOKEN_INVALID;
		return;
	}
}

void tokenizer_init()
{
	tokenizer_cur = cin.get();
	next_token();
}


void parse_state()
{
	
}

void parse_rulebook()
{
	
}

int main()
{
	tokenizer_init();

	while(token_type != TOKEN_EOF)
	{
		switch(token_type)
		{
		case TOKEN_KEYWORD:
			cout << "Keyword: ";
			break;
		case TOKEN_DELIMITER:
			cout << "Delimiter: ";
			break;
		case TOKEN_OPERATOR:
			cout << "Operator: ";
			break;
		case TOKEN_STATE_HEADER:
			cout << "State header: ";
			break;
		case TOKEN_VARIABLE:
			cout << "Variable: ";
			break;
		case TOKEN_STRING:
			cout << "String: ";
			break;
		case TOKEN_PYTHON_CODE:
			cout << "Python code: ";
			break;
		case TOKEN_NUMBER:
			cout << "Number: ";
			break;
		case TOKEN_INVALID:
			cout << "Invalid token";
			break;
		default:
			break;
		}

		cout << token_value << endl;

		next_token();
	}
}
