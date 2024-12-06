#include <iostream>
#include <cstring>
#include <limits>

using namespace std;

const int STRING_SIZE = 32;

class FamilyMember
{
public:
    // 名字
    char name[STRING_SIZE];
    // 指向下一个兄弟
    FamilyMember *nextSibling = nullptr;
    // 指向第一个孩子
    FamilyMember *firstChild = nullptr;
    // 默认构造函数
    FamilyMember() {}
    FamilyMember(const char *name)
    {
        strncpy(this->name, name, STRING_SIZE - 1);
        this->name[STRING_SIZE - 1] = '\0';
    }
    FamilyMember(FamilyMember *sibling, FamilyMember *child, const char *name) : nextSibling(sibling), firstChild(child)
    {
        strncpy(this->name, name, STRING_SIZE - 1);
        this->name[STRING_SIZE - 1] = '\0';
    }
    ~FamilyMember() {}
};

class FamilyTree
{
    friend class FamilyMember;

private:
    // 指向树的根节点的指针
    FamilyMember *rootMember;

public:
    // 默认构造函数
    FamilyTree() : rootMember(nullptr) {}
    ~FamilyTree()
    {
        deleteSubTree(rootMember);
        rootMember = nullptr;
    }
    // 初始化家谱，获取祖先姓名
    void initializeTree();
    // 建立家庭
    void buildFamily();
    // 查找成员
    void findMember();
    // 根据名字递归地找到某个家庭成员并返回其地址
    FamilyMember *findMemberAddress(FamilyMember *currentMember, const char *name);
    // 根据名字递归地找到某个家庭成员的前一个成员并返回其地址
    FamilyMember *findPreviousMemberAddress(FamilyMember *currentMember, const char *name);
    // 为某个成员增加孩子
    void addChildToMember();
    // 解散局部家庭
    void dissolveSubTree();
    // 递归地删除某个家庭成员及其子孙
    void deleteSubTree(FamilyMember *memberRoot);
    // 为某个家庭成员改名
    void renameMember();
    // 打印家谱
    void printFamilyMember(FamilyMember *member, int indentLevel = 0);
    void printFamilyTree();
};

void FamilyTree::initializeTree()
{
    char ancestorName[STRING_SIZE];
    cout << "首先建立一个家谱！" << endl;
    cout << "请输入祖先的姓名：";
    cin >> ancestorName;
    rootMember = new FamilyMember(ancestorName);
    cout << "家谱创建成功！" << endl;
    cout << "此家谱的祖先是：" << rootMember->name << endl;
}

void FamilyTree::buildFamily()
{
    char memberName[STRING_SIZE];
    int num_children;

    cout << "请输入要建立的家庭成员的姓名：";
    cin >> memberName;

    FamilyMember *currentMember = findMemberAddress(rootMember, memberName);

    if (!currentMember)
    {
        cout << "未找到该成员！" << endl;
        return;
    }
    if (currentMember->firstChild)
    {
        cout << "该成员已有子女！" << endl;
        return;
    }
    cout << "请输入 " << currentMember->name << " 的子女数目：";
    cin >> num_children;

    cout << "请依次输入 " << memberName << " 的子女姓名：" << endl;
    cin >> memberName;
    // 创建第一个孩子
    FamilyMember *newChild = new FamilyMember(memberName);
    currentMember->firstChild = newChild;
    FamilyMember *lastChild = newChild;
    // 创建其他孩子
    for (int i = 1; i < num_children; i++)
    {
        cin >> memberName;

        FamilyMember *nextChild = new FamilyMember(memberName);
        lastChild->nextSibling = nextChild;
        lastChild = nextChild;
    }
    cout << "家庭成员添加成功！" << endl;
    cout << currentMember->name << " 的第一代子孙是：";
    while (newChild)
    {
        cout << newChild->name << " ";
        newChild = newChild->nextSibling;
    }
    cout << endl;
}

