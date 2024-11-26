#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept> // For runtime_error
#include <vector>
#include <sstream>

using namespace std;

// Evaluator class declaration
class Evaluator {
public:
    int eval(const string& expression);
    void printOrderOfComputations(const string& expression);
private:
    bool isOperator(const string& op);
    int precedence(const string& op);
    int performOperation(int a, int b, const string& op);
    int evaluateUnary(int a, const string& op);
    bool isValidExpression(const string& expression);
    bool hasBalancedParentheses(const string& expression);
    bool handleErrors(const string& expression);
};

// Main function
int main() {
    Evaluator eval;
    string expression;

    cout << "Welcome to the Expression Evaluator!" << endl;
    cout << "You can test various expressions (type 'exit' to quit)." << endl;
    cout << "Examples of valid expressions: 1 + 2 * 3, ++++2-5*(3^2)" << endl;
    cout << "-----------------------------------------------------------" << endl;

    while (true) {
        cout << "\nEnter an expression: ";
        getline(cin, expression);

        // Exit condition
        if (expression == "exit") {
            cout << "Goodbye!" << endl;
            break;
        }

        // Evaluate the expression
        try {
            bool broke = false;
            int result = eval.eval(expression);
            if (result != -1) { // Only display result if the evaluation was successful
                cout << "Result: " << result << endl;
                for (int x = 0; x < expression.length(); x++) {
                    if ((((expression[x] == '+') && (expression[x + 1] == '+') && (expression[x + 2] == '+') && (expression[x + 3] == '+')) || ((expression[x] == '-') && (expression[x + 1] == '-') && (expression[x + 2] == '-') && (expression[x + 3] == '-')))) {
                        broke = true;
                    }
                }
                if (!broke) {
                    eval.printOrderOfComputations(expression);
                }
            }
        }
        catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}

void Evaluator::printOrderOfComputations(const string& expression) {
    if (!isValidExpression(expression)) {
        cout << "Error: Invalid expression." << endl;
        return;
    }

    stack<int> values;           // Stack for values
    stack<string> operators;     // Stack for operators
    vector<string> computationOrder; // Vector to store computation steps

    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];

        // Skip whitespace
        if (isspace(ch)) continue;

        // Handle numbers (multi-digit)
        if (isdigit(ch)) {
            int num = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                ++i;
            }
            --i;
            values.push(num);
        }
        else if (ch == '(') {
            operators.push(string(1, ch));
        }
        else if (ch == ')') {
            while (!operators.empty() && operators.top() != "(") {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                string op = operators.top();
                operators.pop();
                int result = performOperation(val1, val2, op);

                // Record computation
                stringstream step;
                step << "(" << val1 << " " << op << " " << val2 << ")";
                computationOrder.push_back(step.str());

                values.push(result);
            }
            operators.pop();
        }
        else {
            string op(1, ch);

            // Handle multi-character operators
            if (i + 1 < expression.length()) {
                string potentialOp = op + expression[i + 1];
                if (isOperator(potentialOp)) {
                    op = potentialOp;
                    ++i;
                }
            }

            while (!operators.empty() && precedence(operators.top()) >= precedence(op)) {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                string topOp = operators.top();
                operators.pop();
                int result = performOperation(val1, val2, topOp);

                // Record computation
                stringstream step;
                step << "(" << val1 << " " << topOp << " " << val2 << ")";
                computationOrder.push_back(step.str());

                values.push(result);
            }
            operators.push(op);
        }
    }

    // Process remaining operators
    while (!operators.empty()) {
        int val2 = values.top();
        values.pop();
        int val1 = values.top();
        values.pop();
        string topOp = operators.top();
        operators.pop();
        int result = performOperation(val1, val2, topOp);

        // Record computation
        stringstream step;
        step << "(" << val1 << " " << topOp << " " << val2 << ")";
        computationOrder.push_back(step.str());

        values.push(result);
    }

    // Print the order of computations
    cout << "Order of computations:" << endl;
    for (const auto& step : computationOrder) {
        cout << step << endl;
    }
}


// Check if a string is an operator
bool Evaluator::isOperator(const string& op) {
    static const string validOperators[] = { "+", "-", "*", "/", "%", "^", "!", "&&", "||",
                                            "==", "!=", "<", ">", "<=", ">=", "++", "--" };
    for (const string& validOp : validOperators) {
        if (op == validOp) return true;
    }
    return false;
}

