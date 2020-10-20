#pragma once
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
    BigInter() :sign(0), length(0) { this->number = ""; }
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