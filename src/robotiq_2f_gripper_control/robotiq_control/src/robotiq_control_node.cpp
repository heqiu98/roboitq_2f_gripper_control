/*本节点名为：robotiq_control_node，作用如下
从robotiq_control话题读取需要执行的动作消息，消息格式为std_msgs::String类型，
判断消息内容是否符合要求，然后将其转为outputMsg.Robotiq2FGripper_robot_output，
发送给robotiq_2f_85_gripper执行*/

#include<ros/ros.h>
#include<std_msgs/String.h>
#include<Robotiq2FGripper_robot_output.h>
#include<cctype>

using namespace std;

class GripperController{
public:
    GripperController(){
        
        sub = robotiq_nh.subscribe("robotiq_control", 10, &GripperController::callback, this);

        pub = robotiq_nh.advertise<robotiq_2f_gripper_control::Robotiq2FGripper_robot_output>("Robotiq2FGripperRobotOutput", 10);


    }

    void callback(const std_msgs::String::ConstPtr& msg){

        ROS_INFO("Received gripper control message: %s", msg->data.c_str());
        // 创建输出消息
        
        command = getCommand(msg->data);

        // 发布输出消息到 Robotiq2FGripperRobotOutput 话题
        pub.publish(command);
        
        lastCommand = command;
    }

private:
    ros::NodeHandle robotiq_nh;
    ros::Subscriber sub;
    ros::Publisher pub;
    robotiq_2f_gripper_control::Robotiq2FGripper_robot_output lastCommand;
    robotiq_2f_gripper_control::Robotiq2FGripper_robot_output command;

    robotiq_2f_gripper_control::Robotiq2FGripper_robot_output getCommand(const std::string& msg){
        robotiq_2f_gripper_control::Robotiq2FGripper_robot_output command = lastCommand;
    string commandTemp = msg;

    if (commandTemp == "a"){
        // robotiq_2f_gripper_control::Robotiq2FGripper_robot_output command;
        command.rACT = 1;
        command.rGTO = 1;
        command.rATR = 0;
        command.rPR = 0;
        command.rSP = 255;
        command.rFR = 150;
        cout << " 1 " << endl;
    }

    if (commandTemp == "r"){
        // robotiq_2f_gripper_control::Robotiq2FGripper_robot_output command;
        command.rACT = 0;
        command.rGTO = 0;
        command.rATR = 0;
        command.rPR = 0;
        command.rSP = 0;
        command.rFR = 0;
        cout << " 4 " << endl;
    }

    if (commandTemp == "c"){
        command.rPR = 255;
    }

    if (commandTemp == "o"){
        command.rPR = 0;
    }

    if (isdigit(commandTemp[0])){
    try
    {
        command.rPR = std::stoi(commandTemp);
        if (command.rPR > 255) command.rPR = 255;
        if (command.rPR < 0) command.rPR = 0;
    }catch (const invalid_argument& e) {
        // 如果转换失败，处理异常
        cerr << "Invalid argument: " << e.what() << endl;
    } catch (const out_of_range& e) {
        // 如果转换后的值超出范围，处理异常
        cerr << "Out of range: " << e.what() << endl;
    }
    }

    if (commandTemp == "f"){
        command.rSP += 25;
        if (command.rSP > 255)
            command.rSP = 255;
    }

    if (commandTemp == "l"){
        command.rSP -= 25;
        if (command.rSP < 0)
            command.rSP = 0;
    }
    
    if (commandTemp == "i"){
        command.rFR += 25;
        if (command.rFR > 255)
            command.rFR = 255;
    }

    if (commandTemp == "d"){
        command.rFR -= 25;
        if (command.rFR < 0)
            command.rFR =0;
    }
    return command;
}
};

int main(int argc, char *argv[] ){

    ros::init(argc, argv, "robotiq_2f_85_gripper");
    GripperController controller;
    ros::spin();
    return 0;
}
