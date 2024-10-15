#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <climits>
#include <fstream>
#include <algorithm>


using namespace std;
//���������� ����������
double MAX = std::numeric_limits<double>::max();
double MIN = std::numeric_limits<double>::min();
double mean = 0, stddev = 1;
static random_device rd;
static mt19937 gen(rd());
normal_distribution<double> dist(mean, stddev);
uniform_real_distribution<double> dis(1.0, 100.0);
int unar_oper = 6, bin_oper = 6, trin_oper = 1, num_var = 8, counter = -1, max_nodes = 100;
// 1000sin, 1001cos, 1002exp, 1003ln, 1004 1/x, 1005-�����, 2000log, 2001+, 2002-, 2003*, 2004/, 2005a^x, 3000if..then..else
// 0x,
char type[3]{ 'o', 'v', 'c' };

double generate_normal() {return dist(gen);}

typedef class Node
{
public:
    char type;
    int operation;
    int variable;
    double constant;
    Node* left;
    Node* mid;
    Node* right;

    Node(char type, int number)
    {
        this->type = type;
        if(type == 'o')
        {
            this->operation = number;
            this->variable = NULL;
        }
        if(type == 'v')
        {
            this->variable = number;
            this->operation = NULL;
        }
        this->constant = NULL;
        this->left = NULL;
        this->mid = NULL;
        this->right = NULL;
    }
    Node(char type, double constant)
    {
        this->type = type;
        this->operation = NULL;
        this->variable = NULL;
        this->constant = constant;
        this->left = NULL;
        this->mid = NULL;
        this->right = NULL;
    }
} Node;

class Tree
{
public:
    Node* root;
    int num_nodes;
    Tree()
    {
        this->root = NULL;
        this->num_nodes = 0;
    }

    ~Tree();

    void ClearTree(Node* node);

    void CloneTree(Node* origin_root);

    int CountVar();

    void UpNumNodes();

    void UpNumNodes(Node* cur_el);

    int CountDepth(Node* cur_el);

    int AddNode(char type, int number, double value, int depth, int* flag, int d);

    int Add_Node_mut(char type, int number, double value, int depth, int* flag, int d, Node*& cur_el);

    void AddNode(char type, int number, double value, int depth, int* flag, int d, Node*& cur_el);

    // �������� ������ � ������� �� ������� �����-������-������
    void PrintTree();

    void PrintTree(Node* cur_el);

    // ������� �� ����, ��������� ��������� ������ ���� ������ 0 � �� ������ ���� ����� �������, �������� ��������� ���� > 0
    double evaluateExpression(double* x);

    double evaluateExpression(double* x, Node* cur_el);

    string printExpression();

    string printExpression(Node* cur_el);

    double error(int num_obs, double** x, double* y);

    double count_fitness(double error, double e, double n, double v, double first_el);

    void Growth(int switch_init, int depth, Node*& cur_el, int target_nodes, int d, bool notmut);

    void PointMut(Node* cur_el, double p);

    void PartMut(int switch_init, Node*& cur_el, int depth, int mut_node, int* current_node, int* d, int* flag);

    void StandCross(Node* cur_el, int splitter, Node*& cut_tree, bool cut);
};

void copying(Node* origin_tree, Node*& dupl_tree)
{
    if(origin_tree==NULL){return;}
    if (origin_tree->type == 'o')
    {
        dupl_tree = new Node(origin_tree->type, origin_tree->operation);
    }
    else if (origin_tree->type == 'v')
    {
        dupl_tree = new Node(origin_tree->type, origin_tree->variable);
    }
    else if (origin_tree->type == 'c')
    {
        dupl_tree = new Node(origin_tree->type, origin_tree->constant);
    }
    copying(origin_tree->left, dupl_tree->left);
    copying(origin_tree->right, dupl_tree->right);
    copying(origin_tree->mid, dupl_tree->mid);
}

Tree::~Tree()
{
    ClearTree(root);
}

void Tree::ClearTree(Node* node)
{
    if (node != nullptr)
    {
        ClearTree(node->left);
        ClearTree(node->mid);
        ClearTree(node->right);
        delete node;
        //num_nodes -= 1;
        if (node == root)//�� �������, ��� ��� ��������� �������� ���� ���
            root = nullptr;
    }
}

//������������ ������ ��� ������ �����������!!
void Tree::CloneTree(Node* origin_root)
{
    if(this->root)
    {
        ClearTree(this->root);
    }
    if (origin_root->type == 'o')
    {
        this->root = new Node(origin_root->type, origin_root->operation);
    }
    else if (origin_root->type == 'v')
    {
        this->root = new Node(origin_root->type, origin_root->variable);
    }
    else if (origin_root->type == 'c')
    {
        this->root = new Node(origin_root->type, origin_root->constant);
    }
    copying(origin_root->left, this->root->left);
    copying(origin_root->right, this->root->right);
    copying(origin_root->mid, this->root->mid);
}

//�� ������� ������ ���
int Tree::CountVar()
{
    int current_num_var;
    string expression = printExpression();
    if (expression.find("x00") != std::string::npos)
        current_num_var++;
    if (expression.find("x01") != std::string::npos)
        current_num_var++;
    if (expression.find("x02") != std::string::npos)
        current_num_var++;
    if (expression.find("x03") != std::string::npos)
        current_num_var++;
    if (expression.find("x04") != std::string::npos)
        current_num_var++;
    if (expression.find("x05") != std::string::npos)
        current_num_var++;
    if (expression.find("x06") != std::string::npos)
        current_num_var++;
    if (expression.find("x07") != std::string::npos)
        current_num_var++;
    if (expression.find("x08") != std::string::npos)
        current_num_var++;
    if (expression.find("x09") != std::string::npos)
        current_num_var++;
    if (expression.find("x10") != std::string::npos)
        current_num_var++;
    if (expression.find("x11") != std::string::npos)
        current_num_var++;
    if (expression.find("x12") != std::string::npos)
        current_num_var++;
    if (expression.find("x13") != std::string::npos)
        current_num_var++;
    return current_num_var;
}

void Tree::UpNumNodes()
{
    num_nodes = 0;
    UpNumNodes(root);
}

void Tree::UpNumNodes(Node* cur_el)
{
    if(cur_el==NULL){return;}
    num_nodes++;
    UpNumNodes(cur_el->left);
    UpNumNodes(cur_el->right);
    UpNumNodes(cur_el->mid);
}

int Tree::CountDepth(Node* cur_el)
{
    if (cur_el==NULL) return 0;
    int leftDepth = CountDepth(cur_el->left);
    int rightDepth = CountDepth(cur_el->right);
    int middleDepth = CountDepth(cur_el->mid);
    int maxDepth = max(leftDepth, rightDepth);
    return max(maxDepth, middleDepth) + 1;
}

int Tree::AddNode(char type, int number, double value, int depth, int* flag, int d)
{
    AddNode(type, number, value, depth, flag, d, root);
    if (*flag == 0)
        return 0;
    if (*flag == 1)
    {
        return 1;
    }
}

