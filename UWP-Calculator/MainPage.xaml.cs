using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace winuiCalculator
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button button = (Button)sender;

            String cmd = (String)(button.Content);

            Calaulator calaulator = new Calaulator();

            if (cmd == "1")
                resultTextBlock.Text = calaulator.calculateResult("112-0");
            else
                resultTextBlock.Text = calaulator.calculateResult("112+113");
        }

        private void Grid_KeyUp(object sender, KeyRoutedEventArgs e)
        {

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
            switch (sign[0])
            {
                case '+':
                    {
                        return add(l, r);
                    }
                case '-':
                    {
                        return sub(l, r);
                    }
                //case '*':
                //    {
                //        return l * r;
                //    }
                //case '/':
                //    {
                //        return l / r;
                //    }
                default:
                    break;
            }
            return "";
        }

        // 反转数字字符串
        private String stringReverse(String str)
        {
            String res = "";
            int i = str.Length - 1;
            bool isBegin = false;

            if (!Char.IsDigit(str[str.Length - 1])) i--;

            for (; i >= 0; i--)
            {
                if (!isBegin && str[i] == '0') continue;
                else isBegin = true;

                res += str[i];
            }

            return res;
        }

        // 加法
        private String add(String operation1, String operation2)
        {
            int i = operation1.Length - 1, j = operation2.Length - 1;
            int addTemp = 0;
            String ans = "";
            
            while (i >= 0 || j >= 0 || addTemp > 0)
            {
                int op1 = i < 0 ? 0 : operation1[i--] - '0';
                int op2 = j < 0 ? 0 : operation2[j--] - '0';

                int result = op1 + op2 + addTemp;
                ans += (char)(result%10 + '0');

                addTemp = result / 10;
            }

            return stringReverse(ans);
        }

        // 二目交换函数
        private void Swap<T>(ref T a, ref T b)
        {
            T t = a;
            a = b;
            b = t;
        }

        // 判断数字字符串大小
        // 注意a,b顺序
        // 判断a >= b
        private bool cmpMathString(String a, String b)
        {
            if (a.Length < b.Length)
            {
                return false;
            } 
            else if (a.Length == b.Length)
            {
                int i = 0;
                // 从大数开始比较
                while (i < a.Length - 1 && a[i] == b[i]) i++;
                if (a[i] < b[i]) return false;
            }

            return true;
        }

        // 减法
        private String sub(String operation1, String operation2)
        {
            char flag = '\0';

            // 判断大小
            if (!cmpMathString(operation1, operation2))
            {
                Swap(ref operation1, ref operation2);
                flag = '-';
            } 

            int i = operation1.Length - 1;
            int j = operation2.Length - 1;

            int temp = 0;
            int result = 0;
            String ans = "";


            while (i >= 0)
            {
                int op1 = operation1[i--] - '0';
                int op2 = j < 0 ? 0 : operation2[j--] - '0';

                // 判断是否借位
                result = op1 - op2 - temp;
                if (result < 0) 
                {
                    result += 10;
                    temp = 1;
                } 
                else
                {
                    temp = 0;
                }

                ans += (char)(result % 10 + '0');
            }

            ans += flag;

            return stringReverse(ans);
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
            if (ch == '(')
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
