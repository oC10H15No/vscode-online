/***
The player profile of a chess game contains name, login times, played matches, won
matches, won ratio, rank(beginner, professional, expert). Implement a class named User_Prof
with following operations: login, logout, test equivalence, test not equivalence. Its constructor
should provide default player’s rank and default name “guest”. There may be multiple “guest”
users at one time, never to mix up their login sessions.
 */

#include <iostream>
#include <string>

enum Rank
{
    beginner,
    professional,
    expert
};

class User_Prof
{
private:
    std::string name;   // 玩家名称
    int login_times;    // 登录次数
    int played_matches; // 已玩比赛数
    int won_matches;    // 赢得比赛数
    double won_ratio;   // 胜率
    Rank rank;          // 玩家等级

    static int guest_count;                           // 静态成员，用于区分不同的 guest 用户

    // 更新胜率
    void updateWonRatio()
    {
        if (played_matches > 0)
        {
            won_ratio = static_cast<double>(won_matches) / played_matches;
        }
        else
        {
            won_ratio = 0.0;
        }
    }

public:
    // 默认构造函数，提供默认姓名 "guest" 和默认等级 beginner
    User_Prof() : login_times(0), played_matches(0), won_matches(0), won_ratio(0.0), rank(beginner)
    {
        name = "guest" + std::to_string(guest_count);
        ++guest_count;
    }
    // 带参构造函数，指定姓名和等级
    User_Prof(const std::string &name, Rank rank) : login_times(0), played_matches(0), won_matches(0), won_ratio(0.0), rank(rank)
    {
        this->name = name;
        this->rank = rank;
    }
    // 登录
    void login()
    {
        ++login_times;
        std::cout << "User " << name << " logged in." << std::endl;
    }
    // 登出
    void logout()
    {
        std::cout << "User " << name << " logged out." << std::endl;
    }
    // 进行比赛
    void playMatch(bool won)
    {
        played_matches++;
        if (won)
        {
            won_matches++;
        }
        updateWonRatio();
    }
    // 重载 == 运算符，比较两个用户是否一样（根据用户名）
    bool operator==(const User_Prof &rhs) const
    {
        return name == rhs.name;
    }
    // 重载 != 运算符
    bool operator!=(const User_Prof &rhs) const
    {
        return !(*this == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const User_Prof& user)
    {
        os << "User name: " << user.name << std::endl;
        os << "Login times: " << user.login_times << std::endl;
        os << "Played matches: " << user.played_matches << std::endl;
        os << "Won matches: " << user.won_matches << std::endl;
        os << "Won ratio: " << user.won_ratio * 100 << '%' << std::endl;
        os << "Rank: " << user.rank << std::endl;
        switch (user.rank)
        {
        case beginner:
            os << "Beginner" << std::endl;
            break;
        case professional:
            os << "Professional" << std::endl;
            break;
        case expert:
            os << "Expert" << std::endl;
            break;
        default:
            break;
        }
        return os;
    }
};

// 初始化静态成员变量
int User_Prof::guest_count = 0;

int main()
{
    // 测试构造函数
    User_Prof anonymous("anonymous", professional);
    anonymous.login();
    std::cout << anonymous;
    anonymous.playMatch(true);
    std::cout << anonymous;

    // 测试默认构造函数
    User_Prof guest1;
    guest1.login();
    guest1.playMatch(false);
    guest1.logout();
    User_Prof guest2;
    guest2.login();
    guest2.playMatch(true);
    guest2.playMatch(true);
    guest2.playMatch(false);
    guest2.logout();
    guest1.login();
    guest1.playMatch(true);
    guest1.logout();
    // 测试相等性
    std::cout << "Test equivalence: " << (guest1 == guest2) << std::endl;
    std::cout << "Test not equivalence: " << (guest1 != guest2) << std::endl;
    std::cout << guest1;
    std::cout << guest2;

    User_Prof Zhu("Zhu", expert);
    Zhu.login();
    Zhu.playMatch(false);
    Zhu.playMatch(true);
    Zhu.playMatch(true);
    Zhu.playMatch(false);
    Zhu.playMatch(true);
    std::cout << Zhu;
    Zhu.logout();

    return 0;
}