int Tree::Add_Node_mut(char type, int number, double value, int depth, int* flag, int d, Node*& cur_el)
{
    AddNode(type, number, value, depth, flag, d, cur_el);
    if (*flag == 0)
        return 0;
    if (*flag == 1)
    {
        return 1;
    }
}

void Tree::AddNode(char type, int number, double value, int depth, int* flag, int d, Node*& cur_el)
{
    // ������� ������� � ������, ����� � ������� ��������� ������ ���������� ��� ���������
    int randcase;
    if(cur_el == NULL)
    {
        if(d<depth)
        {
            if (type=='o'||type=='v')
            {
                cur_el = new Node(type, number);
                *flag = 1;
                return;
            }
            else if(type=='c')
            {
                cur_el = new Node(type, value);
                *flag = 1;
                return;
            }
        }
        else if(d==depth)
        {
            if (type=='v')
            {
                cur_el = new Node(type, number);
                *flag = 1;
                return;
            }
            else if(type=='c')
            {
                cur_el = new Node(type, value);
                *flag = 1;
                return;
            }
            else if(type=='o'){return;}
        }
        else if(d>depth){return;}
    }
    // �������, � ����������� �� ������� ������������ ������� �������� ����� ����� ���� � ����
    else if(cur_el->type=='o')
    {
        if(cur_el->operation < 2000)
        {
            AddNode(type, number, value, depth, flag, d+1, cur_el->left);
            if(*flag==1){return;}
        }
        else if(cur_el->operation >= 2000 && cur_el->operation < 3000)
        {
            randcase = rand() % 2;
            if (randcase == 0)
            {
                AddNode(type, number, value, depth, flag, d+1, cur_el->left);
                if(*flag==1){return;}
                AddNode(type, number, value, depth, flag, d+1, cur_el->right);
                if(*flag==1){return;}
            }
            else
            {
                AddNode(type, number, value, depth, flag, d+1, cur_el->right);
                if(*flag==1){return;}
                AddNode(type, number, value, depth, flag, d+1, cur_el->left);
                if(*flag==1){return;}
            }
        }
        else if(cur_el->operation >= 3000)
        {
            AddNode(type, number, value, depth, flag, d+1, cur_el->left);
            if(*flag==1){return;}
            AddNode(type, number, value, depth, flag, d+1, cur_el->right);
            if(*flag==1){return;}
            AddNode(type, number, value, depth, flag, d+1, cur_el->mid);
            if(*flag==1){return;}
        }
    }
    else if(cur_el->type=='v'||cur_el->type=='c'){return;}
}

void Tree::PrintTree()
{
    PrintTree(root);
}

 void Tree::PrintTree(Node* cur_el)
{
    if(cur_el==NULL){return;}
    PrintTree(cur_el->left);

    if(cur_el->type=='o')
        cout << cur_el->type<< " " << cur_el->operation << endl;
    else if(cur_el->type=='v')
        cout << cur_el->type<< " " << cur_el->variable << endl;
    else if(cur_el->type=='c')
        cout << cur_el->type<< " " << cur_el->constant << endl;
    PrintTree(cur_el->mid);
    PrintTree(cur_el->right);
}

// ������� �� ����, ��������� ��������� ������ ���� ������ 0 � �� ������ ���� ����� �������, �������� ��������� ���� > 0
double Tree::evaluateExpression(double* x)
{
    return evaluateExpression(x, root);
}

double Tree::evaluateExpression(double* x, Node* cur_el)
{
    if (cur_el == nullptr) {return 0.0;}
    if (cur_el->type == 'c') {return cur_el->constant;}
    else if (cur_el->type == 'v')
    {
        return x[cur_el->variable];
    }
    else if (cur_el->operation < 2000)
    {
        double ev_expression = evaluateExpression(x, cur_el->left);
        //if (isnan(ev_expression))
        if (isinf(ev_expression)){ev_expression = MAX;}
        switch(cur_el->operation)
        {
            case 1000:
            {
                return sin(ev_expression);
            }
            case 1001:
            {
                return cos(ev_expression);
            }
            case 1002:
            {
                return exp(ev_expression);
            }
            case 1003:
            {
                if(ev_expression <= 0){ev_expression = MIN;}
                return log(ev_expression);
            }
            case 1004:
            {
                if(ev_expression == 0){ev_expression = MIN;}
                return 1./ev_expression;
            }
            case 1005:
            {
                return -ev_expression;
            }
        }
    }
    else if (cur_el->operation < 3000)
    {
        double left_value = evaluateExpression(x, cur_el->left);
        double right_value = evaluateExpression(x, cur_el->right);
        if (isinf(left_value)){left_value = MAX;}
        if (isinf(right_value)){right_value = MAX;}
        switch (cur_el->operation)
        {
            case 2000:// ����� ���������
                {
                    if(left_value <= 0){left_value = MIN;}
                    else if(left_value == 1){left_value = 0.9999999999999999;}//��� ��� ��������
                    if(right_value <= 0){right_value = MIN;}
                    return log(right_value)/log(left_value);
                }
            case 2001:
                return left_value +  right_value;
            case 2002:
                return left_value - right_value;
            case 2003:
                return left_value * right_value;
            case 2004:
                {
                    if(right_value == 0){right_value = MIN;}
                    return left_value / right_value;
                }
            case 2005:
                {
                    if(left_value < 0){right_value = round(right_value);}
                    if(left_value == 0 && right_value < 0){return 0.0;}
                    return pow(left_value, right_value);
                }
        }
    }
    else if (cur_el->operation >= 3000)
    {
        double left_value = evaluateExpression(x, cur_el->left);
        double mid_value = evaluateExpression(x, cur_el->mid);
        double right_value = evaluateExpression(x, cur_el->right);
        if (isinf(left_value)){left_value = MAX;}
        if (isinf(right_value)){right_value = MAX;}
        if (isinf(mid_value)){mid_value = MAX;}
        switch (cur_el->operation)
        {
            case 3000:
                return left_value +  right_value;//� �� ���� ��� ��� ����������...
        }
    }

    return 0.0;
}

string Tree::printExpression()
{
    return printExpression(root);
}

