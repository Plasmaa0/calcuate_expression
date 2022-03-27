#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

const int INVALID_NUMBER = -1;

typedef enum NodeType
{
    NUMBER,
    VARIABLE,
    OPERATOR,
    INVALID
} NodeType;

typedef struct TreeNode
{
    NodeType type;
    char op;
    char var;
    int number;
    TreeNode *left;
    TreeNode *right;
} TreeNode;

typedef struct VariablesDictionary
{
    int values['z' - 'a'];
} VariablesDictionary;

VariablesDictionary *createDict()
{
    VariablesDictionary *dict = new VariablesDictionary;
    for (int i = 0; i < 'z' - 'a'; i++)
    {
        dict->values[i] = INVALID_NUMBER;
    }
    return dict;
}

void setVariable(char name, int value, VariablesDictionary *dict)
{
    dict->values[name - 'a'] = value;
}

int getVariable(char name, VariablesDictionary *dict, bool &success)
{
    int val = dict->values[name - 'a'];
    if (val == INVALID_NUMBER)
    {
        cout << "VARIABLE " << name << " NOT EXIST" << endl;
        success = false;
    }
    return val;
}

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
    root->var = '#';
    root->type = INVALID;
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
        if ('0' <= str[a] && str[a] <= '9') //number
        {
            root->number = str[a] - '0';
            root->type = NUMBER;
        }
        else if ('a' <= str[a] && str[a] <= 'z')
        {
            root->var = str[a];
            root->type = VARIABLE;
        }
        else
        {
            root->type = INVALID;
        }

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

    root->type = OPERATOR;
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
        if (root->type == OPERATOR)
        {
            printf("%c\n", root->op);
        }
        if (root->type == VARIABLE)
        {
            printf("%c\n", root->var);
        }
        else if (root->type == NUMBER)
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

double evaluate(TreeNode *root, VariablesDictionary *dict, bool &success)
{
    if (root == NULL)
    {
        success = false;
        return 10e10;
    }

    if (root->type == NUMBER) //number
    {
        return root->number;
    }
    if (root->type == VARIABLE) //number
    {
        return getVariable(root->var, dict, success);
    }

    return solve(root->op, evaluate(root->left, dict, success), evaluate(root->right, dict, success), success);
}

void pre_order(const TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    if (root->type == OPERATOR)
    {
        printf("%c ", root->op);
    }
    if (root->type == VARIABLE)
    {
        printf("%c ", root->var);
    }
    else if (root->type == NUMBER)
    {
        printf("%d ", root->number);
    }
    pre_order(root->left);
    pre_order(root->right);
}

void in_order(const TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    in_order(root->left);
    if (root->type == OPERATOR)
    {
        printf("%c ", root->op);
    }
    if (root->type == VARIABLE)
    {
        printf("%c ", root->var);
    }
    else if (root->type == NUMBER)
    {
        printf("%d ", root->number);
    }
    in_order(root->right);
}

void post_order(const TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    post_order(root->left);
    post_order(root->right);
    if (root->type == OPERATOR)
    {
        printf("%c ", root->op);
    }
    if (root->type == VARIABLE)
    {
        printf("%c ", root->var);
    }
    else if (root->type == NUMBER)
    {
        printf("%d ", root->number);
    }
}

void info(TreeNode *root)
{
    cout << "TREE:" << endl;
    print(root, 0);
    cout << "post order: ";
    post_order(root);
    cout << endl;
    cout << "in order: ";
    in_order(root);
    cout << endl;
    cout << "pre order: ";
    pre_order(root);
    cout << endl;
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

    // cout << "str: ";
    // cin >> str;
    TreeNode *root = createTree(str);

    VariablesDictionary *dict = createDict();
    setVariable('a', 10, dict);

    info(root);

    bool success = true;
    double res = evaluate(root, dict, success);
    if (success)
        cout << "res = " << setprecision(15) << res << endl;
    else
        cout << "ERROR" << endl;
    return 0;
}