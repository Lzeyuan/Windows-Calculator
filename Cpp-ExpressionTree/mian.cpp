#include <iostream>
#include <stack>
#include <string>
#include <map>

#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// ���ʽ���ṹ
class CalculateNode {
public:
	// ���캯��
	CalculateNode() {
		this->value = "";
		this->lchild = NULL;
		this->rchild = NULL;
	}
	CalculateNode(string value, CalculateNode* l, CalculateNode* r) :
		value(value), lchild(l), rchild(r) {}

	// ����
	CalculateNode* ls() { return this->lchild; }

	// �Һ���
	CalculateNode* rs() { return this->rchild; }

	// ��ʾֵ
	string data() { return value; }

private:
	string value;
	CalculateNode* lchild;
	CalculateNode* rchild;
};

// �ж������
char const chr[] = { '+', '-', '*', '/', '(', ')' , '\0'};
bool isOperationChar(char ch) {
	for (auto i : chr)
		if (ch == i)
			return true;
	return false;
}

// �ж� - ǰ�Ƿ�������
bool isNegative(const string str, int i) {

	if (str[i] != '-') return false;
	if (--i < 0) return true;

	while (str[i] == ' ') i--;
	if (isdigit(str[i])) return false;
	else return true;
}

/*
	�жϷ���Ȩ��
	Լ����< ���㣬 > ��ջ
*/
char judgPower(char ch, char stackTop) {

	if (ch == '\0' && stackTop == '\0')
		return '=';

	// �����ڼ�����
	if (stackTop == '(' && ch == ')')
		return '=';

	// �� ��Զ������ջ
	if (ch == '(')
		return '>';

	// �� ��ջֱ��ƥ�䣩����Զ����ջ
	if (ch == ')')
		return '<';

	// ��������ȼ�
	map<char, int> powerMap;
	powerMap['\0'] = 0;
	powerMap['+'] = 1;
	powerMap['-'] = 1;
	powerMap['*'] = 2;
	powerMap['/'] = 2;

	// ǰ����������ȼ������ڵĴ󣬻���ͬ��������Ҫ��������˳�򣬼����꣨���������ټ����Լ�����ջ��
	// �����������Լ�
	if (powerMap[ch] > powerMap[stackTop]) return '>';
	else return '<';
}

// ������ʽ��
CalculateNode* InitExpTree(string str) {
	stack<char> operationStack;
	stack<CalculateNode*> calculateNodeStack;

	str += '\0';
	int i = 0;

	operationStack.push('\0');

	while (str[i] != '\0' || !operationStack.empty()) {

		while (str[i] != '\0' && !isdigit(str[i]) && !isOperationChar(str[i])) i++;

		if (!isOperationChar(str[i]) || isNegative(str, i)) {	// ���ֲ���
			string value;
			value += str[i++];
			while (isdigit(str[i]) || str[i] == '.') value += str[i++];
			CalculateNode* T = new CalculateNode(value, NULL, NULL);
			calculateNodeStack.push(T);
		}
		else {	// ���Ų���
			char t = judgPower(str[i], operationStack.top());
			switch (t) {
			case '>': {
				operationStack.push(str[i++]);
				break;
			}
			case '<': {
				string sign = string(1, operationStack.top());
				operationStack.pop();

				CalculateNode* second = calculateNodeStack.top();	// �������
				calculateNodeStack.pop();
				CalculateNode* first = calculateNodeStack.top();		// ǰ������
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

// �ַ���ת������
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

// ����
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

// ������ʽ��
double calculateExpTree(CalculateNode* T) {

	// ���������Һ��ӱ�Ϊ��
	if (!T->ls() && !T->rs()) return operand(T->data());

	double l = 0, r = 0;
	if (T->ls() != NULL) l += calculateExpTree(T->ls());
	if (T->rs() != NULL) r += calculateExpTree(T->rs());

	return operate(T->data(), l, r);
}

// ���ٱ����
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

// ������������
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