string Tree::printExpression(Node* cur_el)
{
    string check;
    if (cur_el == nullptr) {return "";}
    if (cur_el->type == 'c')
    {
        if(cur_el->constant<0)
        {
            return "(" + to_string(cur_el->constant) + ")";
        }
        else{return to_string(cur_el->constant);}
    }
    else if (cur_el->type == 'v')
    {
        check = to_string(cur_el->variable);
        if(check.size() == 1){return "x0" + check;}
        else {return "x" + check;}
    }
    else if (cur_el->operation < 2000)
    {
        check = printExpression(cur_el->left);
        switch(cur_el->operation)
        {
            case 1000:
            {
                if (check.find("(")==0)
                    return "sin" + check;
                else
                {
                    return "sin(" + check + ")";
                }
            }
            case 1001:
            {
                if (check.find("(")==0)
                    return "cos" + check;
                else
                {
                    return "cos(" + check + ")";
                }
            }
            case 1002://���� ��������� ���� ������ ������, �� ����� �������������
            {
                if (check.find("(")==0)
                    return "exp" + check;
                else
                {
                    return "exp(" + check + ")";
                }
            }
            case 1003:
            {
                return "ln(" + check + ")";
            }
            case 1004:
            {
                if (check.find("(")==0)
                    return "1/" + check;
                else return "1/(" + check + ")";
            }
            case 1005:// ��� ��� ���� ���� ��������
            {
                return "(-" + check + ")";
            }
        }
    }
     else if (cur_el->operation >=2000 && cur_el->operation < 3000)
    {
        string left_value = printExpression(cur_el->left);
        string right_value = printExpression(cur_el->right);
        switch (cur_el->operation)
        {
            case 2000:
                return "log(" + left_value + ", " + right_value + ")";
            case 2001:
                return "(" + left_value + "+" + right_value + ")";
            case 2002:
                return "(" + left_value + "-" + right_value + ")";
            case 2003:
                return "(" + left_value + "*" + right_value + ")";
            case 2004:
                return "(" + left_value + "/" + right_value + ")";
            case 2005:
                return "(" + left_value + ")^" + "(" + right_value + ")";
        }
    }
    else if (cur_el->operation >= 3000)
    {
        string left_value = printExpression(cur_el->left);
        string mid_value = printExpression(cur_el->mid);
        string right_value = printExpression(cur_el->right);
        switch (cur_el->operation)
        {
            case 3000:
                return "if" + left_value + "then" + mid_value + "else" + right_value;
        }
    }

    return "";
}

double Tree::error(int num_obs, double** x, double* y)
{
    int obs;
    double res=0, check;
    for(obs = 0; obs < num_obs; obs++)
    {
        check = evaluateExpression(x[obs]);
        //if (isnan(check)||check == inf)
            //check = 0;
        res+=(y[obs]-check)*(y[obs]-check);
    }
    //cout << "see" << endl;
    return res;
}

double Tree::count_fitness(double error, double e, double n, double v, double first_el)
{
    UpNumNodes();
    int current_num_var = CountVar();
    double fitness = 1./(1+v*first_el+e*error+n*double(num_nodes)/double(max_nodes)-v*double(current_num_var)/double(num_var));
    if(isinf(fitness)){fitness = MAX;}
    return fitness;
}

void Tree::Growth(int switch_init, int depth, Node*& cur_el, int target_nodes = 0, int d = 0, bool notmut = true)
{
    int success, random_opervar, random_type, random_oper, flag = 0, added_nodes = 1;
    double random_const;
    if(switch_init == 0)
    {
        success = 1;
        while (success == 1)
        {
            flag = 0;
            random_oper = rand() % 2;//if..then..else
            if(random_oper == 0)
            {
                random_opervar = (rand() % unar_oper)+1000;
                added_nodes+=1;
            }
            else if(random_oper == 1)
            {
                random_opervar = (rand() % bin_oper)+2000;
                added_nodes+=2;
            }
            if (notmut)
                success = AddNode(type[0], random_opervar, 0, depth-1, &flag, d);
            else
                success = Add_Node_mut(type[0], random_opervar, 0, depth-1, &flag, d, cur_el);
            if (added_nodes >= target_nodes && notmut == false)
            {
                success = 0;
            }
        }
        success = 1;
        while (success == 1)
        {
            flag = 0;
            random_type = 1+rand() % 2;
            if(random_type == 1)
            {
                random_opervar = rand() % num_var;
                if (notmut)
                    success = AddNode(type[random_type], random_opervar, 0, depth-1, &flag, d);
                else
                    success = Add_Node_mut(type[random_type], random_opervar, 0, depth-1, &flag, d, cur_el);
            }
            else if(random_type == 2)
            {
                random_const = (double) rand() / (double)(RAND_MAX);
                if(random_const < 0.05)
                {
                    if(rand()%2==0){random_const = M_PI;}
                    else {random_const = M_E;}
                }
                else{random_const = generate_normal();}
                if (notmut)
                    success = AddNode(type[random_type], 0, random_const, depth-1, &flag, d);
                else
                    success = Add_Node_mut(type[random_type], 0, random_const, depth-1, &flag, d, cur_el);
            }
        }
    }
    else if(switch_init == 1)
    {
        success = 1;
        while (success == 1)
        {
            flag = 0;
            random_const = (double) rand() / (double)(RAND_MAX);
            if(random_const > 0.1)
            {
                random_oper = rand() % 2;//if..then..else
                if(random_oper == 0)
                {
                    random_opervar = (rand() % unar_oper)+1000;
                    added_nodes+=1;
                }
                else if(random_oper == 1)
                {
                    random_opervar = (rand() % bin_oper)+2000;
                    added_nodes+=2;
                }
                if (notmut)
                    success = AddNode(type[0], random_opervar, 0, depth-1, &flag, d);
                else
                    success = Add_Node_mut(type[0], random_opervar, 0, depth-1, &flag, d, cur_el);
            }
            else
            {
                random_type = 1+rand() % 2;
                if(random_type == 1)
                {
                    random_opervar = rand() % num_var;
                    if (notmut)
                        success = AddNode(type[random_type], random_opervar, 0, depth-1, &flag, d);
                    else
                        success = Add_Node_mut(type[random_type], random_opervar, 0, depth-1, &flag, d, cur_el);
                }
                else if(random_type == 2)
                {
                    random_const = (double) rand() / (double)(RAND_MAX);
                    if(random_const < 0.05)
                    {
                        if(rand()%2==0){random_const = M_PI;}
                        else {random_const = M_E;}
                    }
                    else{random_const = generate_normal();}
                    if (notmut)
                        success = AddNode(type[random_type], 0, random_const, depth-1, &flag, d);
                    else
                        success = Add_Node_mut(type[random_type], 0, random_const, depth-1, &flag, d, cur_el);
                }
            }
            if(added_nodes >= target_nodes && notmut == false)
            {
                success = 0;
            }
        }
        success = 1;
        while (success == 1)
        {
            flag = 0;
            random_type = 1+rand() % 2;
            if(random_type == 1)
            {
                random_opervar = rand() % num_var;
                if (notmut)
                    success = AddNode(type[random_type], random_opervar, 0, depth-1, &flag, d);
                else
                    success = Add_Node_mut(type[random_type], random_opervar, 0, depth-1, &flag, d, cur_el);
            }
            else if(random_type == 2)
            {
                random_const = (double) rand() / (double)(RAND_MAX);
                if(random_const < 0.05)
                {
                    if(rand()%2==0){random_const = M_PI;}
                    else {random_const = M_E;}
                }
                else{random_const = generate_normal();}
                if (notmut)
                    success = AddNode(type[random_type], 0, random_const, depth-1, &flag, d);
                else
                    success = Add_Node_mut(type[random_type], 0, random_const, depth-1, &flag, d, cur_el);
            }
        }
    }
}

