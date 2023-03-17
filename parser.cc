#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include<string>
#include <istream>
#include <cctype>
#include <stack>

//#include "parser.h"
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

typedef enum { private_variable = 0, public_variable, global_variable, unknown_variable
} VariableType;


string final_output = "";
string type_array[] = { "private", "public", "global", "unknown"};

struct Symbol_Object
{
    string scope_name = "";
    string variable_name = "";
    VariableType variable_type;
};


stack<string> execution_scope_stack;

vector<Symbol_Object> symbol_table;
vector<Symbol_Object> global_symbol_table;


LexicalAnalyzer lexer;
//Token token;

//All the required functions
void syntax_error();
Token expect(TokenType expected_type);

void parse_input();
void parse_program();
void parse_global_vars();
void parse_var_list(string scope_name, VariableType variable_type);
void parse_scope();
void parse_public_vars(string scope_name);
void parse_private_vars(string scope_name);
void parse_stmt_list(string scope_name);
void parse_stmt(string scope_name);

bool searchInSymbolTable( string variable_name);
bool searchInGlobalTable(string variable_name);
int returnIndexInSymbolTable( string variable_name);
bool simpleSearchSymbolTable(string unknown_variable_name);


int main(){

   
   // LexicalAnalyzer lexer;
   parse_input();
   cout << final_output;

//    for (int i = 0; i < symbol_table.size(); i++)
//    {
//        cout<<symbol_table[i].scope_name<<" "<<symbol_table[i].variable_name<<" "<< type_array[ symbol_table[i].variable_type ]<<endl;
//    }
   

    return 0;
}

bool searchInGlobalTable(string variable_name){
    for (int i = 0; i < global_symbol_table.size(); i++)
    {
        if(global_symbol_table[i].variable_name == variable_name){
            return true;
        }
    }
    return false;
    
}


int returnIndexInSymbolTable(string variable_name){
    //Access the execution scope stack and search in this manner.

    //copy the Execution scope stack.
    stack<string> copyOfExecutionStack = execution_scope_stack;
    int j = 0;

    while (!copyOfExecutionStack.empty())
    {
        string tempScopeName = copyOfExecutionStack.top();

        for (int i = 0; i < symbol_table.size(); i++)
        {
            if(symbol_table[i].scope_name != tempScopeName){
                continue;
            }
            if(j == 0){
                //for the first current scope
                VariableType tempType = symbol_table[i].variable_type;
                
                    if(symbol_table[i].variable_name == variable_name && (tempType == public_variable || tempType == private_variable || tempType == global_variable)){
                        return i;
                    }
                

            }else{
                //for the higher scope
                VariableType tempType = symbol_table[i].variable_type;
                
                    if(symbol_table[i].variable_name == variable_name && (tempType == public_variable || tempType == global_variable)){
                        return i;
                    }
            }
            
        }
        j++;
        copyOfExecutionStack.pop();
    }

    return -1;

}



bool searchInSymbolTable(string variable_name){
    //Access the execution scope stack and search in this manner.

    //copy the Execution scope stack.
    stack<string> copyOfExecutionStack = execution_scope_stack;
    int j = 0;

    while (!copyOfExecutionStack.empty())
    {
        string tempScopeName = copyOfExecutionStack.top();

        for (int i = 0; i < symbol_table.size(); i++)
        {
            if(symbol_table[i].scope_name != tempScopeName){
                continue;
            }
            if(j == 0){
                //for the first current scope
                VariableType tempType = symbol_table[i].variable_type;
                
                    if(symbol_table[i].variable_name == variable_name && (tempType == public_variable || tempType == private_variable || tempType == global_variable)){
                        return true;
                    }
                

            }else{
                //for the higher scope
                VariableType tempType = symbol_table[i].variable_type;
                
                    if(symbol_table[i].variable_name == variable_name && (tempType == public_variable || tempType == global_variable)){
                        return true;
                    }
            }
            
        }
        j++;
        copyOfExecutionStack.pop();
    }

    return false;  
}









