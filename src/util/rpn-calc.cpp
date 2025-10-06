// Let's implement RPN with linked-list stack and stack from c++ STL(Standard Template library)
// supported operators (+, -, *, /, (, ) )

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <stack>
#include <stdexcept>
#include <cctype>

using namespace std;

// supported operators
const set<string> SUPPORTED_OPERATORS = {"+", "-", "*", "/", "(", ")"};

// linked-list stack
struct node {
    int value;
    node* next;
};

typedef struct node my_stack;

// Stack functions
void create(my_stack** top) {
    *top = nullptr;
}

int isEmpty(my_stack** top) {
    return *top == nullptr;
}

void push(my_stack** top, int element) {
    my_stack* new_node = new my_stack();
    new_node->value = element;
    new_node->next = *top;
    *top = new_node;
}

int pop(my_stack** top) {
    if (isEmpty(top)) return -1;
    int val = (*top)->value;
    my_stack* temp = *top;
    *top = (*top)->next;
    delete temp;
    return val;
}

// Helper functions
bool isNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

int precedence(const string& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

vector<string> infix_to_postfix(const string& infix_expression) {
    stringstream ss(infix_expression);
    vector<string> postfix;
    string token;
    stack<string> operations_stack;

    while (ss >> token) {
        if (isNumber(token)) {
            postfix.push_back(token);
        } else if (SUPPORTED_OPERATORS.count(token)) {
            if (token == "(") {
                operations_stack.push(token);
            } else if (token == ")") {
                while (!operations_stack.empty() && operations_stack.top() != "(") {
                    postfix.push_back(operations_stack.top());
                    operations_stack.pop();
                }
                if (operations_stack.empty()) throw invalid_argument("Mismatched parentheses");
                operations_stack.pop();
            } else {
                while (!operations_stack.empty() && precedence(operations_stack.top()) >= precedence(token) &&
                       operations_stack.top() != "(") {
                    postfix.push_back(operations_stack.top());
                    operations_stack.pop();
                }
                operations_stack.push(token);
            }
        } else {
            throw invalid_argument("Invalid token: " + token);
        }
    }

    while (!operations_stack.empty()) {
        if (operations_stack.top() == "(" || operations_stack.top() == ")")
            throw invalid_argument("Mismatched parentheses");
        postfix.push_back(operations_stack.top());
        operations_stack.pop();
    }

    return postfix;
}

int postfix_to_value(const vector<string>& postfix) {
    my_stack* top;
    create(&top);

    for (const string& token : postfix) {
        if (isNumber(token)) {
            push(&top, stoi(token));
        } else if (SUPPORTED_OPERATORS.count(token)) {
            int val2 = pop(&top);
            int val1 = pop(&top);
            if (token == "+") push(&top, val1 + val2);
            else if (token == "-") push(&top, val1 - val2);
            else if (token == "*") push(&top, val1 * val2);
            else if (token == "/") {
                if (val2 == 0) throw runtime_error("Division by zero");
                push(&top, val1 / val2);
            }
        }
    }

    return pop(&top);
}

string format_infix(const string& infix) {
    string corrected_infix;
    size_t i = 0;
    while (i < infix.size()) {
        if (isspace(infix[i])) { ++i; continue; }
        else if (isdigit(infix[i])) {
            string num;
            while (i < infix.size() && isdigit(infix[i])) { num += infix[i]; ++i; }
            corrected_infix += num + " ";
        } else {
            string op(1, infix[i]);
            if (SUPPORTED_OPERATORS.count(op)) { corrected_infix += op + " "; ++i; }
            else throw invalid_argument("Invalid token: " + op);
        }
    }
    return corrected_infix;
}


int calculate_internal(const string& infix_input) {
    string formatted = format_infix(infix_input);
    vector<string> postfix = infix_to_postfix(formatted);
    return postfix_to_value(postfix);
}

// ===== WASM Exports =====
extern "C" {

    int calculate(const char* expr) {
        try {
            string s(expr);
            vector<string> postfix = infix_to_postfix(format_infix(s));
            return postfix_to_value(postfix);
        } catch (...) { return -999999; }
    }

    const char* formatInfix(const char* expr) {
        try {
            string s(expr);
            static string formatted;
            formatted = format_infix(s);
            return formatted.c_str();
        } catch (...) { return "ERROR"; }
    }

    const char* toPostfix(const char* expr) {
        try {
            string s(expr);
            static string postfix_str;
            vector<string> postfix = infix_to_postfix(format_infix(s));
            postfix_str = "";
            for (auto &tok : postfix) postfix_str += tok + " ";
            return postfix_str.c_str();
        } catch (...) { return "ERROR"; }
    }

    int evaluatePostfix(const char* postfix_expr) {
        try {
            string s(postfix_expr);
            stringstream ss(s);
            vector<string> tokens;
            string token;
            while (ss >> token) tokens.push_back(token);
            return postfix_to_value(tokens);
        } catch (...) { return -999999; }
    }

}