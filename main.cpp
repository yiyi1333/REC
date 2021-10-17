#include <iostream>
#include <stack>
#include <vector>
#include <map>
using namespace std;
string alphas;
class NFA{
private:
    string pe;
    vector<vector<pair<int, char> > > graph;
    pair<int, int> se;
    int statenum;
public:
    NFA(string str) : pe(str), statenum(0){}
    int newState(){
        graph.push_back(vector<pair<int, char>>());
        return statenum++;
    }
    void toNFA(){
        //状态栈
        stack< pair<int, int> > states;
        //状态边起点和终点编号
        int s, e;
        for(auto c : pe){
            //字符
            if(c != '*' && c != '.' && c != '|'){
                s = newState();
                e = newState();
                states.push(make_pair(s, e));
                graph[s].push_back(make_pair(e, c));
                continue;
            }
            //运算符
            switch (c){
                case '|':{
                    //取出栈顶两组
                    pair<int, int> one = states.top(); states.pop();
                    pair<int, int> two = states.top(); states.pop();
                    //合并之后的状态序号
                    s = newState();
                    e = newState();
                    //合并后的状态压入栈
                    states.push(make_pair(s, e));
                    //加入四条ε边
                    graph[s].push_back(make_pair(one.first, ' '));
                    graph[s].push_back(make_pair(two.first, ' '));
                    graph[one.second].push_back(make_pair(e, ' '));
                    graph[two.second].push_back(make_pair(e, ' '));
                    break;
                }
                case '*':{
                    //取出栈顶一组
                    pair<int, int> one = states.top(); states.pop();
                    //合并之后的状态序号
                    s = newState();
                    e = newState();
                    //合并后的状态压入栈
                    states.push(make_pair(s, e));
                    //构造ε边
                    graph[s].push_back(make_pair(one.first, ' '));
                    graph[s].push_back(make_pair(e, ' '));
                    graph[one.second].push_back(make_pair(e, ' '));
                    graph[one.second].push_back(make_pair(one.first, ' '));
                    break;
                }
                case '.':{
                    pair<int, int> two = states.top(); states.pop();
                    pair<int, int> one = states.top(); states.pop();
                    states.push(make_pair(one.first, two.second));
                    graph[one.second].push_back(make_pair(two.first, ' '));
                    break;
                }
                default:
                    break;
            }
        }
        se = make_pair(states.top().first, states.top().second);
    }
    void print(){
        cout << "statr: " << se.first << endl;
        cout << "end: " << se.second << endl;
        for(int i = 0; i < statenum; i++){
            for(auto edge : graph[i]){
                cout << i << "-----" << edge.second << "----->" << edge.first << endl;
            }
        }
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
    NFA nfa = NFA(newstr);
    nfa.toNFA();
    nfa.print();

//    (0|1)*0.10*
    return 0;
}