void parse_stmt(string scope_name){
    Token t1 = lexer.peek(1);
    Token t2 = lexer.peek(2);

    if(t1.token_type == ID && t2.token_type == EQUAL){
        Token tp1 = expect(ID);

        //code to check if it is present
        if(searchInSymbolTable( tp1.lexeme)){  //the scope_name which we passed here is the "scope name" of the particular scope.

           int i = returnIndexInSymbolTable(tp1.lexeme);

           Symbol_Object tempSymbolObject = symbol_table[i];
           if(tempSymbolObject.variable_type == global_variable){
               final_output += tempSymbolObject.scope_name + tp1.lexeme;
            //    cout<<tempSymbolObject.scope_name<<tp1.lexeme;
           }else{
               final_output += tempSymbolObject.scope_name + "." + tp1.lexeme;
            //    cout<<tempSymbolObject.scope_name<<"."<<tp1.lexeme;
           }
           

        }else if(searchInGlobalTable(tp1.lexeme)){
            final_output += "::" + tp1.lexeme;
            // cout<<"::"<<tp1.lexeme;

        
        }else{
            //if not present then we need to add in the symbol table
            //Now, if we search using our search method, we may end up adding twice. So, think!!
            Symbol_Object *newSymbolObject = new Symbol_Object;
            newSymbolObject->scope_name = "?";
            newSymbolObject->variable_name = tp1.lexeme;
            newSymbolObject->variable_type = unknown_variable;

            symbol_table.push_back(*newSymbolObject);
            final_output += "?." + tp1.lexeme;
            // cout<<"?."<<tp1.lexeme;

            

        }
        

       // tp1.Print();
        Token tp3 = expect(EQUAL);
        // cout<<" = ";
        final_output += " = ";
        //tp3.Print();
        Token tp2 = expect(ID);

        if(searchInSymbolTable( tp2.lexeme)){

            int i = returnIndexInSymbolTable(tp2.lexeme);

            Symbol_Object tempSymbolObject = symbol_table[i];
            if(tempSymbolObject.variable_type == global_variable){
                // cout<<tempSymbolObject.scope_name<<tp2.lexeme<<endl;
                final_output += tempSymbolObject.scope_name + tp2.lexeme + "\n";
            }else{
                final_output += tempSymbolObject.scope_name + "." + tp2.lexeme + "\n";
                // cout<<tempSymbolObject.scope_name<<"."<<tp2.lexeme<<endl;
            }
            

        }else if(searchInGlobalTable(tp2.lexeme)){
            final_output += "::" + tp2.lexeme + "\n";
        //    cout<<"::"<<tp2.lexeme<<endl;
        
        }else{
            //if not present then we need to add in the symbol table
            Symbol_Object *newSymbolObject = new Symbol_Object;
            newSymbolObject->scope_name = "?";
            newSymbolObject->variable_name = tp2.lexeme;
            newSymbolObject->variable_type = unknown_variable;

            symbol_table.push_back(*newSymbolObject);
            // cout<<"?."<<tp2.lexeme<<endl;
            final_output += "?." + tp2.lexeme + "\n";
        }




        //tp2.Print();
        Token tp4 = expect(SEMICOLON);
        //tp4.Print();
    }else if(t1.token_type == ID && t2.token_type == LBRACE){
        parse_scope();
    }else{
        syntax_error();
    }
}


void parse_stmt_list(string scope_name){
    Token t1;
    parse_stmt(scope_name);
    t1 = lexer.peek(1);
    if(t1.token_type == ID){
        parse_stmt_list(scope_name);
    }else if(t1.token_type == RBRACE){
        return;
    }else{
        syntax_error();
    }
    
}

void parse_private_vars(string scope_name){
    Token t1 = lexer.peek(1);

    if(t1.token_type == PRIVATE){
        expect(PRIVATE);
        expect(COLON);
        parse_var_list( scope_name , private_variable);
        expect(SEMICOLON);
    } else if(t1.token_type == ID){
        return;
    }else{
        syntax_error();
    }
}