void FamilyTree::findMember()
{
    char memberName[STRING_SIZE];
    cout << "请输入要查找的家庭成员的姓名：";
    cin >> memberName;

    FamilyMember *memberAddress = findMemberAddress(rootMember, memberName);

    if (!memberAddress)
    {
        cout << "未找到该成员！" << endl;
        return;
    }

    cout << "成员 " << memberName << " 的信息如下：" << endl;
    FamilyMember *currentChild = memberAddress->firstChild;
    if (!currentChild)
    {
        cout << memberName << " 没有子孙！" << endl;
        return;
    }
    cout << memberName << " 的第一代子孙是：" << endl;
    while (currentChild)
    {
        cout << currentChild->name << " ";
        currentChild = currentChild->nextSibling;
    }
    cout << endl;
}

FamilyMember *FamilyTree::findMemberAddress(FamilyMember *currentMember, const char *name)
{
    if (!currentMember)
    {
        return nullptr;
    }
    if (strcmp(currentMember->name, name) == 0)
    {
        return currentMember;
    }

    FamilyMember *childAddress = findMemberAddress(currentMember->firstChild, name);
    if (childAddress)
    {
        return childAddress;
    }

    FamilyMember *siblingAddress = findMemberAddress(currentMember->nextSibling, name);
    if (siblingAddress)
    {
        return siblingAddress;
    }

    return nullptr;
}

FamilyMember *FamilyTree::findPreviousMemberAddress(FamilyMember *currentMember, const char *name)
{
    if (!currentMember)
    {
        return nullptr;
    }

    // 检查当前节点的第一个孩子
    if (currentMember->firstChild && strcmp(currentMember->firstChild->name, name) == 0)
    {
        return currentMember;
    }

    // 检查当前节点的兄弟节点
    if (currentMember->nextSibling && strcmp(currentMember->nextSibling->name, name) == 0)
    {
        return currentMember;
    }

    // 递归检查孩子
    FamilyMember *childResult = findPreviousMemberAddress(currentMember->firstChild, name);
    
    if (childResult)
    {
        return childResult;
    }

    // 递归检查兄弟节点
    return findPreviousMemberAddress(currentMember->nextSibling, name);
}

void FamilyTree::addChildToMember()
{
    char memberName[STRING_SIZE];
    int num_children;

    cout << "请输入要添加子女的家庭成员的姓名：";
    cin >> memberName;

    FamilyMember *currentMember = findMemberAddress(rootMember, memberName);

    if (!currentMember)
    {
        cout << "未找到该成员！" << endl;
        return;
    }

    if (!currentMember->firstChild)
    {
        // 该成员没有子女
        cout << "请输入 " << currentMember->name << " 新添加的子女姓名：";
        cin >> memberName;

        FamilyMember *currentChild = new FamilyMember(memberName);
        currentMember->firstChild = currentChild;
        cout << currentMember->name << " 的第一代子孙是：" << currentChild->name << endl;
        return;
    }

    FamilyMember *currentChild = currentMember->firstChild;
    while (currentChild->nextSibling)
    {
        currentChild = currentChild->nextSibling;
    }
    cout << "请输入 " << currentMember->name << " 新添加的子女姓名：";
    cin >> memberName;

    currentChild->nextSibling = new FamilyMember(memberName);
    cout << currentMember->name << " 的第一代子孙是：";
    while (currentChild)
    {
        cout << currentChild->name << " ";
        currentChild = currentChild->nextSibling;
    }
    cout << endl;
}

void FamilyTree::dissolveSubTree()
{
    char memberName[STRING_SIZE];
    cout << "请输入要解散家庭的家庭成员的姓名：";
    cin >> memberName;

    // 检查根节点是否就是要删除的节点
    if (strcmp(rootMember->name, memberName) == 0)
    {
        cout << "正在删除家庭 " << rootMember->name << " 及其所有子孙..." << endl;
        deleteSubTree(rootMember);
        rootMember = nullptr; // 清空根节点
        return;
    }

    // 找到待删除成员的前一个节点
    FamilyMember *previousMember = findPreviousMemberAddress(rootMember, memberName);
    if (!previousMember)
    {
        cout << "未找到该成员！" << endl;
        return;
    }

    FamilyMember *targetSubTree = nullptr;

    // 判断待删除节点是前一个节点的第一个孩子还是兄弟
    if (previousMember->firstChild && strcmp(previousMember->firstChild->name, memberName) == 0)
    {
        targetSubTree = previousMember->firstChild;
        previousMember->firstChild = targetSubTree->nextSibling; // 更新指针
    }
    else if (previousMember->nextSibling && strcmp(previousMember->nextSibling->name, memberName) == 0)
    {
        targetSubTree = previousMember->nextSibling;
        previousMember->nextSibling = targetSubTree->nextSibling; // 更新指针
    }
    else
    {
        cout << "未找到该成员！" << endl;
        return;
    }

    // 显示要删除的子孙信息
    FamilyMember *currentChild = targetSubTree->firstChild;
    if (!currentChild)
    {
        cout << memberName << " 没有子孙！" << endl;
    }
    else
    {
        cout << memberName << " 的第一代子孙是：" << endl;
        while (currentChild)
        {
            cout << currentChild->name << " ";
            currentChild = currentChild->nextSibling;
        }
        cout << endl;
    }

    // 删除子树
    cout << "正在删除家庭 " << memberName << " 及其所有子孙..." << endl;
    deleteSubTree(targetSubTree);
}

