using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace UWP_Calcunlator
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {

        private String expression;
        private String input;
        private String operationChar;
        private String result;
        private int countBrackets;

        public MainPage()
        {
            this.InitializeComponent();
            ApplicationView.PreferredLaunchViewSize = new Size(320, 650);
            ApplicationView.PreferredLaunchWindowingMode = ApplicationViewWindowingMode.PreferredLaunchViewSize;
            expression = "";
            input = "";
            operationChar = "";
            result = "";
            countBrackets = 0;
        }


        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;
            string cmd = Convert.ToString(button.Content);

            //inputTextBlock.Text = cmd;

            if (char.IsDigit(cmd[0]) || cmd[0] == '(')
            {
                numberHandle(cmd[0]);
            }
            else
            {
                characterHandle(cmd[0]);
            }

            EnterButton.Focus(FocusState.Programmatic);
        }

        // 处理数字输入事件
        private void numberHandle(char number)
        {
            // 操作符是否为空
            // 不为空则加入算式中
            if (!String.IsNullOrEmpty(operationChar))
            {
                expression += operationChar;
                operationChar = "";
            }

            // 输入栏 == 上次计算的结果数，清空输入栏
            if (String.Equals(input, result))
            {
                result = "";
                input = "";
            }

            // 输入栏 == "0"，清空输入栏
            if (String.Equals(input, "0"))
            {
                input = "";
            }

            if (number == '(')
            {
                input += '(';
                inputTextBlock.Text = input;
                countBrackets++;
                return;
            }

            // 输入栏后添加输入的字符
            input += Convert.ToString(number);

            inputTextBlock.Text = input;
        }

        // 处理计算符输入
        private void characterHandle(char character)
        {
            // 把操作符前的数字加入 expression
            if (String.IsNullOrEmpty(operationChar))
            {
                expression += input;
            }

            switch (character)
            {
                case ')':
                    {
                        if (countBrackets == 0)
                        {
                            return;
                        }

                        if (!String.IsNullOrEmpty(operationChar))
                        {
                            operationChar = "";
                        }

                        expression += ')';
                        countBrackets--;
                        break;
                    }
                case '+':
                    {
                        operationChar = "+";
                        break;
                    }
                case '-':
                    {
                        operationChar = "-";
                        break;
                    }
                case '×':
                    {
                        operationChar = "*";
                        break;
                    }
                case '÷':
                    {
                        operationChar = "/";
                        break;
                    }
                case '=':
                    {
                        operationChar = "=";
                        break;
                    }
            }

            // 补齐括号
            string t = expression;
            for (int i = countBrackets; i > 0; i--)
            {
                t += ")";
            }

            // 计算结果
            Calaulator calaulator = new Calaulator();
            input = calaulator.calculateResult(t);
            result = input;

            // 输出表达式和结果
            expressionTextBlock.Text = expression + operationChar;
            inputTextBlock.Text = input;

            // 清0
            if (character == '=')
            {
                expressionTextBlock.Text = t + operationChar;
                operationChar = "";
                expression = "";
                countBrackets = 0;
            }

        }
    }

    public class CalculateTree
    {
        private String value;
        private CalculateTree lchild;
        private CalculateTree rchild;

        public CalculateTree(String value, CalculateTree lchild = null, CalculateTree rchild = null)
        {
            this.value = value;
            this.lchild = lchild;
            this.rchild = rchild;
        }

        public CalculateTree getRchild() { return this.rchild; }
        public CalculateTree getLchild() { return this.lchild; }

        public String getValue() { return this.value; }

    }

    public class Calaulator
    {
        private CalculateTree T;

        // 构造表达树
        private void initCalculateTree(String expression)
        {
            Stack<char> operationStack = new Stack<char>();
            Stack<CalculateTree> calculateNodeStack = new Stack<CalculateTree>();


            int i = 0;
            expression += '\0';
            operationStack.Push('\0');

            while (i < expression.Length && (expression[i] != '\0' || operationStack.Count != 0))
            {
                // 过滤掉除操作符和数字以外的字符
                spiltChar(expression, ref i);

                // 开始建树
                if (!isOperationChar(expression[i]) || isNegative(expression, i))
                {   // 数字部分
                    String value = "";
                    value += expression[i++];
                    while (Char.IsDigit(expression[i]) || expression[i] == '.') value += expression[i++];
                    CalculateTree T = new CalculateTree(value);
                    calculateNodeStack.Push(T);
                }
                else
                {
                    char t = judgPower(expression[i], operationStack.Peek());
                    switch (t)
                    {
                        case '>':
                            {
                                operationStack.Push(expression[i++]);
                                break;
                            }
                        case '<':
                            {
                                string sign = new string(operationStack.Peek(), 1);
                                operationStack.Pop();

                                CalculateTree second = calculateNodeStack.Peek();   // 后操作数
                                calculateNodeStack.Pop();
                                CalculateTree first = calculateNodeStack.Peek();        // 前操作数
                                calculateNodeStack.Pop();

                                CalculateTree T = new CalculateTree(sign, first, second);
                                calculateNodeStack.Push(T);
                                break;
                            }
                        case '=':
                            {
                                operationStack.Pop();
                                i++;
                                break;
                            }
                    }
                }
            }

            this.T = calculateNodeStack.Peek();
        }

        // 计算表达树
        private String Calculate(CalculateTree T)
        {
            // 操作数左右孩子必为空
            if (T.getLchild() == null && T.getRchild() == null) return T.getValue();

            String l = "", r = "";
            if (T.getLchild() != null) l += Calculate(T.getLchild());
            if (T.getRchild() != null) r += Calculate(T.getRchild());

            return operate(T.getValue(), l, r);
        }

        // 类入口
        public String calculateResult(String expression)
        {
            initCalculateTree(expression);
            return Calculate(this.T);
        }

        // 运算
        private String operate(string sign, String l, String r)
        {


            BigInteger a = BigInteger.Parse(l);
            BigInteger b = BigInteger.Parse(r);
            switch (sign[0])
            {
                case '+':
                    {
                        return (a + b).ToString();
                    }
                case '-':
                    {
                        return (a - b).ToString();
                    }
                case '*':
                    {
                        return (a * b).ToString();
                    }
                case '/':
                    {
                        return (a / b).ToString();
                    }
                default:
                    break;
            }
            return "";
        }

        // 判断运算符
        private char[] opr = { '+', '-', '*', '/', '(', ')', '\0' };
        private bool isOperationChar(char ch)
        {
            foreach (var i in opr)
                if (ch == i)
                    return true;
            return false;
        }

        // 判断 - 前是否是数字
        private bool isNegative(string str, int i)
        {
            // 确认是否为"-"
            if (str[i] != '-') return false;

            // 负号是第一个输入数，必为负号
            if (--i < 0) return true;

            // 如果"-"接数字，则为负号，否则为减号
            if (Char.IsDigit(str[i])) return false;
            else return true;
        }

        // 过滤掉除操作符和数字以外的字符
        private void spiltChar(String expression, ref int i)
        {
            while (!Char.IsDigit(expression[i]) && !isOperationChar(expression[i])) i++;
        }

        // 判断运算符优先级
        private char judgPower(char ch, char stackTop)
        {

            if (ch == '\0' && stackTop == '\0')
                return '=';

            // 括号内计算完
            if (stackTop == '(' && ch == ')')
                return '=';

            // （ 永远可以入栈
            // 前面是 ( 也永远可以进栈
            if (ch == '(' || stackTop == '(')
                return '>';

            // ） 出栈直到匹配），永远不入栈
            if (ch == ')')
                return '<';

            // 运算符优先级
            IDictionary<char, int> powerMap = new Dictionary<char, int>();
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
    }
}