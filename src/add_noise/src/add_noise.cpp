#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <stdlib.h>

#define pi 3.1415926536

geometry_msgs::Twist t;

//noise_generate function generates additive white Gaussian noise samples with zero mean and a standard deviation of 1
double noise_generate(){
    double temp1,temp2;
    double output;
    
    int p=1;

    while(p>0){
        temp1 = (rand()/(double)RAND_MAX);

        if(temp1==0){
            p=1;
        }
        else{
            p=-1;
        }
    }

    temp2 = cos(2*(double)pi*rand()/(double)RAND_MAX);

    output = sqrt(-2.0*log(temp1))*temp2;

    return output;
}

void add_noise(const geometry_msgs::Twist& msg){
    t.linear.x = msg.linear.x + noise_generate();
    t.angular.z = msg.angular.z + noise_generate();
}


int main(int argc, char* argv[]){
    ros::init(argc, argv, "add_noise_vel");
    ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/cmd_vel",1000,add_noise);
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/velocity_noise_added",1000);
    ros::Rate loop_rate(10);

    int count=0;
    
    while(ros::ok()){
        pub.publish(t);
        ros::spinOnce();
        loop_rate.sleep();
        count++;
    }

    return 0;
}