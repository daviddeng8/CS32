//
//  eval.cpp
//  CS32 Homework 2
//
//  Created by David Deng on 2/3/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include "Map.h"
using namespace std;

bool syntacticallyValidInfix(string infix);
bool operandsInMap(string infix, const Map& values);
void infixToPostfix(string infix, string& postfix);
bool postfixToResult(const Map& values, const string& postfix, int& result);

int evaluate(string infix, const Map& values, string& postfix, int& result) {
    if (!syntacticallyValidInfix(infix)) {
        return 1;
    }
    
    infixToPostfix(infix, postfix);
    
    if (!operandsInMap(infix, values)) {
        return 2;
    }
    
    if (!postfixToResult(values, postfix, result)) {
        return 3;
    }
    
    return 0;
}

bool syntacticallyValidInfix(string infix) {
    //first and last can't be operators
    //can't have two operators or two operands in a row
    //ending should have one more operand than operator
    //parentheses all need to be proper
    
    //number of operands can't be 2 more than operators, and vice versa
    
    // 5 + 3 + (2 +)
    
    //when you reach an end paren, there also has to be 1 more operand than operator
    
    //5 + 3 + (+ 2 2) caught by the no two in a row
    
    //5 + 3 + (+ 2) caught by has to have more operand than operator
    
    //5 + 3 + (2 +) same as before
    
    //if either of these counts exceeds 1, there means that there were two operators or two operands in a row, and that's not syntactically valid
    int operandCount = 0;
    int operatorCount = 0;
    
    //stack to keep track of parentheses
    stack<char> parens;
    
    for (int i = 0; i < infix.size(); i++) {
        //checks to make sure there's not two operands or operators in a row
        if (operandCount - operatorCount > 1 || operatorCount - operandCount > 0) {
            return false;
        }
        if (isalpha(infix[i])) {
            if (!islower(infix[i])) {
                return false;
            }
            operandCount++;
        }
        else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
            operatorCount++;
        }
        else if (infix[i] == '(') {
            if (operandCount != operatorCount) { //
                return false;
            }
            parens.push(infix[i]);
        }
        else if (infix[i] == ')') {
            if (parens.empty()) {
                return false; //improper end parentheses
            }
            if (operandCount != operatorCount + 1) {
                return false; //examples like 5 + 3 + (4 +), there must be a complete statement within the parentheses
            }
            parens.pop();
        }
    }
    
    if (!parens.empty()) {
        return false;
    }
    
    if (operandCount != operatorCount + 1) {
        return false;
    }
    
    return true;
}

bool operandsInMap(string infix, const Map& values) {
    for (int i = 0; i < infix.size(); i++) {
        //no need to check for lowercase because previous check accounts for it
        if (isalpha(infix[i])) {
            if (!values.contains(infix[i])) {
                return false;
            }
        }
    }
    return true;
}

void infixToPostfix(string infix, string& postfix) {
   Map precedence;
   precedence.insertOrUpdate('+', 0);
   precedence.insertOrUpdate('-', 0);
   precedence.insertOrUpdate('*', 1);
   precedence.insertOrUpdate('/', 1);
    
   postfix = "";
   stack<char> operators;
   
   for (int i = 0; i < infix.size(); i++) {
       if (isalpha(infix[i])) {
           postfix += infix[i];
       }
       else if (infix[i] == '(') {
           operators.push(infix[i]);
       }
       else if (infix[i] == ')') {
           char next = operators.top();
           while (next != '(') {
               postfix += next;
               operators.pop();
               next = operators.top();
           }
           operators.pop(); //to remove the '('
       }
       else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
           int precedenceNew;
           precedence.get(infix[i], precedenceNew);
           
           int precedencePrev = -1;
           if (!operators.empty()) {
               precedence.get(operators.top(), precedencePrev); //the case where the top is a '(' is checked for in the while loop
           }
           
           while (operators.empty() == false && operators.top() != '(' && precedenceNew <= precedencePrev) {
               postfix += operators.top();
               operators.pop();
           }
           operators.push(infix[i]);
       }
   }
   
   while (operators.empty() == false) {
       postfix += operators.top();
       operators.pop();
   }
    
}

bool postfixToResult(const Map& values, const string& postfix, int& result) {
    //postfix to result
    stack<int> operands;
    for (int i = 0; i < postfix.size(); i++) {
        if (isalpha(postfix[i])) {
            int getVal;
            values.get(postfix[i], getVal);
            operands.push(getVal);
        }
        else {
            int operand2 = operands.top();
            operands.pop();
            int operand1 = operands.top();
            operands.pop();
            
            switch (postfix[i]) {
                case '+':
                    operands.push(operand1 + operand2);
                    break;
                case '-':
                    operands.push(operand1 - operand2);
                    break;
                case '*':
                    operands.push(operand1 * operand2);
                    break;
                case '/':
                    if (operand2 == 0) {
                        return false;
                    }
                    operands.push(operand1 / operand2);
                    break;
                default:
                    break;
            }
        }
    }
    result = operands.top();
    return true;
}


