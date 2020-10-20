#include "Naturalnumber.h"

/**
     * ��ʼ��������
     * param:
     * string    �ַ�������
     */
BigInter::BigInter(const std::string& number) {
    this->sign = isdigit(number[0]) ? 1 : -1;
    this->number = filterZero(number);
    if (this->number[0] == '0') this->sign = 0;
    this->length = this->number.length();
}

/**
 * ��ʼ��������
 * param:
 * string  �ַ�������
 * int     ��������
*/
BigInter::BigInter(const std::string& number, const int& sign) {
    this->number = filterZero(number);
    this->sign = sign;
    this->length = this->number.length();
}

// �����޷�����ǰ��0    (filter zero)
std::string BigInter::filterZero(const std::string& number) {

    std::string res = "";
    bool isBegin = false;

    // ����ǰ��0
    for (int i = isdigit(number[0]) ? 0 : 1; i <= number.length() - 1; i++) {
        if (!isBegin && number[i] == '0') continue;
        else isBegin = true;

        res += number[i];
    }

    return res == "" ? "0" : res;
}

// ȡ��
BigInter BigInter::negative(const BigInter& x) {
    return BigInter(x.number, -x.sign);
}

// ������� (ToString)
std::string BigInter::ToString() {
    switch (this->sign)
    {
    case 1:
        return this->number;
        break;
    case 0:
        return "0";     // �ǵ�Ҫ���� string ������ 0
        break;
    case -1:
        return "-" + this->number;
        break;
    default:
        break;
    }
    return "ToString Error!\n";
}

// �Ƚ��޷�������С
int BigInter::compareMagnitude(const std::string& operator1, const std::string& operator2) {
    int op1 = operator1.length() - 1;
    int op2 = operator2.length() - 1;

    // ���жϳ���
    if (op1 > op2)
        return 1;
    else if (op1 < op2)
        return -1;
    else {

        // ����һ�����Ӵ�С�ж�ÿλ��С
        for (int i = 0; i <= op1; i++) {
            if (operator1[i] != operator2[i])
                return operator1[i] < operator2[i] ? -1 : 1;
        }
    }

    // �������
    return 0;
}


// ���ؼӷ�������
BigInter BigInter::operator+(const BigInter& x) {
    // �ж����������Ƿ�Ϊ0
    if (x.sign == 0) return *this;
    if (this->sign == 0) return x;



    // �жϷ����Ƿ���ͬ
    if (this->sign == x.sign) {

        // ������ͬ
        return BigInter(add(this->number, x.number), x.sign);
    }
    else {

        // ���Ų�ͬ�����
        // �ж��ĸ�����
        int cmp = compareMagnitude(this->number, x.number);

        // �������
        if (cmp == 0)
            return BigInter("0");

        // ���ô���������������С��
        std::string result = cmp == 1 ? subtract(this->number, x.number) : subtract(x.number, this->number);

        // ���Ÿ�����
        return BigInter(result, cmp == 1 ? this->sign : x.sign);
    }
}

// ���ؼ�������
BigInter BigInter::operator-(const BigInter& x) {
    // �ж����������Ƿ�Ϊ0
    if (x.sign == 0) return *this;
    if (this->sign == 0) return negative(x);

    // ���Ų�ͬ���������Ϊ������
    if (x.sign != this->sign) return BigInter(add(this->number, x.number), this->sign);
    else {
        // ������ͬ
        // �жϴ�С
        int cmp = compareMagnitude(this->number, x.number);
        if (cmp == 0) return BigInter("0");
        std::string result = cmp == 1 ? subtract(this->number, x.number) : subtract(x.number, this->number);

        // ���ű����������������С�ͺͱ������෴
        return BigInter(result, cmp == 1 ? this->sign : -this->sign);
    }
}

// ���س˷�������
BigInter BigInter::operator * (const BigInter& x) {
    if (this->sign == 0 || x.sign == 0) return BigInter("0");

    // ���Ϊ��
    // ͬ�ţ�������������Ϊ��
    if (this->sign != x.sign) return BigInter(multiply(this->number, x.number), -1);
    else if (this->sign == x.sign) return BigInter(multiply(this->number, x.number), this->sign * x.sign);

    return BigInter("0");
}

// ���س���������
BigInter BigInter::operator / (const BigInter& x) {

    // ����Ϊ0
    if (x.sign == 0) throw "Division by zero condition!";

    // ������Ϊ 0 �� ����������С�ڳ���
    if (this->sign == 0 || this->number.length() < x.number.length()) return BigInter("0");
    else {

        // ���������ڳ���
        std::string result = divide(this->number, x.number);

        // ͬ�ţ��������Ϊ�������
        if (this->sign == x.sign) return BigInter(result, this->sign * x.sign);
        else return BigInter(result, this->sign == 1 ? -this->sign : this->sign);
    }

    return BigInter("0");
}

// �޷������ӷ�
std::string BigInter::add(const std::string& operator1, const std::string& operator2) {
    std::string result = "";

    int i = operator1.length() - 1;
    int j = operator2.length() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry > 0) {
        int op1 = i < 0 ? 0 : operator1[i--] - '0';
        int op2 = j < 0 ? 0 : operator2[j--] - '0';

        int add = op1 + op2 + carry;
        carry = add / 10;
        result += (char)(add % 10 + '0');
    }

    std::reverse(result.begin(), result.end());

    return result;
}

// �޷���������
std::string BigInter::subtract(const std::string& big, const std::string& little) {
    std::string result;

    int i = big.length() - 1;
    int j = little.length() - 1;
    int borrow = 0;

    while (i >= 0 || j >= 0) {
        int op1 = i < 0 ? 0 : big[i--] - '0';
        int op2 = j < 0 ? 0 : little[j--] - '0';

        int sub = op1 - op2 - borrow;
        if (sub < 0) {
            sub += 10;
            borrow = 1;
        }

        result += (char)(sub % 10 + '0');
    }

    std::reverse(result.begin(), result.end());
    result = filterZero(result);

    return result;
}

// �޷��ų˷�
std::string BigInter::multiply(const std::string& operator1, const std::string& operator2) {
    int m = operator1.size(), n = operator2.size();
    auto ansArr = std::vector<int>(m + n);
    for (int i = m - 1; i >= 0; i--) {
        int x = operator1.at(i) - '0';
        for (int j = n - 1; j >= 0; j--) {
            int y = operator2.at(j) - '0';
            ansArr[i + j + 1] += x * y;
        }
    }
    for (int i = m + n - 1; i > 0; i--) {
        ansArr[i - 1] += ansArr[i] / 10;
        ansArr[i] %= 10;
    }
    int index = ansArr[0] == 0 ? 1 : 0;
    std::string ans;
    while (index < m + n) {
        ans.push_back(ansArr[index]);
        index++;
    }
    for (auto& c : ans) {
        c += '0';
    }
    return ans;
}

// �޷��ų���
std::string BigInter::divide(const std::string& dividend, const std::string& divisor) {

    std::string t_dividend = dividend;
    std::string t_divisor = divisor;
    std::string result;

    // ��λ
    int difLen = dividend.length() - divisor.length();
    for (int i = 1; i <= difLen; i++) t_divisor.push_back('0');

    int temp = 0;

    // ѭ��������Ϊ��
    while (t_divisor.length() >= divisor.length()) {
        int temp = 0;
        while (compareMagnitude(t_dividend, t_divisor) >= 0) {
            temp++;
            t_dividend = subtract(t_dividend, t_divisor);
        }

        // ������λ
        t_divisor.pop_back();

        // �����
        result.push_back(temp + '0');
    }

    result = filterZero(result);

    return result;
}