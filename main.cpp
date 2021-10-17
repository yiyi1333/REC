#include <iostream>
#include <stack>
#include <vector>
using namespace std;
string alphas;
class node{
private:
    char alpha;
    vector<node *> next;
public:
    node() {}
    node(char alpha) : alpha(alpha) {}
    node(char alpha, const vector<node *> &next) : alpha(alpha), next(next) {}

    char getAlpha() const {
        return alpha;
    }

    void setAlpha(char alpha) {
        node::alpha = alpha;
    }

    const vector<node *> &getNext() const {
        return next;
    }

    void setNext(const vector<node *> &next) {
        node::next = next;
    }

};

string add_symbol(string reg){
    string newreg;
    for(int i = 0; i < reg.length(); i++){
        if(reg[i] == '(' || reg[i] == '.' || reg[i] == '|'){
            newreg.push_back(reg[i]);
            continue;
        }
        else if(i + 1 < reg.length() && (alphas.find(reg[i + 1]) != string::npos || reg[i + 1] == '(' )){
            newreg.push_back(reg[i]);
            newreg.push_back('.');
        }
        else{
            newreg.push_back(reg[i]);
        }
    }
    return newreg;
}
//. | ( ) *
string translate(string oldstr){
    string newstr;
    stack<char> operatorstack;
    for(int i = 0;i < oldstr.length(); i++){
        if(oldstr[i] == '('){
            operatorstack.push(oldstr[i]);
        }
        else if(oldstr[i] == ')'){
            while(operatorstack.top() != '('){
                newstr.push_back(operatorstack.top());
                operatorstack.pop();
            }
            operatorstack.pop();
        }
        else if(oldstr[i] == '.' || oldstr[i] == '|'){
            while(operatorstack.top() == '*' || operatorstack.top() == '.' || operatorstack.top() == '|'){
                newstr.push_back(operatorstack.top());
                operatorstack.pop();
                if(operatorstack.empty()){
                    break;
                }
            }
            operatorstack.push(oldstr[i]);
        }
        else if(oldstr[i] == '*'){
            operatorstack.push(oldstr[i]);
        }
        else{
            newstr.push_back(oldstr[i]);
        }
    }
    while (!operatorstack.empty()){
        newstr.push_back(operatorstack.top());
        operatorstack.pop();
    }
    return newstr;
}

void caculate(string oldstr){
    for(int i = 0; i < oldstr.length(); i++){
        if(alphas.find(oldstr[i]) == string::npos && oldstr[i] != '(' &&  oldstr[i] != ')' && oldstr[i] != '.' && oldstr[i] != '|' && oldstr[i] != '*'){
            alphas.push_back(oldstr[i]);
        }
    }
}
int main() {
    string oldstr;
    cin >> oldstr;
    caculate(oldstr);
    string newstr = translate(add_symbol(oldstr));
    cout << newstr;
    return 0;
}