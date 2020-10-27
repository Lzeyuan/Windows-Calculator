#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>

class BigInter {
public:
    std::string number;
    int length;
    int sign;

    // 构造函数
    BigInter():sign(0), length(0) { this->number = ""; }
    BigInter(const std::string& number);
    BigInter(const std::string& number, const int& sign);

    // util函数
    std::string filterZero(const std::string& number);
    std::string ToString();
    BigInter negative(const BigInter& x);
    int compareMagnitude(const std::string& operator1, const std::string& operator2);

    // 无符号运算
    std::string add(const std::string& operator1, const std::string& operator2);
    std::string subtract(const std::string& operator1, const std::string& operator2);
    std::string multiply(const std::string& operator1, const std::string& operator2);
    std::string divide(const std::string& dividend, const std::string& divisor);

    // 运算符重载
    BigInter operator + (const BigInter& x);
    BigInter operator - (const BigInter& x);
    BigInter operator * (const BigInter& x);
    BigInter operator / (const BigInter& x);
};

/**
 * 初始化大数类 
 * param: 
 * string    字符串数组
 */
BigInter::BigInter(const std::string& number) {
    this->sign = isdigit(number[0]) ? 1 : -1;
    this->number = filterZero(number);
    if (this->number[0] == '0') this->sign = 0;
    this->length = this->number.length();
}

/** 
 * 初始化大数类 
 * param: 
 * string  字符串数组
 * int     大数符号
*/
BigInter::BigInter(const std::string& number, const int& sign) {
    this->number = filterZero(number);
    this->sign = sign;
    this->length = this->number.length();
}

// 过滤无符号数前导0    (filter zero)
std::string BigInter::filterZero(const std::string& number) {
    
    std::string res = "";
    bool isBegin = false;

    // 过滤前导0
    for ( int i = isdigit(number[0]) ? 0 : 1; i <= number.length() - 1; i++) {
        if (!isBegin && number[i] == '0') continue;
        else isBegin = true;

        res += number[i];
    }
    
    return res == "" ? "0": res;
}

// 取反
BigInter BigInter::negative(const BigInter& x) {
    return BigInter(x.number, -x.sign);
}

// 输出大数 (ToString)
std::string BigInter::ToString() {
    switch (this->sign)
    {
    case 1:
        return this->number;
        break;
    case 0:
        return "0";     // 记得要返回 string 而不是 0
        break;
    case -1:
        return "-" + this->number;
        break;
    default:
        break;
    }
    return "ToString Error!\n";
}

// 比较无符号数大小
int BigInter::compareMagnitude(const std::string& operator1, const std::string& operator2) {
    int op1 = operator1.length() - 1;
    int op2 = operator2.length() - 1;

    // 先判断长度
    if (op1 > op2) 
        return 1;
    else if (op1 < op2) 
        return -1; 
    else {

        // 长度一样，从大到小判断每位大小
        for (int i = 0; i <= op1; i++) {
            if (operator1[i] != operator2[i])
                return operator1[i] < operator2[i] ? -1 : 1;
        }
    }

    // 两数相等
    return 0;
}


// 重载加法操作符
BigInter BigInter::operator+(const BigInter& x) {
    // 判断两操作数是否为0
    if (x.sign == 0) return *this;
    if (this->sign == 0) return x;



    // 判断符号是否相同
    if (this->sign == x.sign) {     
        
        // 符号相同
        return BigInter(add(this->number, x.number), x.sign);
    }
    else {      
        
        // 符号不同，相加
        // 判断哪个数大
        int cmp = compareMagnitude(this->number, x.number);

        // 两数相等
        if (cmp == 0) 
            return BigInter("0");

        // 调用大数减法，大数减小数
        std::string result = cmp == 1 ? subtract(this->number, x.number) : subtract(x.number, this->number);

        // 符号跟大数
        return BigInter(result, cmp == 1 ? this->sign : x.sign);
    }
}

// 重载减操作符
BigInter BigInter::operator-(const BigInter& x) {
    // 判断两操作数是否为0
    if (x.sign == 0) return *this;
    if (this->sign == 0) return negative(x);

    // 符号不同，结果符号为被减数
    if (x.sign != this->sign) return BigInter(add(this->number, x.number), this->sign);
    else {
        // 符号相同
        // 判断大小
        int cmp = compareMagnitude(this->number, x.number);
        if (cmp == 0) return BigInter("0");
        std::string result = cmp == 1 ? subtract(this->number, x.number) : subtract(x.number, this->number);

        // 符号被减数大跟被减数，小就和被减数相反
        return BigInter(result, cmp == 1 ? this->sign : -this->sign);
    }
}

// 重载乘法操作符
BigInter BigInter::operator * (const BigInter& x) {
    if (this->sign == 0 || x.sign == 0) return BigInter("0"); 

    // 异号为负
    // 同号，负负得正，或为正
    if (this->sign != x.sign) return BigInter(multiply(this->number, x.number), -1);
    else if (this->sign == x.sign) return BigInter(multiply(this->number, x.number), this->sign * x.sign);

    return BigInter("0");
}

// 重载除法操作符
BigInter BigInter::operator / (const BigInter& x) {
    
    // 除数为0
    if (x.sign == 0) throw "Division by zero condition!";

    // 被除数为 0 或 被除数长度小于除数
    if (this->sign == 0 || this->number.length() < x.number.length()) return BigInter("0");
    else {

        // 被除数长于除数
        std::string result = divide(this->number, x.number);

        // 同号，结果符号为两数相乘
        if (this->sign == x.sign) return BigInter(result, this->sign * x.sign);
        else return BigInter(result, this->sign == 1? -this->sign : this->sign);
    }

    return BigInter("0");
}

// 无符号数加法
std::string BigInter::add(const std::string& operator1, const std::string& operator2) {
    std::string result = "";

    int i = operator1.length() - 1;
    int j = operator2.length() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry > 0) {
        int op1 = i < 0 ? 0 : operator1[i--] - '0';
        int op2 = j < 0 ? 0 : operator2[j--] - '0';

        int add = op1 + op2 + carry;
        carry = add/10;
        result += (char)(add%10 + '0');
    }

    std::reverse(result.begin(), result.end());

    return result;
}

// 无符号数减法
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

        result += (char)(sub%10 + '0');
    }

    std::reverse(result.begin(), result.end());
    result = filterZero(result);

    return result;
}

// 无符号乘法
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
    for (auto &c: ans) {
        c += '0';
    }
    return ans;
}

// 无符号除法
std::string BigInter::divide(const std::string& dividend, const std::string& divisor) {
    
    std::string t_dividend = dividend;
    std::string t_divisor = divisor;
    std::string result;

    // 补位
    int difLen = dividend.length() - divisor.length();
    for (int i = 1; i <= difLen; i++) t_divisor.push_back('0');

    int temp = 0;

    // 循环到除数为空
    while (t_divisor.length() >= divisor.length()) {
        int temp = 0;
        while (compareMagnitude(t_dividend, t_divisor) >= 0) {
            temp++;
            t_dividend = subtract(t_dividend, t_divisor);
        }

        // 除数退位
        t_divisor.pop_back();

        // 加入答案
        result.push_back(temp + '0');
    }

    result = filterZero(result);

    return result;
}

int main()
{
    std::string op1, op2;

    // std::cout << BigInter(input).number << std::endl;

    op1 = "-99999999999999999999999999999999999999999999999", op2 = "-9";
    std::cout << BigInter(op1).ToString() << ", " << BigInter(op2).ToString() << std::endl;
    try {
        std::cout << (BigInter(op1) / BigInter(op2)).ToString() << std::endl;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }
    return 0;
}