// Return precedence of an operator
int Evaluator::precedence(const string& op) {
    if (op == "!" || op == "++" || op == "--") return 8;
    if (op == "^") return 7;
    if (op == "*" || op == "/" || op == "%") return 6;
    if (op == "+" || op == "-") return 5;
    if (op == "<" || op == ">" || op == "<=" || op == ">=") return 4;
    if (op == "==" || op == "!=") return 3;
    if (op == "&&") return 2;
    if (op == "||") return 1;
    return 0;
}

// Perform binary operations
int Evaluator::performOperation(int a, int b, const string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : throw runtime_error("Division by zero");
    if (op == "%") return a % b;
    if (op == "^") return pow(a, b);
    if (op == "&&") return a && b;
    if (op == "||") return a || b;
    if (op == "==") return a == b;
    if (op == "!=") return a != b;
    if (op == "<") return a < b;
    if (op == ">") return a > b;
    if (op == "<=") return a <= b;
    if (op == ">=") return a >= b;
    throw runtime_error("Invalid operator");
}

// Perform unary operations
int Evaluator::evaluateUnary(int a, const string& op) {
    if (op == "!") return !a;
    if (op == "-") return -a;
    if (op == "+") return a;
    if (op == "++") return a + 1;
    if (op == "--") return a - 1;
    throw runtime_error("Invalid unary operator");
}

// Check if the expression is valid
bool Evaluator::isValidExpression(const string& expression) {

    if (expression[0] == ')') {
        cout << "Expression can't start with a closing parenthesis @ char: 0" << endl;
        return false;
    }

    if (expression[0] == '<' || expression[0] == '>') {
        cout << "Expression can't start with a binary operator @ char: 0" << endl;
        return false;
    }

    if (!hasBalancedParentheses(expression)) {
        cout << "Error: Unbalanced parentheses." << endl;
        return false;
    }

    bool lastWasOperator = true; // Indicates the last character was an operator or start of the expression
    int openParentheses = 0; // Count of open parentheses

    for (size_t i = 0; i < expression.length(); ++i) {

        if ((expression[i] == '/') && (expression[i + 1] == '0')) {
            cout << "Division by zero @ char " << i + 1 << endl;
            return false;
        }

        if ((expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '&' || expression[i] == '|' || expression[i] == '^' || expression[i] == '!') && (expression[i + 1] == '>' || expression[i + 1] == '<')) {
            cout << "A unary operand can't be followed by a binary operator @ char: " << i + 1 << endl;
            return false;
        }

        if ((isspace(expression[i])) && (i > 0) && (isdigit(expression[i - 1]) && isdigit(expression[i + 1]))) {
            cout << "Two operands in a row @ char " << i + 1 << endl;
            return false;
        }
        // Skip whitespace
        if (isspace(expression[i])) continue;
        if ((expression[i] == '&' && expression[i + 1] == '&' && expression[i + 2] == '&' && expression[i + 3] == '&') || (expression[i] == '|' && expression[i + 1] == '|' && expression[i + 2] == '|' && expression[i + 3] == '|')) {
            cout << "Two binary operators in a row @ char " << i + 2 << endl;
            return false;
        }
        // Handle multi-character operators
        string currentOp(1, expression[i]);
        if (i + 1 < expression.length()) {
            string potentialOp = currentOp + expression[i + 1];
            if (isOperator(potentialOp)) {
                currentOp = potentialOp;
                ++i; // Skip the next character as it's part of the multi-character operator
            }
        }

        // Check for invalid characters
        if (!isdigit(expression[i]) && !isOperator(currentOp) && expression[i] != '(' && expression[i] != ')') {
            cout << "Error: Invalid character '" << expression[i] << "' at position " << i << "." << endl;
            return false;
        }

        // Handle parentheses
        if (expression[i] == '(') {
            openParentheses++;
            lastWasOperator = true; // Opening parenthesis acts like an operator
        }
        else if (expression[i] == ')') {
            openParentheses--;
            if (openParentheses < 0) {
                cout << "Error: Misplaced closing parenthesis at position " << i << "." << endl;
                return false;
            }
            lastWasOperator = false; // Closing parenthesis must follow an operand
        }

        // Handle operators
        if (isOperator(currentOp)) {
            if (currentOp == "++" || currentOp == "--") {
                if (lastWasOperator) continue; // Allow unary increment or decrement at the start or after an operator
            }
            else if (lastWasOperator && currentOp != "+" && currentOp != "-") {
                cout << "Error: Misplaced operator '" << currentOp << "' at position " << i << "." << endl;
                return false;
            }
            lastWasOperator = true; // Operators must be followed by an operand
        }
        else if (isdigit(expression[i])) {
            lastWasOperator = false; // Digits are operands
        }
    }

    // Check if there are unmatched open parentheses
    if (openParentheses != 0) {
        cout << "Error: Unmatched open parentheses." << endl;
        return false;
    }

    // Check if the expression ends with an operator
    if (lastWasOperator) {
        cout << "Error: Expression ends with an operator." << endl;
        return false;
    }

    return true;
}

