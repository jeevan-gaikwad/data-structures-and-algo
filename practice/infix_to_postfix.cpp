#include <iostream>
#include <stack>
#include <string>
#include <sstream>
using namespace std;
//Simple infix string: A + B * 5
class StringConverter
{

private:
	class Opr
	{
		public:
			char oper;
			int prcdnc;
			Opr(char oper, short prcdnc):oper{oper},prcdnc{prcdnc} { }
	};

	Opr* operators;
	
	bool precedence(const char op1,const char op2);
	bool isOperator(char oper);


public:
	void postfix(string& infix, string& out_postfix);
	StringConverter()
	{
		
		Opr sum('+', 1), subtract('-',1), mul('*', 2), div('/',2), openParanth('(',0);
		operators = new Opr[5]  { sum, subtract, mul, div, openParanth };
	}
};

bool StringConverter::precedence(const char op1,const char op2)
{
	cout<<"Evaluating precedence of "<< op1 <<" & "<<op2<<":";
	int op1Prcdnc = 0, op2Prcdnc = 0, bothFound = 0;
	//Some hardcoded results
	if(op1 == '(' || op2 == '(')
	{
		return true;
	}else if(op2 == ')')
	{
		if(op1 == '(')
			return false;
		else
			return true;
	}

		
	for(int i=0; i< 5; i++)
	{
		if(operators[i].oper == op1)
		{
			op1Prcdnc = operators[i].prcdnc;
			bothFound++;
		}
		if(operators[i].oper == op2)
		{
			op2Prcdnc = operators[i].prcdnc;
			bothFound++;
		}
	}
	if(bothFound == 2)
	{
		if(op1Prcdnc > op2Prcdnc)
		{
			cout<<op1<<" has higher precedence than "<<op2<<endl;
			return true;
		}
		//false
	}else
	{
		cout<<"Both operators not found :("<<endl;
	}


	cout<<op1<<" does not have higher precedence than "<<op2<<endl;
	return false;
}

bool StringConverter::isOperator(char oper)
{
	cout<<"Evaluating whether " << oper <<" is an operator or not -> ";
	for(int i=0; i< 5; i++)
	{
		if(operators[i].oper == oper)
		{
			cout<<" Yes! It is."<<endl;
			return true;
		}
	}
	cout<<"No. Its not."<<endl;
	return false;
}


void StringConverter::postfix(string& infix, string& out_postfix)
{
	string::iterator infix_iter = infix.begin(); 
	stringstream output_stream;	
	stack<char> operStack;
	while(infix_iter != infix.end())
	{
		if(isOperator(*infix_iter))
		{
			if(!operStack.empty())
			{
				if(precedence(*infix_iter, operStack.top()))
				{
					operStack.push(*infix_iter);
					cout<<"Pushed:"<<*infix_iter<<endl;
					infix_iter++;
					continue;
				}
				output_stream << operStack.top();//output stack top
				cout<<"Popped:"<<operStack.top()<<endl;
				operStack.pop();
				continue;
			}
			operStack.push(*infix_iter);
			cout<<"Pushed:"<<*infix_iter<<endl;
			infix_iter++;
			continue;
		}
		if(*infix_iter == ')' && !operStack.empty())
		{
			while(operStack.top() != '(')
			{
				cout<<"Popped:"<<operStack.top()<<endl;
				output_stream<<operStack.top();
				operStack.pop();
			}
			operStack.pop(); //Discard '('
			infix_iter++;
			continue;
		}
		//flush the operand to output stream
		output_stream<<*infix_iter;
		infix_iter++;
	}
	
	while(!operStack.empty())
	{
		output_stream<<operStack.top();
		operStack.pop();
	}
	out_postfix = output_stream.str();
}

int main(void)
{
	string infix_str, postfix_str;
	cin>>infix_str;
	cout<<"You just entered:"<<infix_str<<endl;
	StringConverter stringConverter;
	stringConverter.postfix(infix_str,postfix_str);
	cout<<"Post fix string is:"<<postfix_str<<endl;
	exit(0);
}
