#include "rclcpp/rclcpp.hpp"

using namespace std;
//当程序需要接收命令行参数时，main函数需要带参数，要是写在函数体内就无法获取命令行参数了
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv); //初始化
    //先创建这个，后续再加一些参数，运行程序时参数会被rclcpp读取和解析，想要修改节点的参数和功能时就能通过argc，argv来修改
    auto node = make_shared<rclcpp::Node>("cpp_node"); //创建对象：括号中按照类构造函数输入对象的实参
    // make_shared是智能指针，用于解决new，delete容易内存泄漏的问题
    /*
    这一行代码既定义了node指针，又同时创建了一个匿名的Node类对象（没有独立的对象名，意味着不能直接访问）
    并让指针指向这个匿名对象，我们只能用指针来访问对象
    故意这样设计，让node看起来像对象一样，尽管它本质是指针，所以要改变一下想法："auto node 代表一个Node对象，由智能指针管理"
    */
    RCLCPP_INFO(node->get_logger(), "你好c++节点");
    //打印日志
    rclcpp::spin(node); //程序启动后，spin会不断循环检测，处理所有node相关事件
    rclcpp::shutdown(); //关闭
    return 0;
}