#include <iostream>
#include <stack>
#include <string>
#include <map>

#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// 表达式数结构
class CalculateNode {
public:
	// 构造函数
	CalculateNode() {
		this->value = "";
		this->lchild = NULL;
		this->rchild = NULL;
	}
	CalculateNode(string value, CalculateNode* l, CalculateNode* r) :
		value(value), lchild(l), rchild(r) {}

	// 左孩子
	CalculateNode* ls() { return this->lchild; }

	// 右孩子
	CalculateNode* rs() { return this->rchild; }

	// 显示值
	string data() { return value; }

private:
	string value;
	CalculateNode* lchild;
	CalculateNode* rchild;
};

// 判断运算符
char const chr[] = { '+', '-', '*', '/', '(', ')' , '\0'};
bool isOperationChar(char ch) {
	for (auto i : chr)
		if (ch == i)
			return true;
	return false;
}

// 判断 - 前是否是数字
bool isNegative(const string str, int i) {

	if (str[i] != '-') return false;
	if (--i < 0) return true;

	while (str[i] == ' ') i--;
	if (isdigit(str[i])) return false;
	else return true;
}

/*
	判断符号权重
	约定：< 计算， > 入栈
*/
char judgPower(char ch, char stackTop) {

	if (ch == '\0' && stackTop == '\0')
		return '=';

	// 括号内计算完
	if (stackTop == '(' && ch == ')')
		return '=';

	// （ 永远可以入栈
	if (ch == '(')
		return '>';

	// ） 出栈直到匹配），永远不入栈
	if (ch == ')')
		return '<';

	// 运算符优先级
	map<char, int> powerMap;
	powerMap['\0'] = 0;
	powerMap['+'] = 1;
	powerMap['-'] = 1;
	powerMap['*'] = 2;
	powerMap['/'] = 2;

	// 前面运算符优先级比现在的大，或则同级，就需要按照运输顺序，计算完（建树），再计算自己（入栈）
	// 否则先运算自己
	if (powerMap[ch] > powerMap[stackTop]) return '>';
	else return '<';
}

// 构造表达式树
CalculateNode* InitExpTree(string str) {
	stack<char> operationStack;
	stack<CalculateNode*> calculateNodeStack;

	str += '\0';
	int i = 0;

	operationStack.push('\0');

	while (str[i] != '\0' || !operationStack.empty()) {

		while (str[i] != '\0' && !isdigit(str[i]) && !isOperationChar(str[i])) i++;

		if (!isOperationChar(str[i]) || isNegative(str, i)) {	// 数字部分
			string value;
			value += str[i++];
			while (isdigit(str[i]) || str[i] == '.') value += str[i++];
			CalculateNode* T = new CalculateNode(value, NULL, NULL);
			calculateNodeStack.push(T);
		}
		else {	// 符号部分
			char t = judgPower(str[i], operationStack.top());
			switch (t) {
			case '>': {
				operationStack.push(str[i++]);
				break;
			}
			case '<': {
				string sign = string(1, operationStack.top());
				operationStack.pop();

				CalculateNode* second = calculateNodeStack.top();	// 后操作数
				calculateNodeStack.pop();
				CalculateNode* first = calculateNodeStack.top();		// 前操作数
				calculateNodeStack.pop();

				CalculateNode* T = new CalculateNode(sign, first, second);
				calculateNodeStack.push(T);
				break;
			}
			case '=': {
				operationStack.pop();
				i++;
				break;
			}
			}

		}
	}
	return calculateNodeStack.top();
}

// 字符串转操作数
double operand(string str) {
	int i = 0;
	int x = 0, f = 1;

	if (str[i] == '-') f = -1, i++;
	while (isdigit(str[i])) {
		x = (x << 3) + (x << 1) + str[i] - '0';
		i++;
	}
	if (str[i++] == '.') {
		double p = 0, t = 0.1;
		while (isdigit(str[i])) {
			p += (str[i] - '0') * t;
			t *= 0.1;
			i++;
		}
		// cout << x << ',' << p << endl;
		return f * (x + p);
	}
	else return f * x;

}

// 运算
double operate(string sign, double l, double r) {

	switch (sign[0])
	{
	case '+': {
		return l + r;
	}
	case '-': {
		return l - r;
	}
	case '*': {
		return l * r;
	}
	case '/': {
		return l / r;
	}
	default:
		break;
	}
	return 0;
}

// 计算表达式树
double calculateExpTree(CalculateNode* T) {

	// 操作数左右孩子必为空
	if (!T->ls() && !T->rs()) return operand(T->data());

	double l = 0, r = 0;
	if (T->ls() != NULL) l += calculateExpTree(T->ls());
	if (T->rs() != NULL) r += calculateExpTree(T->rs());

	return operate(T->data(), l, r);
}

// 销毁表达树
void destroyExpTree(CalculateNode* T) {
	stack<CalculateNode*> Node;
	Node.push(T);

	while (!Node.empty()) {
		CalculateNode* node = Node.top();
		Node.pop();

		if (node->ls()) Node.push(node->ls());
		if (node->rs()) Node.push(node->rs());

		delete node;
	}
}

// 计算机函数入口
void calculateSolve() {
	string str;
	while (getline(cin, str)) {
		if (str[0] == '\n') continue;
		CalculateNode* T = InitExpTree(str);
		cout << str << " = " << calculateExpTree(T) << endl;
		destroyExpTree(T);
	}
}

int main() {
	calculateSolve();
	return 0;
}