// Evaluate the expression using stacks
int Evaluator::eval(const string& expression) {
    if (!isValidExpression(expression)) {
        cout << "Error: Invalid expression." << endl;
        return -1;
    }

    stack<int> values;        // Stack for storing values (operands)
    stack<string> operators;  // Stack for storing operators

    for (size_t i = 0; i < expression.length(); ++i) {
        char ch = expression[i];

        // Skip whitespace
        if (isspace(ch)) continue;

        // Handle numbers (support multi-digit numbers)
        if (isdigit(ch)) {
            int num = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                ++i;
            }
            --i;  // Adjust index after the loop
            values.push(num);
        }
        // Handle opening parenthesis
        else if (ch == '(') {
            operators.push(string(1, ch));
        }
        // Handle closing parenthesis
        else if (ch == ')') {
            while (!operators.empty() && operators.top() != "(") {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                string op = operators.top();
                operators.pop();
                values.push(performOperation(val1, val2, op));
            }
            // Pop the opening parenthesis
            if (!operators.empty() && operators.top() == "(") {
                operators.pop();
            }
        }
        // Handle operators
        else {
            string op(1, ch);

            // Handle multi-character operators
            if (i + 1 < expression.length()) {
                string potentialOp = op + expression[i + 1];
                if (isOperator(potentialOp)) {
                    op = potentialOp;
                    ++i;
                }
            }

            // Handle unary `++` and `--` operators
            if (op == "++" || op == "--") {
                int topValue = 0;

                // If there are values on the stack, apply the operator to the top value
                if (!values.empty()) {
                    topValue = values.top();
                    values.pop();
                }

                // Apply the unary operation
                topValue = evaluateUnary(topValue, op);

                // Push the updated value back onto the stack
                values.push(topValue);
                continue;
            }

            // Handle unary `+` and `-` at the beginning of the expression or after another operator
            if ((op == "+" || op == "-") && (values.empty() || !isdigit(expression[i - 1]))) {
                int value = 0; // Default operand is 0
                if (!values.empty()) {
                    value = values.top();
                    values.pop();
                }
                value = evaluateUnary(value, op);
                values.push(value);
                continue;
            }

            // Process operator precedence and associativity
            while (!operators.empty() && precedence(operators.top()) >= precedence(op)) {
                int val2 = values.top();
                values.pop();
                int val1 = values.top();
                values.pop();
                string topOp = operators.top();
                operators.pop();
                values.push(performOperation(val1, val2, topOp));
            }
            operators.push(op);
        }
    }

    // Final evaluation for remaining operators
    while (!operators.empty()) {
        int val2 = values.top();
        values.pop();
        int val1 = values.top();
        values.pop();
        string topOp = operators.top();
        operators.pop();
        values.push(performOperation(val1, val2, topOp));
    }

    // If there are still multiple values in the stack, reduce them using addition
    while (values.size() > 1) {
        int val1 = values.top();
        values.pop();
        int val2 = values.top();
        values.pop();
        values.push(val1 + val2); // Combine the values (default to addition)
    }

    // Return the final result
    return values.top();
}




// Check for balanced parentheses
bool Evaluator::hasBalancedParentheses(const string& expression) {
    stack<char> stack;
    for (char ch : expression) {
        if (ch == '(') {
            stack.push(ch);
        }
        else if (ch == ')') {
            if (stack.empty()) return false;
            stack.pop();
        }
    }
    return stack.empty();
}
