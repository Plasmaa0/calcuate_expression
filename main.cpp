#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

typedef struct TreeNode
{
    bool isOp;
    char op;
    int number;
    TreeNode *left;
    TreeNode *right;
} TreeNode;

int priority(char ch)
{
    switch (ch)
    {
    case '-':
    case '+':
        return 1;
        break;
    case '*':
    case '/':
        return 2;
        break;
    default:
        return 1000;
        break;
    }
}

TreeNode *create_node()
{
    TreeNode *root = new TreeNode;
    root->left = NULL;
    root->right = NULL;
    root->number = -42;
    root->op = '#';
    root->isOp = false;
    return root;
}

void strip(char *str, int &a, int &b)
{
    for (int i = a; i < b; i++)
    {
        if (str[i] == '(')
        {
            a++;
        }
        else
        {
            break;
        }
    }
    for (int i = b - 1; i >= a; i--)
    {
        if (str[i] == ')')
        {
            b--;
        }
        else
        {
            break;
        }
    }
}

TreeNode *createTree(char *str, int a, int b)
{
    strip(str, a, b);

    TreeNode *root = create_node();
    if (b - a == 1)
    {
        root->number = str[a] - '0';
        root->isOp = false;
        return root;
    }

    int min_priority = 2000;
    int current_priority;
    int division_index = -1;
    int bracket_level = 0;
    for (int i = a; i < b; i++)
    {
        // cout << str[i];
        if (str[i] == '(')
        {
            bracket_level++;
        }
        else if (str[i] == ')')
        {
            bracket_level--;
        }
        else
        {
            current_priority = priority(str[i]) + bracket_level * 10;
            if (current_priority <= min_priority)
            {
                min_priority = current_priority;
                division_index = i;
            }
        }
    }

    if (division_index == -1)
    {
        // printf("end\n");
        return NULL;
    }

    root->isOp = true;
    root->op = str[division_index];

    // printf("\nchar = %c\nindex = %d\npriority = %d\n\n", str[division_index], division_index, min_priority);

    // printf("Left\n");
    root->left = createTree(str, a, division_index);
    // printf("Right\n");
    root->right = createTree(str, division_index + 1, b);

    return root;
}

TreeNode *createTree(char *str)
{
    return createTree(str, 0, strlen(str));
}

void tabulate(int n)
{
    for (int i = 0; i < n; i++)
        printf("\t");
}

void print(const TreeNode *root, int depth)
{
    if (root != NULL)
    {
        depth++;
        print(root->right, depth);
        tabulate(depth);
        if (root->isOp)
        {
            printf("%c\n", root->op);
        }
        else
        {
            printf("%d\n", root->number);
        }
        print(root->left, depth);
    }
}

double solve(char op, double a, double b, bool &success)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (abs(b) < 0.000001)
        {
            cout << "ZERO DIVISION" << endl;
            success = false;
            return 10e10;
        }

        return a / b;
        break;

    default:
        break;
    }
    return 10e10;
}

double evaluate(TreeNode *root, bool &success)
{
    if (root == NULL)
    {
        success = false;
        return 10e10;
    }

    if (!root->isOp) //number
    {
        return root->number;
    }

    return solve(root->op, evaluate(root->left, success), evaluate(root->right, success), success);
}

int main()
{
    char str[] = "(9*(6-1))/(4+3/(3+7))"; // 10.465116279069768

    /*
                                   /  
                     *                      +
                9        -              4       /
                      6     1                3     +
                                                  3   7

    */

    cout << "str: ";
    cin >> str;
    TreeNode *root = createTree(str);
    print(root, 0);

    bool success = true;
    double res = evaluate(root, success);
    if (success)
        cout << "res = " << setprecision(15) << res << endl;
    else
        cout << "ERROR" << endl;
    return 0;
}