void Tree::PointMut(Node* cur_el, double p)
{
    double mut, before, random_const;
    int random_opervar;
    if(cur_el==NULL){return;}
    mut = ((double) rand() / (double)(RAND_MAX));
    if (mut <= p)
    {
        if(cur_el->type=='o')
        {
            before = cur_el->operation;
            do
            {
                if(before < 2000)
                {
                    random_opervar = (rand() % unar_oper)+1000;
                }
                else if(before >= 2000 && before < 3000)
                {
                    random_opervar = (rand() % bin_oper)+2000;
                }
                else if(before >= 3000)
                {
                    random_opervar = (rand() % trin_oper)+3000;
                }
            }while (before == random_opervar);
            cur_el->operation = random_opervar;
        }
        else if(cur_el->type=='v')
        {
            before = cur_el->variable;
            if (num_var > 1)
            {
               do
                {
                    random_opervar = rand() % num_var;
                }while (before == random_opervar);
                cur_el->variable = random_opervar;
            }
        }
        else if(cur_el->type=='c')
        {
            before = cur_el->constant;
            do
            {
                random_const = (double) rand() / (double)(RAND_MAX);
                if(random_const < 0.05)
                {
                    if(rand()%2==0){random_const = M_PI;}
                    else {random_const = M_E;}
                }
                else{random_const = generate_normal();}
            }while (before == random_const);
            cur_el->constant = random_const;
        }
        //cout << endl;
        //cout << "Mutation "<< before << endl;
        //cout << endl;
    }
    PointMut(cur_el->left, p);
    PointMut(cur_el->mid, p);
    PointMut(cur_el->right, p);

}

void Tree::PartMut(int switch_init, Node*& cur_el, int depth, int mut_node, int* current_node, int* d, int* flag)
{
    int before, after, target, cur_d;
    if(cur_el==NULL){return;}
    *current_node += 1;
    if (*current_node > mut_node){return;}
    if (*current_node == mut_node)
    {
        //cout << "�� �������" << endl;
        //PrintTree();
        *d = CountDepth(cur_el);
        ClearTree(cur_el);
        if(*current_node == 0)
        {
            Growth(switch_init, depth, root, num_nodes, 0, false);
            //cout << "����� �������" << endl;
            //PrintTree();
            return;
        }
        *flag = 1;//��������, ��� ������ �������
        return;
    }
    PartMut(switch_init, cur_el->left, depth, mut_node, current_node, d, flag);
    if(*flag == 1)
    {
        cur_el->left = NULL;
        cur_d = depth-*d;
        before = num_nodes;
        UpNumNodes();
        after = num_nodes;
        target = before-after;
        Growth(switch_init, depth, cur_el->left, target, cur_d, false);
        *flag = 2;
    }
    if(*flag == 2){return;}
    PartMut(switch_init, cur_el->right, depth, mut_node, current_node, d, flag);
    if(*flag == 1)
    {
        cur_el->right = NULL;
        cur_d = depth-*d;
        before = num_nodes;
        UpNumNodes();
        after = num_nodes;
        target = before-after;
        Growth(switch_init, depth, cur_el->right, target, cur_d, false);
        *flag = 2;
    }
    if(*flag == 2){return;}
    PartMut(switch_init, cur_el->mid, depth, mut_node, current_node, d, flag);
    if(*flag == 1)
    {
        cur_el->mid = NULL;
        cur_d = depth-*d;
        before = num_nodes;
        UpNumNodes();
        after = num_nodes;
        target = before-after;
        Growth(switch_init, depth, cur_el->mid, target, cur_d, false);
        *flag = 2;
    }
    if(*flag == 2){return;}
}

void Tree::StandCross(Node* cur_el, int splitter, Node*& cut_tree, bool cut)
{
    if(cur_el==NULL){return;}
    counter++;
    if (counter == splitter)
    {
        if(cut){copying(cur_el, cut_tree);}
        else
        {
             ClearTree(cur_el->left);
             cur_el->left = NULL;
             //cout << "here" << endl;
             ClearTree(cur_el->right);
             cur_el->right = NULL;
             ClearTree(cur_el->mid);
             cur_el->mid = NULL;
             //PrintTree();
             cur_el->type = cut_tree->type;
             cur_el->operation = cut_tree->operation;
             cur_el->variable = cut_tree->variable;
             cur_el->constant = cut_tree->constant;
             copying(cut_tree->left, cur_el->left);
             copying(cut_tree->right, cur_el->right);
             copying(cut_tree->mid, cur_el->mid);
        }
    }
     else if(counter > splitter)
        return;
    StandCross(cur_el->left, splitter, cut_tree, cut);
    StandCross(cur_el->right, splitter, cut_tree, cut);
    StandCross(cur_el->mid, splitter, cut_tree, cut);
}

int part(double* p, int* in, int start, int _end)
{
    double pivot = p[_end];
    int pIndex = start;
    for (int i = start; i < _end; i++)
    {
        if (p[i] <= pivot)
        {
            swap(p[i], p[pIndex]);
            swap(in[i], in[pIndex]);
            pIndex++;
        }
    }
    swap (p[pIndex], p[_end]);
    swap (in[pIndex], in[_end]);
    return pIndex;
}

void quicksort(double* p, int* in, int start, int _end)
{
    if (start >= _end) {
        return;
    }
    int pivot = part(p, in, start, _end);
    quicksort(p, in, start, pivot - 1);
    quicksort(p, in, pivot + 1, _end);
}

int part(double* p, double* in, int start, int _end)
{
    double pivot = p[_end];
    int pIndex = start;
    for (int i = start; i < _end; i++)
    {
        if (p[i] <= pivot)
        {
            swap(p[i], p[pIndex]);
            swap(in[i], in[pIndex]);
            pIndex++;
        }
    }
    swap (p[pIndex], p[_end]);
    swap (in[pIndex], in[_end]);
    return pIndex;
}

void quicksort(double* p, double* in, int start, int _end)
{
    if (start >= _end) {
        return;
    }
    int pivot = part(p, in, start, _end);
    quicksort(p, in, start, pivot - 1);
    quicksort(p, in, pivot + 1, _end);
}

int part(int* p, double* in, int start, int _end)
{
    int pivot = p[_end];
    int pIndex = start;
    for (int i = start; i < _end; i++)
    {
        if (p[i] <= pivot)
        {
            swap(p[i], p[pIndex]);
            swap(in[i], in[pIndex]);
            pIndex++;
        }
    }
    swap (p[pIndex], p[_end]);
    swap (in[pIndex], in[_end]);
    return pIndex;
}

void quicksort(int* p, double* in, int start, int _end)
{
    if (start >= _end) {
        return;
    }
    int pivot = part(p, in, start, _end);
    quicksort(p, in, start, pivot - 1);
    quicksort(p, in, pivot + 1, _end);
}

void synthetic_data(double** x, double* y, int num_obs)
{
    int i;
    for(i = 0; i < num_obs; i++)
    {
        x[i][0] = dis(gen);
        x[i][1] = dis(gen);
        x[i][2] = dis(gen);
        y[i] = x[i][0]*x[i][0]+0*x[i][1];
    }
}