void parse_public_vars(string scope_name){
    Token t1 = lexer.peek(1);

    if(t1.token_type == PUBLIC){
        expect(PUBLIC);
        expect(COLON);
        parse_var_list(scope_name, public_variable);
        expect(SEMICOLON);
    }else if(t1.token_type == PRIVATE){
        return;
    }else if(t1.token_type == ID){
        return;
    }else{
        syntax_error();
    }
}

void parse_scope(){
    Token tempToken1;

    tempToken1 = expect(ID);
    execution_scope_stack.push(tempToken1.lexeme);

   // cout<<"Did it parse ID??"<<endl;
    expect(LBRACE);
   // cout<<"Did it parse LBRACE??"<<endl;

    parse_public_vars(tempToken1.lexeme);   //passing scopeName
    //cout<<"Did it parse vars??"<<endl;

    parse_private_vars(tempToken1.lexeme);
    parse_stmt_list(tempToken1.lexeme);
    expect(RBRACE);
    execution_scope_stack.pop();
}


void parse_var_list(string scope_name, VariableType variable_type ){

    Token t = expect(ID);

    //Here, we need to add the token to the list
    Symbol_Object *newSymbol = new Symbol_Object;
    (*newSymbol).scope_name = scope_name;
    newSymbol->variable_type = variable_type;
    newSymbol->variable_name = t.lexeme;

    symbol_table.push_back(*newSymbol);   //added a symbol

    if(scope_name == "::"){
        global_symbol_table.push_back(*newSymbol);
    }





    Token t1 = lexer.peek(1);
    if(t1.token_type == SEMICOLON){
        return;
    }else if(t1.token_type == COMMA){
        expect(COMMA);
        parse_var_list(scope_name,variable_type);
    }else{
        syntax_error();
    }
//     Token t1 = lexer.peek(1);
//     Token t2 = lexer.peek(2);

//    // if(t1.token_type == ID && t2.token_type == SEMICOLON){
//         Token t = expect(ID);
//         t.Print();

//     if(t1.token_type == ID && t2.token_type == COMMA){
//        Token t = expect(ID);
//        t.Print();
//         expect(COMMA);
//         parse_var_list();
//     }else{
//         syntax_error();
//     }
}



void parse_global_vars(){
    Token t1 = lexer.peek(1);
    Token t2 = lexer.peek(2);
    if(t1.token_type == ID && t2.token_type == LBRACE){
        //cout<<"Should Return"<<endl;
        return;
    }else if(t1.token_type == ID && (t2.token_type == COMMA || t2.token_type == SEMICOLON)){
        execution_scope_stack.push("::");
        parse_var_list("::" , global_variable);
        expect(SEMICOLON);
    }else{
        syntax_error();
    }
}


void parse_program(){
   // cout<<"In Parse Program"<<endl;
    parse_global_vars();
   // cout<<"Parsed Global variables"<<endl;
    parse_scope();
}




void parse_input(){
    //cout<<"Entering Input"<<endl;
    parse_program();
    expect(END_OF_FILE);
   // cout<<"Exiting Input"<<endl;

}




// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
//From Professor Rida Baazi's lecture
Token expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

void syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}





/*
bool searchInSymbolTable(string variable_name){
    //Access the execution scope stack and search in this manner.

    //copy the Execution scope stack.
    stack<string> copyOfExecutionStack = execution_scope_stack;
    int j = 0;

    while (!copyOfExecutionStack.empty())
    {
        string tempScopeName = copyOfExecutionStack.top();

        for (int i = 0; i < symbol_table.size(); i++)
        {
            if(symbol_table[i].scope_name != tempScopeName){
                continue;
            }
            if(j == 0){
                //for the first current scope
                VariableType tempType = symbol_table[i].variable_type;
                
                    if(symbol_table[i].variable_name == variable_name && (tempType == public_variable || tempType == private_variable || tempType == global_variable)){
                        return true;
                    }
                

            }else{
                //for the higher scope
                VariableType tempType = symbol_table[i].variable_type;
                
                    if(symbol_table[i].variable_name == variable_name && (tempType == public_variable || tempType == global_variable)){
                        return true;
                    }
            }
            
        }
        j++;
        copyOfExecutionStack.pop();
    }

    return false;  
}
*/