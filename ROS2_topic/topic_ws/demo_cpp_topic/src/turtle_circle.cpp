//发布话题控制海龟画圆
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
//要发布话题来控制小海龟，所以就要通过包含头文件的方式引入小海龟订阅话题的 消息类型(数据结构)
#include <string>
#include <chrono> //时间处理库，提供精确的时间点和时间间隔表示，用来创建计时器

using namespace std::chrono_literals; // chrono_literals是嵌套在std内部的一整个命名空间(namespace)，其中定义了一些字面量，如毫秒(ms)
//再引入两个std命名空间内的具体标识符(所以不用namespace)
using std::make_shared;
using std::string;

class TurtleCircleNode : public rclcpp::Node
{
private:
    // rclcpp类的::发布者<消息类型>::智能指针
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
    // rclcpp类的::定时器::智能指针
    rclcpp::TimerBase::SharedPtr time;

public:
    // explicit(明确的/显式)防止隐式转换，限定只能通过构造函数这一种办法创建对象
    explicit TurtleCircleNode(const string &node_name) : Node(node_name),
                                                         publisher(create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10)), // 10是消息队列容量
                                                         time(create_wall_timer(1000ms, bind(&TurtleCircleNode::timer_callback, this)))
    {
    }
    /*bind函数，将"成员函数"(函数)和"对象"(参数)绑定，创建可调用的回调函数包装器。
    因为成员函数不能像普通函数那样直接作为回调函数使用，而必须通过对象来调用。
    所以将成员函数和对象绑定在一起，形成一个可调用对象，这个可调用对象可以像普通函数那样被调用。
    语法: bind(成员函数指针, 对象指针, 参数1, 参数2, ...)
    */

    //定时器的回调函数
    void timer_callback()
    {
        //调用消息类型的构造函数，创建Twist类型的消息
        auto msg = geometry_msgs::msg::Twist();
        //设置线速度和角速度
        msg.linear.x = 1.0;
        msg.angular.z = 0.5;
        //消息设置好了还要发布出去
        publisher->publish(msg);
    }
};

int main(int argc, char *argv[]) //跟char** argv完全等价
{
    rclcpp::init(argc, argv);
    auto node = make_shared<TurtleCircleNode>("turtle_circle");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}