// ����������� ������ � ��������, ����� �����, ����� ������, ����� � ��������
void init_population(int switch_init, int n, Tree* tree, int depth)
{
    int i;
    for(i = 0; i < n; i++)
    {
         tree[i].Growth(switch_init, depth, tree[i].root);
    }
}

string delete_brackets(string s)
{
    if (s.find("(")==0)
    {
        s.erase(0, 1);
        s.erase(s.size()-1,1);
    }
    return s;
}

double add_lex_selection1(double* rangs, int* accept_index, int n, int* n_new)
{
    double min_value;
    int i, j;
    min_value = rangs[accept_index[0]];
    for(i = 0; i < n; i++)
    {
        j = accept_index[i];
        if(rangs[j] <= min_value)
        {
            min_value = rangs[j];
        }
    }
    for(i = 0; i < n; i++)
    {
        j = accept_index[i];
        if(rangs[j] == min_value)
        {
            (*n_new)++;
        }
    }
    return min_value;
}

void add_lex_selection2(double* rangs, int* accept_index, int n, double min_value, int* win_index)
{
    int i, j, k = 0;
    for(i = 0; i < n; i++)
    {
        j = accept_index[i];
        if(rangs[j] == min_value)
        {
            win_index[k] = j;
            k++;
        }
    }
}

void selection (string sel_switch, double *fitness, int n, Tree* parents, Tree* tree, double** rangs, int nrang)
{
    double *temp_fitness = new double[n];
    double sumfitness = 0;
    int *marks = new int[n];
    int *index = new int[n];
    int fight, i, j, candidate1, candidate2, winner, cont = 0, k, sumrang = 0, n_new, n_cur;;
    double *rang = new double[n];
    double *p = new double[n];
    double temp = 0, randp, min_value;
    if (sel_switch == "prop")
    {
        for (i = 0; i < n; i++)
        {
            sumfitness += fitness[i];
        }
        for (i = 0; i < n; i++)
        {
            p[i] = temp + (double)fitness[i]/((double)sumfitness);
            //cout << p[i] << endl;
            temp += (double)fitness[i]/((double)sumfitness);
        }
        for (fight = 0; fight < n*2; fight++)
        {
            randp = ((double) rand() / (RAND_MAX));
            if (randp >= 0&&randp < p[0])
            {
                parents[fight].CloneTree(tree[0].root);
                continue;
            }
            for (j = 0; j < n-1; j++)
            {
                if (randp < p[j+1]&&randp >=p[j])
                {
                    parents[fight].CloneTree(tree[j+1].root);
                    break;
                }
            }
        }
    }
    if (sel_switch == "rang")
    {
        for (i = 0; i < n; i++)
        {
            temp_fitness[i] = fitness[i];
            index[i] = i;//������ ���������� � ����
            rang[i] = i + 1;//���� ������� ���� �� ������
            marks[i] = 0;
        }
        quicksort(temp_fitness, index, 0, n - 1);
        for (i = 0; i < n-1; i++)
        {
            if (marks[i] == 2)
                continue;
            sumrang = i+1;
            for (j = i + 1; j < n; j++)
            {
                if (temp_fitness[i] == temp_fitness[j])
                {
                    cont +=1;
                    marks[i] = 1;
                    marks[j] = 1;
                    sumrang += j+1;
                }
            }
            if (cont > 0)
            {
                cont += 1;
                for (k = 0; k < n; k++)
                {
                    if (marks[k] == 1)
                    {
                        rang[k] = (double)sumrang/((double) cont);
                        marks[k] = 2;
                    }
                }
            }
            cont = 0;
            sumrang = 0;
        }
        quicksort(index, rang, 0, n-1);
        for (i = 0; i < n; i++)
        {
            sumfitness += rang[i];
        }
        for (i = 0; i < n; i++)
        {
            p[i] = temp + (double)rang[i]/((double)sumfitness);
            //cout << p[i] << endl;
            temp += (double)rang[i]/((double)sumfitness);
        }

        for (fight = 0; fight < n*2; fight++)
        {
            randp = ((double) rand() / (RAND_MAX));
            if (0 <= randp && randp < p[0])
            {
                parents[fight].CloneTree(tree[0].root);
                continue;
            }
            for (j = 0; j < n-1; j++)
            {
                if (p[j] <= randp && randp < p[j+1])
                {
                    parents[fight].CloneTree(tree[j+1].root);
                    break;
                }
            }
        }
    }
    if (sel_switch == "tour")
    {
         for (fight = 0; fight < n*2; fight++)
        {
            candidate1 = rand() % n;
            candidate2 = rand() % n;
            if (fitness[candidate1] > fitness[candidate2])
            {
                parents[fight].CloneTree(tree[candidate1].root);
            }
            if (fitness[candidate1] < fitness[candidate2])
            {
                parents[fight].CloneTree(tree[candidate2].root);
            }
            if (fitness[candidate1] == fitness[candidate2])
            {
                winner = rand() % 2;
                if (winner == 0)
                    parents[fight].CloneTree(tree[candidate1].root);
                if (winner == 1)
                    parents[fight].CloneTree(tree[candidate2].root);
            }
        }
    }
    if (sel_switch == "lex")
    {
        int* winners;
        int* idx;
        int criteria[3]{0, 1, 2};
        for (fight = 0; fight < n*2; fight++)
        {
            //cout << "fight " << fight << endl;
            n_new = n;
            idx = new int[n_new];
            shuffle(begin(criteria), end(criteria), gen);
            for (i = 0; i < n; i++)
            {
                idx[i] = i;
            }
            for(i = 0; i < nrang; i++)
            {
                //cout << "nrang " << i << endl;
                n_cur = n_new;
                n_new = 0;
                min_value = add_lex_selection1(rangs[criteria[i]], idx, n_cur, &n_new);
                winners = new int[n_new];
                add_lex_selection2(rangs[criteria[i]],idx, n_cur, min_value, winners);
                delete[] idx;
                idx = new int[n_new];
                for (j = 0; j < n_new; j++)
                {
                    idx[j] = winners[j];
                }
                delete[] winners;
                if(n_new == 1)
                    break;
            }
            j = rand() % n_new;
            parents[fight].CloneTree(tree[idx[j]].root);
            delete[] idx;
        }
    }
    delete[] marks;
    delete[] temp_fitness;
    delete[] index;
    delete[] rang;
    delete[] p;
}

void mutation(string mut_switch, int switch_init, int n, Tree *children, double ppoint, double ppart)
{
    double mut;
    int i, flag = 0, depth, mut_node, j, curr_node, depth2, d;
    Tree temp;
    if(mut_switch == "point")
    {
        for (i = 0; i< n; i++)
        {
            children[i].PointMut(children[i].root, ppoint);
        }
    }
    if(mut_switch == "part")
    {
        for (i = 0; i < n; i++)
        {
            mut = ((double) rand() / (double)(RAND_MAX));
            if(mut < ppart)
            {
                children[i].UpNumNodes();
                if (children[i].num_nodes == 1)
                    mut_node = 0;
                else mut_node = rand() % (children[i].num_nodes-1)+1;
                depth = children[i].CountDepth(children[i].root);
                flag = 0;
                curr_node = -1;
                temp.CloneTree(children[i].root);
                temp.UpNumNodes();
                temp.PartMut(switch_init, temp.root, depth, mut_node, &curr_node, &d, &flag);
                temp.UpNumNodes();
                if (temp.num_nodes >= 200)
                {
                    cout << i << " got " << temp.num_nodes << endl;
                }
                children[i].CloneTree(temp.root);
                depth2 = children[i].CountDepth(children[i].root);
                if (depth2 > depth)
                    cout << "HERE________________________________________________________________________________________________";
                //cout << i << " done" << endl;
            }
        }
        temp.ClearTree(temp.root);
    }
}

