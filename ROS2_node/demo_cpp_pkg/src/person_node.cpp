#include "rclcpp/rclcpp.hpp"
#include <string>
#include <memory>
using std::make_shared;
using std::string;
//大型项目应该避免使用using namespace std 因为引入整个命名空间后，比如说里面有count函数，但我自己也定义一个count函数的话，调用时到底用谁的？

class PersonNode : public rclcpp::Node
//定义子类并公有继承Node父类，公有继承并不是只继承父类的公共部分，而是所有成员都会继承
//但在子类中访问父类成员的权限有变：1.子类不能访问父类的私有部分。2.子类内部才能访问父类保护部分，子类对象不能访问
{
private:
    string name;
    int age;

public:
    /*
    PersonNode(const string &node_name,const string& name_,const int& age_)
    : Node(node_name)
    //调用父类的构造函数，并把常引用进来的node_name参数再传给父类
    {
        this->name = name_;     this-> 先通过this指针找到对象，然后访问它的成员
        //this指针指向的代表成员变量，用于解决命名冲突问题
        //比如name=name，难以区分谁是成员变量谁是参数；
        this->age = age_;
        //而且还用于区分成员变量和局部变量，前面定义类型的int age是局部变量，this指针的是成员变量
    }*/

    //先定义后赋值的办法比较老，还要用到this指针，不如直接写成员初始化列表
    PersonNode(const string &node_name, const string &name_, const int &age_)
        : Node(node_name), name(name_), age(age_){};

    void eat(const string &food_name)
    {
        //使用从父类继承来的功能，打印节点日志
        RCLCPP_INFO(this->get_logger(), "我是%s，%d岁，爱吃%s", name.c_str(), age, food_name.c_str());
        //这里语法是c风格的，因为RCLCPP_INFO底层定义用的是c风格的printf，所以不管怎么弄，字符串也都要用c_str()转成c字符串
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = make_shared<PersonNode>("person_node", "张三", 18);
    RCLCPP_INFO(node->get_logger(), "你好C++节点");
    node->eat("饭");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}