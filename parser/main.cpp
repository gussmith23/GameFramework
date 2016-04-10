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
	TOKEN_VARIABLE,
	TOKEN_STRING,
	TOKEN_PYTHON_CODE,
	TOKEN_NUMBER,
	TOKEN_EOF,
	TOKEN_INVALID
};

unsigned int token_line;
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
    {
		if(cur_char() == '\n')
			++token_line;
		advance_char();
	}

	// Check for end of input
	if(cur_char() == EOF)
	{
		token_type = TOKEN_EOF;
		return;
	}

	// Check if delimiter
	if(cur_char() == '.' || cur_char() == ',' || cur_char() == ':')
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

	// Check if keyword
	if(temp_lower == "get" || temp_lower == "say" ||
	   temp_lower == "if" || temp_lower == "else" ||
	   temp_lower == "is" || temp_lower == "finish" ||
	   temp_lower == "state")
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

string str_token()
{
	string token_type_str;
	switch(token_type)
	{
	case TOKEN_KEYWORD:
		token_type_str = "Keyword";
		break;
	case TOKEN_DELIMITER:
		token_type_str = "Delimiter";
		break;
	case TOKEN_OPERATOR:
		token_type_str = "Operator";
		break;
	case TOKEN_VARIABLE:
		token_type_str = "Variable";
		break;
	case TOKEN_STRING:
		token_type_str = "String";
		break;
	case TOKEN_PYTHON_CODE:
		token_type_str = "Python code";
		break;
	case TOKEN_NUMBER:
		token_type_str = "Number";
		break;
	case TOKEN_INVALID:
		token_type_str = "Invalid token";
		break;
	default:
		break;
	}

	return "(" + token_type_str + ", \"" + token_value + "\")";
}

void tokenizer_init()
{
	token_line = 1;
	tokenizer_cur = cin.get();
	next_token();
}

bool parse_expression();

bool parse_expression_term()
{
	if(token_type == TOKEN_OPERATOR && token_value == "(")
	{
		if(!parse_expression())
			return false;
		if(token_type != TOKEN_OPERATOR || token_value != ")")
		{
			cerr << "Parse error in parse_expression_term(): expected operator ')'" << endl;
			return false;
		}
		next_token();
	}
	else if(token_type == TOKEN_NUMBER)
	{
		next_token();
		return true;
	}
	else if(token_type == TOKEN_VARIABLE)
	{
		next_token();
		return true;
	}
	else
	{
		cerr << "Parse error in parse_expression_term(): expected number or variable" << endl;
		return false;
	}

	return true;
}

bool parse_expression_factor()
{
	while(true)
	{
		if(!parse_expression_term())
			return false;

		if(token_type == TOKEN_OPERATOR)
		{
			if(token_value != "+" && token_value != "-" && token_value != ")")
			{
				cerr << "Parse error in parse_expression_factor(): expected operator '+', '-', or ')'" << endl;
				return false;
			}

			next_token();
		}
		else
		{
			return true;
		}
	}
}

bool parse_expression_noncompare()
{
	while(true)
	{
		if(!parse_expression_factor())
			return false;

		if(token_type == TOKEN_OPERATOR)
		{
			if(token_value != "*" && token_value != "/" && token_value != "%" && token_value != ")")
			{
				cerr << "Parse error in parse_expression_noncompare(): expected operator '*', '/', '%', ')'" << endl;
				return false;
			}

			next_token();
		}
		else
		{
		    return true;
		}
	}
}

bool parse_expression()
{
	if(!parse_expression_noncompare())
		return false;

	if(token_type == TOKEN_KEYWORD && token_value == "is")
	{
		next_token();
		if(!parse_expression_noncompare())
			return false;
	}

	return true;
}

bool parse_statement();

bool parse_conditional()
{
	if(token_type != TOKEN_KEYWORD || token_value != "if")
	{
		cerr << "Parse error in parse_conditional(): expected 'if'" << endl;
		return false;
	}
	
	next_token();
	if(!parse_expression())
		return false;

	if(token_type != TOKEN_DELIMITER || token_value != ":")
	{
		cerr << "Parse error in parse_conditional(): expected ':'" << endl;
		return false;
	}

	next_token();
	if(!parse_statement())
		return false;

	if(token_type == TOKEN_KEYWORD && token_value == "else")
	{
		next_token();
		if(token_type == TOKEN_DELIMITER && token_value == ":")
		{
			next_token();
			if(!parse_statement())
				return false;
		}
		else if(token_type == TOKEN_KEYWORD && token_value == "if")
		{
			if(!parse_conditional())
				return false;
		}
		else
		{
			cerr << "Parse error in parse_conditional(): expected ':' or 'if'" << endl;
			return false;
		}
	}

	return true;
}

bool parse_assignment()
{
	if(token_type != TOKEN_VARIABLE)
	{
		cerr << "Parse error in parse_assignment(): expected variable name." << endl;
		return false;
	}

	next_token();
	if(token_type != TOKEN_KEYWORD && token_value != "is")
	{
		cerr << "Parse error in parse_assignment(): expected keyword \"is\"." << endl;
		return false;
	}

	next_token();
	if(!parse_expression())
		return false;

	return true;
}

bool parse_statement()
{
	while(true)
	{
		if(token_type == TOKEN_VARIABLE)
		{
			if(!parse_assignment())
				return false;
		}
		else if(token_type == TOKEN_KEYWORD && token_value == "if")
		{
			if(!parse_conditional())
				return false;
			return true;
		}
		else if(token_type == TOKEN_KEYWORD && token_value == "say")
		{
			next_token();
			if(token_type != TOKEN_STRING)
			{
				cerr << "Parse error in parse_statement(): expected string" << endl;
				return false;
			}
			next_token();
		}
		else if(token_type == TOKEN_KEYWORD && token_value == "get")
		{
			next_token();
			if(token_type != TOKEN_VARIABLE)
			{
				cerr << "Parse error in parse_statement(): expected variable" << endl;
				return false;
			}
			next_token();
		}
		else if(token_type == TOKEN_KEYWORD && token_value == "finish")
		{
			next_token();
		}
		else
		{
			cerr << "Parse error in parse_statement(): expected variable or 'if'" << endl;
			return false;
		}

		if(token_type != TOKEN_DELIMITER)
		{
			cerr << "Parse error in parse_statement(): expected '.' or ',' at end of statement." << endl;
			return false;
		}

		char delimiter = token_value[0];
		next_token();
		if(delimiter == '.')
		{
			return true;
		}
	}
}

bool parse_state()
{
	if(token_type != TOKEN_KEYWORD || token_value != "state")
	{
		cerr << "Parse error in parse_state(): expected 'state'" << endl;
		return false;
	}

	next_token();
	if(token_type != TOKEN_VARIABLE)
	{
		cerr << "Parse error in parse_state(): expected variable" << endl;
		return false;
	}

	next_token();
	if(token_type != TOKEN_DELIMITER || token_value != ":")
	{
		cerr << "Parse error in parse_state(): expected ':'" << endl;
		return 1;
	}
		
	next_token();
	while(token_type != TOKEN_KEYWORD || token_value != "state")
	{
		if(token_type == TOKEN_EOF)
			return true;
		
		if(!parse_statement())
			return false;
	}

	return true;
}

bool parse_rulebook()
{
	while(token_type != TOKEN_EOF)
	{
		if(!parse_state())
			return false;
	}

	return true;
}

int main()
{
	tokenizer_init();

	if(!parse_rulebook())
	{
		cerr << "Failed at line " << token_line << " token " << str_token() << endl;
	}
}
