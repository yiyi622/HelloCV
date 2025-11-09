//订阅pose实时获取海龟发布的消息(即海龟的位置信息)，进而实现闭环控制
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
#include <string>

using namespace std::chrono_literals;
using std::bind;
using std::make_shared;
using std::string;

class TurtleContralNode : public rclcpp::Node
{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriber;
    //定义目标点的位置
    double target_x{1.0};
    double target_y{1.0};
    double k{1.0}; //调节速度的比例系数
    double max_speed{3.0};

public:
    explicit TurtleContralNode(const string &node_name) : Node(node_name)
    {
        publisher = create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        subscriber = create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10,
                                                               bind(&TurtleContralNode::on_pose_received, this, std::placeholders::_1));
        //回调函数里引入了参数，这里也要占一位
    }

    //回调函数
    void on_pose_received(const turtlesim::msg::Pose::SharedPtr pose) //参数：收到数据的指针
    {
        // 1.获取当前位置
        auto current_x = pose->x;
        auto current_y = pose->y;
        RCLCPP_INFO(get_logger(), "当前：x=%f，y=%f", current_x, current_y);

        // 2.计算当前海龟位置与目标位置间的距离和角度差
        auto distance = std::sqrt(
            (target_x - current_x) * (target_x - current_x) +
            (target_y - current_y) * (target_y - current_y));
        //角度差
        auto angle = std::atan2((target_y - current_y), (target_x - current_x)) - pose->theta;

        // 3.控制策略：角度差大了就转，然后直走，走着走着要是角度差大了就再转
        auto msg = geometry_msgs::msg::Twist();
        if (distance > 0.1)
        {
            if (fabs(angle) > 0.2) // fabs()取绝对值
            {
                msg.angular.z = fabs(angle);
            }
            else
            {
                msg.linear.x = k * distance;
            }
        }

        // 4.限制最大速度
        if (msg.linear.x > max_speed)
        {
            msg.linear.x = max_speed;
        }
        publisher->publish(msg);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = make_shared<TurtleContralNode>("turtle_contral");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}