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
	   cur_char() == '/' || cur_char() == '%' ||
	   cur_char() == '<' || cur_char() == '>')
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

bool parse_expression(string& expr);
bool parse_expression_unquoted(string& expr);

bool parse_expression_factor(string& expr)
{
	string temp;
	if(token_type == TOKEN_OPERATOR && token_value == "(")
	{
		if(!parse_expression_unquoted(temp))
			return false;
		expr = "(" + temp + ")";
		if(token_type != TOKEN_OPERATOR || token_value != ")")
		{
			cerr << "Parse error in parse_expression_term(): expected operator ')'" << endl;
			return false;
		}
		next_token();
	}
	else if(token_type == TOKEN_OPERATOR && token_value == "-")
	{
		next_token();
		if(!parse_expression_unquoted(temp))
			return false;
		expr = "-" + temp;
	}
	else if(token_type == TOKEN_NUMBER)
	{
		expr = token_value;
		next_token();
		return true;
	}
	else if(token_type == TOKEN_VARIABLE)
	{
		expr = token_value;
		next_token();
		return true;
	}
	else if(token_type == TOKEN_PYTHON_CODE)
	{
		expr = "(" + token_value  + ")";
		next_token();
		return true;
	}
	else
	{
		cerr << "Parse error in parse_expression_factor(): expected number or variable" << endl;
		return false;
	}

	return true;
}

bool parse_expression_term(string& expr)
{
	expr.clear();
	string temp;
	while(true)
	{
		if(!parse_expression_factor(temp))
			return false;
		expr += temp;

		if(token_type == TOKEN_OPERATOR &&
		   (token_value == "*" || token_value == "/" || token_value == "%"))
		{
			expr += " " + token_value + " ";
			next_token();
		}
		else
		{
			return true;
		}
	}
}

bool parse_expression_noncompare(string& expr)
{
	expr.clear();
	string temp;
	while(true)
	{
		temp.clear();
		if(!parse_expression_term(temp))
			return false;
		expr += temp;

		if(token_type == TOKEN_OPERATOR &&
		   (token_value == "+" || token_value == "-"))
		{
			expr += " " + token_value + " ";
			next_token();
		}
		else
		{
		    return true;
		}
	}
}

bool parse_expression_unquoted(string& expr)
{
	string temp;
	if(!parse_expression_noncompare(temp))
		return false;
	expr += temp;

	if((token_type == TOKEN_KEYWORD && token_value == "is") ||
	   (token_type == TOKEN_OPERATOR && (token_value == "<" || token_value == ">")))
	{
		expr += " " + token_value + " ";
		next_token();
		if(!parse_expression_noncompare(temp))
			return false;
		expr += temp;
	}

	return true;
}

bool parse_expression(string& expr)
{
	expr = "\"";

	string temp;
	if(!parse_expression_unquoted(temp))
		return false;
	expr += temp;

	expr += "\"";

	return true;
}

bool parse_statement(string& json);

bool parse_conditional(string& json)
{
	json = "{\"type\":\"cond\",\"cond_list\":";
	if(token_type != TOKEN_KEYWORD || token_value != "if")
	{
		cerr << "Parse error in parse_conditional(): expected 'if'" << endl;
		return false;
	}

	string temp;
	next_token();
	if(!parse_expression(temp))
		return false;
	
	json += "[{\"type\":\"if\",\"condition\":\"" + temp + "\",\"updates\":[";

	if(token_type != TOKEN_DELIMITER || token_value != ":")
	{
		cerr << "Parse error in parse_conditional(): expected ':'" << endl;
		return false;
	}

	next_token();
	if(!parse_statement(temp))
		return false;

	json += temp + "]}";

	if(token_type == TOKEN_KEYWORD && token_value == "else")
	{
		next_token();
		if(token_type == TOKEN_DELIMITER && token_value == ":")
		{
			next_token();
			if(!parse_statement(temp))
				return false;
			json += ",{\"type\":\"else\",\"condition\":\"\",\"updates\":[" + temp + "]}]";
		}
		else if(token_type == TOKEN_KEYWORD && token_value == "if")
		{
			if(!parse_conditional(temp))
				return false;
			json += ",{\"type\":\"else\",\"condition\":\"\",\"updates\":[" + temp + "]}]";
		}
		else
		{
			cerr << "Parse error in parse_conditional(): expected ':' or 'if'" << endl;
			return false;
		}
	}

	json += "}";

	return true;
}

bool parse_assignment(string& json)
{
	if(token_type != TOKEN_VARIABLE)
	{
		cerr << "Parse error in parse_assignment(): expected variable name." << endl;
		return false;
	}

	json = "{\"type\":\"set\",\"field\":\"" + token_value + "\",\"value\":";

	next_token();
	if(token_type != TOKEN_KEYWORD && token_value != "is")
	{
		cerr << "Parse error in parse_assignment(): expected keyword \"is\"." << endl;
		return false;
	}

	string temp;
	next_token();
	if(token_type == TOKEN_STRING)
	{
		json += "\"\\\"" + token_value + "\\\"\"}";
		next_token();
		return true;
	}
	else if(!parse_expression(temp))
		return false;

	json += temp + "}";

	return true;
}

bool parse_statement(string& json)
{
	json = "{\"type\":\"update_list\",\"list\":[";
	string cur_statement;
	bool nfirst = false;
	bool done = false;
	while(!done)
	{
		cur_statement.clear();

		if(token_type == TOKEN_KEYWORD && token_value == "if")
		{
			if(!parse_conditional(cur_statement))
				return false;
			done = true;
		}
		else
		{
			if(token_type == TOKEN_VARIABLE)
			{
				if(!parse_assignment(cur_statement))
					return false;
			}
			else if(token_type == TOKEN_KEYWORD && token_value == "say")
			{
				next_token();
				if(token_type != TOKEN_STRING)
				{
					cerr << "Parse error in parse_statement(): expected string" << endl;
					return false;
				}
				cur_statement = "{\"type\":\"say\",\"say_string\":\"" + token_value + "\"}";
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
				cur_statement = "{\"type\":\"get\",\"get_field\":\"" + token_value + "\"}";
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
			    done = true;
			}
		}

		if(nfirst)
			json += ",";
		json += cur_statement;
	}

	json += "]}";
	
	return true;
}

bool parse_state(string& json)
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
	json = "\"" + token_value + "\":";

	next_token();
	if(token_type != TOKEN_DELIMITER || token_value != ":")
	{
		cerr << "Parse error in parse_state(): expected ':'" << endl;
		return 1;
	}
		
	next_token();
	json += "[";
	bool nfirst = false;
	while((token_type != TOKEN_KEYWORD || token_value != "state") && token_type != TOKEN_EOF)
	{
		string temp;
		if(!parse_statement(temp))
			return false;
		if(nfirst)
			json += ",";
		json += temp;
		nfirst = true;
	}
	json += "]";

	return true;
}

bool parse_rulebook(string& json)
{
	string temp;
	json = "{";
	bool nfirst = false;
	while(token_type != TOKEN_EOF)
	{
		if(!parse_state(temp))
			return false;
		if(nfirst)
			json += ",";
		json += temp;
		nfirst = true;
	}
	json += "}";
	
	return true;
}

int main()
{
	tokenizer_init();

	string json;
	if(!parse_rulebook(json))
	{
		cerr << "Failed at line " << token_line << " token " << str_token() << endl;
	}
	else
	{
		cout << json << endl;
	}
}