// ���������� �� ������� ������, ����������� � ������
void crossover(int n, Tree *parents, Tree *children)
{
    int i, j, k, splitter1, splitter2;
    Tree cut_tree, temp;
    for (i = 0, k = 0; k < n*2; i++, k += 2)
    {
        //cout << "��������� " << parents[k].printExpression() << endl;
        //cout << "��������� " << parents[k+1].printExpression() << endl;
        parents[k].UpNumNodes();
        parents[k+1].UpNumNodes();
         for (j = 0; j < 100; j++)
        {
            if (parents[k].num_nodes == 1)
                splitter1 = 0;
            else splitter1 = rand() % (parents[k].num_nodes-1)+1;
            if(parents[k+1].num_nodes == 1)
                splitter2 = 0;
            else splitter2 = rand() % (parents[k+1].num_nodes-1)+1;
            parents[k+1].StandCross(parents[k+1].root, splitter2, cut_tree.root, true);
            counter = -1;
            temp.CloneTree(parents[k].root);
            //children[i].PrintTree();
            //cout << splitter1 << " " << splitter2 << " " << k << endl;
            temp.StandCross(temp.root, splitter1, cut_tree.root, false);
            //children[i].PrintTree();
            counter = -1;
            cut_tree.ClearTree(cut_tree.root);
            cut_tree.root = NULL;
            temp.UpNumNodes();
            if (temp.num_nodes < max_nodes)
            {
                children[i].CloneTree(temp.root);
                break;
            }
        }
        if (temp.num_nodes >= max_nodes)
            children[i].CloneTree(temp.root);
    }
    temp.ClearTree(temp.root);
}

void count_rang_fitness(double* arr, double* rang, int n, bool var = false)
{
    int i, j, sumrang = 0, cont = 0, k;
    int *marks = new int[n];
    int *index = new int[n];
    for(i = 0; i < n; i++)
    {
        index[i] = i;
        rang[i] = i + 1;
        marks[i] = 0;
    }
    quicksort(arr, index, 0, n - 1);
    if (var)
    {
        reverse(arr, arr+n);
        reverse(index, index+n);
    }
    for (i = 0; i < n-1; i++)
    {
        if (marks[i] == 2)
            continue;
        sumrang = i+1;
        for (j = i + 1; j < n; j++)
        {
            if (arr[i] == arr[j])
            {
                cont +=1;
                marks[i] = 1;
                marks[j] = 1;
                sumrang += j+1;
            }
        }
        if (cont > 0)
        {
            cont += 1;
            for (k = 0; k < n; k++)
            {
                if (marks[k] == 1)
                {
                    rang[k] = (double)sumrang/((double) cont);
                    marks[k] = 2;
                }
            }
        }
        cont = 0;
        sumrang = 0;
    }
    quicksort(index, rang, 0, n-1);
    delete[] marks;
    delete[] index;
}

double find_median(double* arr, int n)
{
    int* index = new int[n];
    quicksort(arr, index, 0, n-1);
    if (n % 2 == 0)
    {
        return (arr[n / 2 - 1] + arr[n / 2]) / 2;
    }
    else
    {
        return arr[n / 2];
    }
    delete[] index;
}


