#include <iostream>
#include <cstring>

using namespace std;

#define ID_LEN 10
#define NAME_LEN 20
#define GENDER_LEN 10
#define EXAMTYPE_LEN 30

// 学生节点类
class Student
{
public:
    char id[ID_LEN];             // 学号
    char name[NAME_LEN];         // 姓名
    char gender[GENDER_LEN];     // 性别
    int age;                     // 年龄
    char examType[EXAMTYPE_LEN]; // 报考类别
    Student *next;               // 指向下一个学生的指针

    // 构造函数
    Student(const char id[], const char name[], const char gender[], int age, const char examType[])
    {
        strcpy(this->id, id);
        strcpy(this->name, name);
        strcpy(this->gender, gender);
        this->age = age;
        strcpy(this->examType, examType);
        this->next = nullptr;
    }

    // 析构函数
    ~Student() {}
};
// 学生管理类
class StudentManager
{
private:
    Student *head; // 链表头指针
public:
    // 构造函数初始化链表为空
    StudentManager() : head(nullptr) {}

    // 析构函数释放链表内存
    ~StudentManager()
    {
        while (head)
        {
            Student *temp = head;
            head = head->next;
            delete temp;
        }
    }

    // 检查考号是否存在
    bool checkIDExist(const char id[]) const
    {
        Student *temp = head;
        while (temp)
        {
            if (strcmp(temp->id, id) == 0)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // 添加学生
    void addStudent()
    {
        char id[ID_LEN];
        char name[NAME_LEN];
        char gender[GENDER_LEN];
        int age;
        char examType[EXAMTYPE_LEN];

        cin >> id >> name >> gender >> age >> examType;

        Student *newStudent = new Student(id, name, gender, age, examType);

        if (!head)
        {
            head = newStudent;
        }
        else
        {
            Student *temp = head;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = newStudent;
        }
    }

    // 插入学生
    void insertStudent()
    {
        char id[ID_LEN];
        char name[NAME_LEN];
        char gender[GENDER_LEN];
        int age;
        char examType[EXAMTYPE_LEN];

        int position;
        cout << "请输入你要插入的考生的位置：";
        while (true)
        {
            cin >> position;
            if (cin.fail() || position <= 0)
            {
                cout << "无效操作，请重新输入" << endl;
                cin.clear();
                cin.ignore(1024, '\n');
            }
            else
            {
                break;
            }
        }

        cout << "请依次输入要插入考生的考号、姓名、性别、年龄、报考类别！" << endl;
        do
        {
            cin >> id >> name >> gender >> age >> examType;
            if (checkIDExist(id))
            {
                cout << "考号已存在，请重新输入" << endl;
            }
        } while (checkIDExist(id));

        Student *newStudent = new Student(id, name, gender, age, examType);

        // 如果链表为空，直接将新学生作为头节点
        if (!head)
        {
            head = newStudent;
            return;
        }
        // 如果插入位置为1，则在链表头部插入
        if (position == 1)
        {
            newStudent->next = head;
            head = newStudent;
            return;
        }
        // 插入到链表的中间或尾部
        Student *temp = head;
        int currentPos = 1;
        // 找到指定位置的前一个节点
        while (temp->next && currentPos < position - 1)
        {
            temp = temp->next;
            currentPos++;
        }
        // 将新学生插入到链表
        newStudent->next = temp->next;
        temp->next = newStudent;
    }

    // 显示所有学生信息
    void displayStudents() const
    {
        if (!head)
        {
            cout << "暂无考生信息" << endl;
            return;
        }

        cout << endl
             << "考号    姓名    性别    年龄    报考类别" << endl;
        Student *temp = head;
        while (temp)
        {
            printf("%-8s%-8s%-8s%-8d%-8s\n", temp->id, temp->name, temp->gender, temp->age, temp->examType);
            temp = temp->next;
        }
    }

    // 查找学生
    void findStudent() const
    {
        char id[ID_LEN];
        cout << "请输入要查找的考生的考号：";
        cin >> id;

        Student *temp = head;
        while (temp)
        {
            if (strcmp(temp->id, id) == 0)
            {
                cout << "考号    姓名    性别    年龄    报考类别" << endl;
                printf("%-8s%-8s%-8s%-8d%-8s\n", temp->id, temp->name, temp->gender, temp->age, temp->examType);
                return;
            }
            temp = temp->next;
        }
        cout << "未找到该学生" << endl;
    }

    // 删除学生
    void deleteStudent()
    {
        char id[ID_LEN];
        cout << "请输入要删除的考生的考号：";
        cin >> id;

        Student *temp = head;
        Student *prev = nullptr;

        while (temp)
        {
            if (strcmp(temp->id, id) == 0)
            {
                cout << "你删除的考生信息是：";
                printf("%-8s%-8s%-8s%-8d%-8s\n", temp->id, temp->name, temp->gender, temp->age, temp->examType);
                if (prev)
                {
                    prev->next = temp->next;
                }
                else
                {
                    head = temp->next;
                }
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        cout << "未找到该学生" << endl;
    }

    // 更新学生信息
    void updateStudent()
    {
        char id[ID_LEN];
        cout << "输入要修改的考生的考号: ";
        cin >> id;

        Student *temp = head;
        while (temp)
        {
            if (strcmp(temp->id, id) == 0)
            {
                cout << "输入新姓名: ";
                cin >> temp->name;
                cout << "输入新性别: ";
                cin >> temp->gender;
                cout << "输入新年龄: ";
                cin >> temp->age;
                cout << "输入新报考类别: ";
                cin >> temp->examType;
                cout << "学生信息已更新" << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "未找到该学生" << endl;
    }

    // 统计功能
    void analyseStudents() const
    {
        int choice;
        cout << "请选择统计选项：\n1. 按性别统计\n2. 按年龄统计\n3. 按报考类别统计\n4. 按年龄段统计\n5. 统计所有学生总数\n";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            char gender[GENDER_LEN];
            cout << "请输入要统计的性别: ";
            cin >> gender;

            int count = 0;
            Student *temp = head;
            while (temp)
            {
                if (strcmp(temp->gender, gender) == 0)
                {
                    count++;
                }
                temp = temp->next;
            }
            cout << "性别为 " << gender << " 的学生人数为: " << count << endl;
            break;
        }
        case 2:
        {
            int age;
            cout << "请输入要统计的年龄: ";
            cin >> age;

            int count = 0;
            Student *temp = head;
            while (temp)
            {
                if (temp->age == age)
                {
                    count++;
                }
                temp = temp->next;
            }
            cout << "年龄为 " << age << " 的学生人数为: " << count << endl;
            break;
        }
        case 3:
        {
            char examType[EXAMTYPE_LEN];
            cout << "请输入要统计的报考类别: ";
            cin >> examType;

            int count = 0;
            Student *temp = head;
            while (temp)
            {
                if (strcmp(temp->examType, examType) == 0)
                {
                    count++;
                }
                temp = temp->next;
            }
            cout << "报考类别为 " << examType << " 的学生人数为: " << count << endl;
            break;
        }
        case 4:
        {
            int minAge, maxAge;
            cout << "请输入要统计的年龄段（例如20 25）: ";
            cin >> minAge >> maxAge;

            int count = 0;
            Student *temp = head;
            while (temp)
            {
                if (temp->age >= minAge && temp->age <= maxAge)
                {
                    count++;
                }
                temp = temp->next;
            }
            cout << "年龄在 " << minAge << " 到 " << maxAge << " 的学生人数为: " << count << endl;
            break;
        }
        case 5:
        {
            int count = 0;
            Student *temp = head;
            while (temp)
            {
                count++;
                temp = temp->next;
            }
            cout << "学生总人数为: " << count << endl;
            break;
        }
        default:
            cout << "无效选择，请重试" << endl;
            break;
        }
    }
};

int main()
{
    StudentManager manager;
    char input;
    int choice;

    int numStudent;
    cout << "首先请建立考生信息系统！" << endl;
    cout << "请输入考生人数：";
    while (true)
    {
        cin >> numStudent;
        if (cin.fail() || numStudent <= 0)
        {
            cout << "无效操作，请重新输入" << endl;
            cin.clear();
            cin.ignore(1024, '\n');
        }
        else
        {
            break;
        }
    }
    cout << "请输入考号、姓名、性别、年龄、报考类别！" << endl;
    for (int i = 0; i < numStudent; i++)
    {
        manager.addStudent();
    }
    manager.displayStudents();

    cout << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，0为取消操作，Q/q为退出程序）:" << endl;
    do
    {
        cout << "请选择您要进行的操作: ";
        while (true)
        {
            cin >> input;
            if (cin.fail() || input < '0' || input > '5')
            {
                cout << "无效操作，请重新输入" << endl;
                cin.clear();
                cin.ignore(1024, '\n');
            }
            else
            {
                break;
            }
        }

        // 判断是否退出
        if (input == 'Q' || input == 'q')
        {
            cout << "程序已退出。" << endl;
            break;
        }

        choice = input - '0'; // 将输入转为数字

        switch (choice)
        {
        case 1:
            manager.insertStudent();
            manager.displayStudents();
            break;
        case 2:
            manager.deleteStudent();
            manager.displayStudents();
            break;
        case 3:
            manager.findStudent();
            break;
        case 4:
            manager.updateStudent();
            manager.displayStudents();
            break;
        case 5:
            manager.analyseStudents();
            break;
        case 0:
            break;
        default:
            cout << "无效操作，请重新输入" << endl;
            break;
        }
    } while (true);

    return 0;
}

/*
2351050 stu1 F 20 Designer
2350947 stu2 M 21 Developer
2252552 stu3 M 20 Developer
1954444 stu4 F 21 Tester
2337088 stu5 M 18 Designer
*/