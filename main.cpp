#include <iostream>
#include<string.h>
#include <list> 
#include<vector>
#include <set> 
#include <fstream>
#include <algorithm>

using namespace std;

// Signatures of Functions that are non-terminals
string expression();
string expressionBar();
string term();
string termBar();
string factor();


set <string > operators={"+","-","*","/","(",")"};  
//vector containing tokens
vector<string> token;
//variable to track the consumption of tokens 
int index_ = 0; 


string expression(){

    // expr ->TermExpr`
    string str=term();
    str += expressionBar();
    return str;
}


string expressionBar(){

    // expr' ->+TermExpr`

    if(token[index_]=="+"){
        index_ ++;
        string str=term();
        str += expressionBar();
        str +="POP AX \n POP BX \n ADD AX,BX \n PUSH AX \n";
        return str;
    }
    // expr' -> -TermExpr`
    else if(token[index_]=="-"){
        index_ ++;
        string str=term();
        str += expressionBar();
        str +="POP BX \n POP AX \n SUB AX,BX \n PUSH AX \n";
        return str;
    }
    // expr' -> ebsilon
    else{
        return "";
    }
}


string term(){

    // Term ->FactorTerm`
    string str=factor();
    str += termBar();
    return str;
}

string termBar(){

    // Term` -> *FactorTerm`
    if(token[index_]=="*"){
        index_ ++;
        string str=factor();
        str += termBar();
        str +="POP AX \n POP BX \n MUL BX \n PUSH AX \n";
        return str;
    }
    // Term` -> /FactorTerm`
    else if(token[index_]=="/"){
        index_ ++;
        string str=factor();
        str += termBar();
        str +="POP BX \n POP AX \n MOV DX,0 \n DIV BX \n PUSH AX \n";
        return str;
    }
    // Term` -> %FactorTerm`
    else if(token[index_]=="%"){
        index_ ++;
        string str=factor();
        str += termBar();
        str +="POP AX \n POP BX \n MOV DX,0 \n DIV BX \n PUSH DX \n";
        return str;
    }
    // Term` -> ebsilon
    else{
        return "";
    }
}

string factor(){

 
    bool is_in = operators.find(token[index_]) != operators.end();

    // if token is a number then return number evaluation code is assembly
    if(!is_in){
        string str;
        str ="MOVE AX , " + token[index_] + " \n PUSH AX \n" ;
        index_++;
        return str;
    }
    // factor -> (expr)
    // If token is an opening paranthesis then call expr, 
    //and automatically all other function will be called recursively
    else if(token[index_]=="(") {
        index_++;
        string str = expression();
        //If '(' and 'expr' is consumed then also consume ')' 
        if(token[index_]==")"){
            index_++;
            return str;
        }
         // if after '(' there is no ')' then there is an error in expression   
        else
        {
            return "error";
        }
        
    }
    else{
        return "error";
    }
    
}





//Function to remove extra spaces 
string RemoveSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}




int main()
{
        set <char > operators2={'+','-','*','/','(',')'};    

              
        int counter=0;

               

        string exp="22      *( 50 +( 36 / 12 )- 16)";
 
        exp=RemoveSpaces(exp);

        string temp = "";

    //Making token of exp
    for (int i=0;i<exp.length();i++){
        // cout<<exp[i]<<endl;
         bool is_in = operators2.find(exp[i]) != operators2.end();
         // if it is an operator then store previous number and operator in the vector
         if(is_in)
         {
            //  cout<<exp[i]<<endl;
            //temp2 to store operator for pushing it into vector
            string temp2="";
            temp2+=exp[i];
            // most recent number is tored in temp
            token.push_back(temp);
            token.push_back(temp2);
            temp="";        
            
         }
         else{
             temp+=exp[i];
             
         }

    }
    
    
            token.push_back(temp);
            //Removing the extra spaces from the vector
            vector<string>::iterator it; 
            it = token.begin(); 

            for(int i =0;i<token.size();i++)
            {
                if(token[i].length()==0)
                {
                    token.erase(it);
                }
                it++;
                
            }
            
            // Displaying the token on output screen 
            for(int i =0;i<token.size();i++)
            {   

               bool is_in = operators.find(token[i]) != operators.end(); 
               if(is_in){
                    cout<<"Token[" << counter <<"] = "<< token[i] <<" (Operator)"<<endl;
                    counter++;
               }
                
               else{
                        cout<<"Token[" << counter << "] = "<<token[i]<<" (Number)"<<endl;
                        counter++;
                    }
                
            }



string s = expression();

//Writting the output to the file
    ofstream myfile;
    myfile.open ("output.asm");

    myfile<<"INCLUDE Irvine32.inc \n \n \n .code \n main PROC \n";
    myfile << s;
    myfile<< " MOV EAX,0 \n POP AX \n \n call WriteDec \n exit \n main ENDP \n END main";

    myfile.close();

cout<<s;

    return 0;
}