int main()
{
    //ifstream fin("boston_processed_forc++.csv");
    ifstream fin("concrete_data.txt");
    ofstream fout("For_comparison.txt");
    ofstream fout2("Expressions.txt");
    ofstream fout_analysis("For_analysis.txt");
    srand(time(NULL));
    setlocale(0, "");
    int globali, globaln = 25;
    int i = 0, j, k, in, obs, num_obs = 1030-206, num_obs_test = 206, depth = 4, nrang = 3, sum;
    // flag ����� ��� ����, ����� �������� ��������� �� ������� � ������
    int n = 700, num_generals = 500, general, ready_general;
    //n - ���������� ��������� � ���������, num_generals - ���������� ���������
    double MSE1, MSE2, median_MSE;
    int switch_init = 1;//0 - ������ �����, 1 - ����� �����������
    string sel_switch = "tour";// prop, rang, tour, lex
    string mut_switch = "part";// point, part
    string fit_switch = "formula";// formula, rang
    //string cross_switch = "stand";// stand, one
    double nr = 10, vr = 1, er = 20, nf = 5, vf = 10, ef = 1, first_el = 1;//first_el ����� ��� ����, ����� ���������� ����� ����� ������ ����� � �����������, ����� ��������� �������� 0 � 1
    double ppoint = 0.01, ppart = 0.2;
    string line;
    int option, opt;
    while(i == 0)
    {
        cout << "1. �������� ��� ��� ��������� n = " << n << ", num_generals = " << num_generals << ", globaln = " << globaln << endl;
        cout << "nr = " << nr << ", vr = " << vr << ", er = " << er << endl;
        cout << "nf = " << nf << ", vf = " << vf << ", ef = " << ef << ", first_el = " << first_el << endl;
        cout << "ppoint = " << ppoint << ", ppart = " << ppart << ", max_nodes = " << max_nodes << ", depth = " << depth << endl;
        cout << "2. �������� ���������" << endl;
        cin >> option;
        switch(option)
        {
            case 1:
            {
                i++;
                break;
            }
            case 2:
            {
                cout << "1. ���������� ��������� � ���������\n2. ������������ ����������� � ������\n3. ������������ ����������� � �������\n4. ������������ �������\n5. ������� ����������� �������� � ������������ ���������� �����" << endl;
                cin >> opt;
                switch(opt)
                {
                    case 1:
                    {
                        cout << "n ";
                        cin >> n;
                        cout << "num_generals ";
                        cin >> num_generals;
                        cout << "globaln ";
                        cin >> globaln;
                        break;

                    }
                    case 2:
                    {
                        cout << "er ";
                        cin >> er;
                        cout << "nr ";
                        cin >> nr;
                        cout << "vr ";
                        cin >> vr;
                        break;
                    }
                    case 3:
                    {
                        cout << "ef ";
                        cin >> ef;
                        cout << "nf ";
                        cin >> nf;
                        cout << "vf ";
                        cin >> vf;
                        break;
                    }
                    case 4:
                    {
                        cout << "ppoint ";
                        cin >> ppoint;
                        cout << "ppart ";
                        cin >> ppart;
                        break;
                    }
                    case 5:
                    {
                        cout << "depth ";
                        cin >> depth;
                        cout << "max_nodes ";
                        cin >> max_nodes;
                        break;
                    }
                }
                system("cls");
            }
        }
    }

    double** data = new double* [num_obs+num_obs_test];
    double** x = new double* [num_obs];//������� - ����������, ������� - �������� ��� ������� ����������
    for(i = 0; i < num_obs; i++)
    {
        x[i] = new double[num_var];
    }
    double** x_test = new double* [num_obs_test];
    for(i = 0; i < num_obs_test; i++)
    {
        x_test[i] = new double[num_var];
    }
    double* y = new double [num_obs];
    double* y_test = new double [num_obs_test];
    double* fitness = new double[n*2];
    double* fitness_temp = new double[n];
    Tree* tree = new Tree[n];
    Tree* tree_temp = new Tree[n];
    Tree* parents = new Tree[n*2];
    Tree* children = new Tree[n];
    int *index = new int[n*2];
    double *for_calc = new double[n*2];
    double** rangs = new double* [nrang];//nodes, vars, errors
    for(i = 0; i < nrang; i++)
    {
        rangs[i] = new double[n*2];
    }
    double** rangs_temp = new double* [nrang];
    for(i = 0; i < nrang; i++)
    {
        rangs_temp[i] = new double[n];
    }
    double* MSE = new double[globaln];
    double* MSE_test = new double[globaln];
    int* average_generals = new int[globaln];


    for(int i=0;i!=num_obs+num_obs_test;i++)
    {
        data[i] = new double[num_var+1];
        getline(fin, line);
        const char *cstr = line.c_str();
        char* end1;
        int offset = 0;
        for(int j=0;j!=num_var+1;j++)
        {
            data[i][j] = strtod(cstr+offset, &end1);
            offset = end1-cstr;
        }
        //if(i%1000 == 0)
            //cout<<i<<endl;
    }
    for(i = 0; i < num_obs; i++)
    {
        for(j = 0; j < num_var; j++)
        {
            x[i][j] = data[i][j];
        }
        y[i] = data[i][num_var];
    }
    for(i = num_obs, k = 0; i < num_obs+num_obs_test; i++, k++)
    {
        for(j = 0; j < num_var; j++)
        {
            x_test[k][j] = data[i][j];
        }
        y_test[k] = data[i][num_var];
    }

    //synthetic_data(x, y, num_obs);
    //�������� ������������ ����� ������
    /*for(i = 0; i < num_obs; i++)
    {
        for(j = 0; j < num_var; j++)
        {
            cout << x[i][j] << "\t";
        }
        cout << y[i] << endl;
    }*/

    //����� ��� ����� "For_comparison"
    fout << "Parametrs\tmedianMSE_train\tmedianMSE_test\tav_general\n";

    //���������� ���� ��������
    for (switch_init = 0; switch_init < 2; switch_init++)
    {
        for (string i_sel: {"prop", "rang", "tour", "lex"})
        {
            sel_switch = i_sel;
            for (string i_mut: {"point", "part"})
            {
                mut_switch = i_mut;
                for (string i_fit: {"formula", "rang"})
                {
                    fit_switch = i_fit;
                    cout << switch_init << " " << sel_switch << " " << mut_switch << " " << fit_switch << endl;
                    fout << switch_init << " " << sel_switch << " " << mut_switch << " " << fit_switch << "\t";
                    fout2 << switch_init << " " << sel_switch << " " << mut_switch << " " << fit_switch << endl;
                    for (globali = 0; globali < globaln; globali++)
                    {
                        cout << "Launch " << globali << endl;
                        fout2 << "Launch " << globali << endl;
                        init_population(switch_init, n, tree, depth);
                        if (fit_switch == "formula")
                        {
                            for(i = 0; i < n; i++)
                            {
                                fitness[i] = tree[i].count_fitness(tree[i].error(num_obs, x, y), ef, nf, vf, first_el);
                            }
                        }
                        else if (fit_switch == "rang")
                        {
                            for(i = 0; i < n; i++)
                            {
                                tree[i].UpNumNodes();
                                for_calc[i] = tree[i].num_nodes;
                            }
                            count_rang_fitness(for_calc, rangs[0], n);
                            for(i = 0; i < n; i++)
                            {
                                for_calc[i] = tree[i].CountVar();
                            }
                            count_rang_fitness(for_calc, rangs[1], n, true);
                            for(i = 0; i < n; i++)
                            {
                                for_calc[i] = tree[i].error(num_obs, x, y);
                            }
                            count_rang_fitness(for_calc, rangs[2], n);
                            for(i = 0; i < n; i++)
                            {
                                fitness[i] = nr*rangs[0][i] + vr*rangs[1][i] + er*rangs[2][i];
                            }
                        }

                        //���� ���������
                        for(general = 0; general < num_generals; general++)
                        {
                            MSE1 = MSE2;
                            //cout << "General " << general << endl;

                            /*for(i = 0; i < n; i++)
                            {
                                cout << "Tree " << i <<endl;
                                //tree[i].PrintTree();
                                tree[i].UpNumNodes();
                                cout << "��������� " << tree[i].printExpression() << " " << tree[i].CountDepth(tree[i].root) << " " << tree[i].num_nodes << endl;
                                //cout << "�������� ��������� " << tree[i].evaluateExpression(x[0]) << endl;
                                //cout << "�������� ������� ����������� " << fitness[i] << " vars " << tree[i].CountVar() << endl;
                                //cout << "MSE " << tree[i].error(num_obs, x, y) << endl;
                            }*/

                            /*for(i = 0; i < n; i++)
                            {
                                cout << "Tree " << i <<endl;
                                //tree[i].PrintTree();
                                tree[i].UpNumNodes();
                                cout << "��������� " << tree[i].printExpression() << " " << tree[i].CountDepth(tree[i].root) << " " << tree[i].num_nodes << endl;
                                //cout << "�������� ��������� " << tree[i].evaluateExpression(x[0]) << endl;
                                cout << "�������� ������� ����������� " << fitness[i] << " vars " << tree[i].CountVar() << endl;
                                cout << "MSE " << tree[i].error(num_obs, x, y) << endl;
                            }*/
                            selection(sel_switch, fitness, n, parents, tree, rangs, nrang);
                            //cout << "selection is done" << endl;
                            /*for(i = 0; i < n*2; i++)
                            {
                                cout << "Parent " << i <<endl;
                                parents[i].UpNumNodes();
                                cout << "��������� " << parents[i].printExpression() << " " << parents[i].num_nodes << endl;
                            }*/
                            crossover(n, parents, children);
                            //cout << "crossover is done" << endl;
                            /*if (general == 94)
                            {
                                for(i = 0; i < n; i++)
                                {
                                    cout << "Child before mutation " << i <<endl;
                                    //children[i].PrintTree();
                                    cout << "��������� " << children[i].printExpression() << endl;
                                }
                            }*/
                            /*for(i = 0; i < n; i++)
                            {
                                cout << "Child before mutation " << i <<endl;
                                //children[i].PrintTree();
                                cout << "��������� " << children[i].printExpression() << endl;
                            }*/
                            /*if (general == 94)
                            {
                                mutation1(mut_switch, switch_init, n, children);
                            }
                            else*/
                            mutation(mut_switch, switch_init, n, children, ppoint, ppart);
                            //cout << "mutation is done" << endl;
                            /*if (general == 94)
                            {
                                for(i = 0; i < n; i++)
                                {
                                    cout << "Child after mutation " << i <<endl;
                                    //children[i].PrintTree();
                                    cout << "��������� " << children[i].printExpression() << endl;
                                }
                            }*/
                            if (fit_switch == "formula")
                            {
                                for(i = 0; i < n; i++)
                                {
                                    fitness[i+n] = children[i].count_fitness(children[i].error(num_obs, x, y), ef, nf, vf, first_el);
                                }
                            }
                            else if (fit_switch == "rang")
                            {
                                for(i = 0; i < n; i++)//����� ����������� ����������� � ����
                                {
                                    children[i].UpNumNodes();
                                    for_calc[i+n] = children[i].num_nodes;
                                }
                                for(i = 0; i < n; i++)
                                {
                                    tree[i].UpNumNodes();
                                    for_calc[i] = tree[i].num_nodes;
                                }
                                count_rang_fitness(for_calc, rangs[0], n*2);
                                for(i = 0; i < n; i++)
                                {
                                    for_calc[i+n] = children[i].CountVar();
                                }
                                for(i = 0; i < n; i++)
                                {
                                    for_calc[i] = tree[i].CountVar();
                                }
                                count_rang_fitness(for_calc, rangs[1], n*2, true);
                                for(i = 0; i < n; i++)
                                {
                                    for_calc[i+n] = children[i].error(num_obs, x, y)/num_obs;
                                }
                                for(i = 0; i < n; i++)
                                {
                                    for_calc[i] = tree[i].error(num_obs, x, y)/num_obs;
                                }
                                count_rang_fitness(for_calc, rangs[2], n*2);
                                for(i = 0; i < n*2; i++)
                                {
                                    fitness[i] = nr*rangs[0][i] + vr*rangs[1][i] + er*rangs[2][i];
                                }
                            }
                            //cout << "fitness is counted" << endl;
                            for (i = 0; i < n*2; i++)
                                index[i] = i;
                            quicksort(fitness, index, 0, n*2 - 1);
                            if(fit_switch == "rang")
                            {
                                reverse(fitness, fitness+n*2);
                                reverse(index, index+n*2);
                            }
                            //����� ���������
                            for (i = 0, k = n*2-1; i < n; i++,k--)
                            {
                                in = index[k];
                                if (in < n)
                                {
                                    tree_temp[i].CloneTree(tree[in].root);
                                    fitness_temp[i] = fitness[k];
                                    for(j = 0; j < nrang; j++)
                                    {
                                        rangs_temp[j][i] = rangs[j][in];
                                    }
                                }
                                else if (in >= n)
                                {
                                    tree_temp[i].CloneTree(children[in-n].root);
                                    fitness_temp[i] = fitness[k];
                                    for(j = 0; j < nrang; j++)
                                    {
                                        rangs_temp[j][i] = rangs[j][in];
                                    }
                                }
                            }
                            for (i = 0; i < n; i++)
                            {
                                tree[i].CloneTree(tree_temp[i].root);
                                fitness[i] = fitness_temp[i];
                                for(j = 0; j < nrang; j++)
                                {
                                    rangs[j][i] = rangs_temp[j][i];
                                }
                            }
                            //cout << "the best are found" << endl;
                            //fout.close();
                            MSE2 = (tree[0].error(num_obs, x, y))/num_obs;
                            if (abs(MSE1-MSE2)>0.001)
                            {
                                ready_general = general;
                            }
                        }
                        average_generals[globali] = ready_general;
                        tree[0].UpNumNodes();
                        cout << "������ ������� " << tree[0].printExpression() << " " << tree[0].CountDepth(tree[0].root) << " " << tree[0].num_nodes<< endl;
                        //cout << "�������� ������� ����������� " << fitness[0] << " vars " << tree[0].CountVar() << endl;
                        cout << "MSE " << MSE2 << endl;
                        fout2 << tree[0].printExpression() << endl;
                        fout2 << "MSE " << MSE2 << "\t";
                        MSE[globali] = MSE2;
                        fout_analysis << MSE2 << "\t";
                        MSE1 = tree[0].error(num_obs_test, x_test, y_test);
                        MSE_test[globali] = MSE1/num_obs_test;
                        cout << "MSE_test " << MSE_test[globali] << endl;
                        fout2 << "MSE_test  " << MSE_test[globali] << endl;
                        fout_analysis << MSE_test[globali] << endl;
                        for(i = 0; i < n; i++)
                        {
                            tree[i].ClearTree(tree[i].root);
                        }

                    }
                    sum = 0;
                    for (globali = 0; globali < globaln; globali++)
                    {
                        sum+=average_generals[globali];
                    }
                    median_MSE = find_median(MSE, globaln);
                    cout << "Median_train_MSE " << median_MSE << endl;
                    fout << median_MSE << "\t";
                    median_MSE = find_median(MSE_test, globaln);
                    cout << "Median_test_MSE " << median_MSE << endl;
                    fout << median_MSE << "\t";
                    cout << "ready_general " << (double)sum/(double)globaln << endl;
                    fout << (double)sum/(double)globaln << endl;// �� ����� ��������� ������� ���� �������
                }
            }
        }
    }

    delete[] average_generals;
    delete[] MSE_test;
    for(i = 0; i < nrang; i++)
    {
        delete[] rangs[i];
    }
    delete[] rangs;
    for(i = 0; i < nrang; i++)
    {
        delete[] rangs_temp[i];
    }
    delete[] rangs_temp;
    delete[] children;
    delete[] parents;
    delete[] for_calc;
    for(i = 0; i < num_obs+num_obs_test; i++)
    {
        delete[] data[i];
    }
    delete[] data;
    delete[] fitness;
    delete[] fitness_temp;
    delete[] tree;
    delete[] tree_temp;
    delete[] index;
    for(i = 0; i < num_obs_test; i++)
    {
        delete[] x_test[i];
    }
    delete[] x_test;
    for(i = 0; i < num_obs; i++)
    {
        delete[] x[i];
    }
    delete[] x;
    delete[] y_test;
    delete[] y;
    fin.close();
    fout.close();
    fout2.close();
    fout_analysis.close();
}


// ��� ��������� ������� ��� ���������� ������������� ���������� ����� ����� ������� �������� ������???
// ������ ����������� �� ������������ ���������� ����� � ������ ��� �������� � ��������� �������, ���� ������ ������� �� �����, �� ��������� �����, ���� �� ��������� 100 ���
// ��������� ���� � ����������� ������ ��������� ����
// ������������� ��������� � �����: �����������, ��������, �������, �����������,
// ������������� ��������� �������: ������������ �����������, ����������� �������, ���� ���������� �����, ������� ������������ ������, ���������� ��������� � ���������
