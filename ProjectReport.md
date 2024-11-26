assumpotions, all inputs are integers




UML class diagram

Class: Evaluator

  Public Methods:
  
    int eval(const string& expression)
    
    void printOrderOfComputations(const string& expression)
    
  Private Methods:
  
    bool isOperator(const string& op)
    
    int precedence(const string& op)
    
    int performOperation(int a, int b, const string& op)
    
    int evaluateUnary(int a, const string& op)
    
    bool isValidExpression(const string& expression)
    
    bool hasBalancedParentheses(const string& expression)
    
    bool handleErrors(const string& expression)
    
  Relationships:
  
    Utilizes stack and vector STL containers.


time complextity 

eval

O(n)

isValidExpression

O(n)

printOrderOfComputations

O(n)

hasBalancedParentheses

O(n)

contributers
did project alone

refrences
mostly gpt for questions and debugs