void FamilyTree::deleteSubTree(FamilyMember *memberRoot)
{
    if (!memberRoot)
    {
        return;
    }

    // 删除所有子女节点
    FamilyMember *child = memberRoot->firstChild;
    while (child)
    {
        FamilyMember *nextChild = child->nextSibling;
        deleteSubTree(child);
        child = nextChild;
    }

    // 删除当前节点
    delete memberRoot;
}

void FamilyTree::renameMember()
{
    char memberName[STRING_SIZE];
    char newName[STRING_SIZE];
    cout << "请输入要修改姓名的家庭成员目前的姓名：";
    cin >> memberName;

    FamilyMember *currentMember = findMemberAddress(rootMember, memberName);
    if (!currentMember)
    {
        cout << "未找到该成员！" << endl;
        return;
    }

    cout << "请输入 " << memberName << " 的新姓名：";
    cin >> newName;

    strncpy(currentMember->name, newName, STRING_SIZE - 1);
    currentMember->name[STRING_SIZE - 1] = '\0';
    cout << memberName << " 已更名为：" << newName << endl;
}

void FamilyTree::printFamilyMember(FamilyMember *member, int indentLevel)
{
    if (!member)
    {
        return;
    }
    for (int i = 0; i < indentLevel; i++)
    {
        cout << "  ";
    }
    cout << member->name << endl;
    // 递归打印孩子
    FamilyMember *child = member->firstChild;
    while (child)
    {
        printFamilyMember(child, indentLevel + 1);
        child = child->nextSibling;
    }
}

void FamilyTree::printFamilyTree()
{
    cout << "家谱如下：" << endl;
    printFamilyMember(rootMember);
}

class FamilyTreeManager
{
public:
    FamilyTree familyTree;
    void printMenu();
    void run();
};

void FamilyTreeManager::printMenu()
{
    cout << "**             家谱管理系统                **" << endl;
    cout << "=============================================" << endl;
    cout << "**           请选择要执行的操作            **" << endl;
    cout << "**            A --- 完善家谱               **" << endl;
    cout << "**            B --- 添加家庭成员           **" << endl;
    cout << "**            C --- 解散局部家庭           **" << endl;
    cout << "**            D --- 更改家庭成员姓名       **" << endl;
    cout << "**            E --- 查找家庭成员           **" << endl;
    cout << "**            F --- 打印所有家庭成员       **" << endl;
    cout << "**            G --- 退出程序               **" << endl;
    cout << "=============================================" << endl;
}

void FamilyTreeManager::run()
{
    familyTree.initializeTree();

    char choice;
    while (true)
    {
        cout << "请选择要执行的操作：";
        cin >> choice;
        cin.clear();

        switch (choice)
        {
        case 'A':
            familyTree.buildFamily();
            break;
        case 'B':
            familyTree.addChildToMember();
            break;
        case 'C':
            familyTree.dissolveSubTree();
            break;
        case 'D':
            familyTree.renameMember();
            break;
        case 'E':
            familyTree.findMember();
            break;
        case 'F':
            familyTree.printFamilyTree();
            break;
        case 'G':
            cout << "程序结束！" << endl;
            return;
        default:
            cout << "输入错误！" << endl;
            break;
        }
        cout << endl;
    }
}

int main()
{
    FamilyTreeManager manager;
    manager.printMenu();
    manager.run();

    return 0;
}
