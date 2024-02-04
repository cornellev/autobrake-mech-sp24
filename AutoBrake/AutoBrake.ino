#include <ros.h>
#include <std_msgs/Int32.h>
#include <geometry_msgs/Point.h>
#define ena_plus_vertical 22
#define dir_plus_vertical 10
#define pul_plus_vertical 7
int32_t desired_position_vertical = 0;
int32_t current_position_vertical = 0;
int32_t difference_vertical = 0;
int counter=0;
ros::NodeHandle nh;
void command_vertical_callback(const std_msgs::Int32& command_msg);
void setVerticalMotion();
geometry_msgs::Point state_msg;
ros::Subscriber<std_msgs::Int32> command_sub_vertical("motion_command_vertical", &command_vertical_callback);
ros::Publisher state_pub("arduino_state", &state_msg);
void command_vertical_callback(const std_msgs::Int32& command_msg) {
  desired_position_vertical = command_msg.data;
  difference_vertical = desired_position_vertical - current_position_vertical;
}
void setVerticalMotion() {
//  digitalWrite(relay_pin, HIGH);
//  delay(100);
//  while (difference_vertical != 0) {
    if (difference_vertical > 0) {
      digitalWrite(dir_plus_vertical, LOW);
      digitalWrite(pul_plus_vertical, HIGH);
      delayMicroseconds(200); // pulse delay
      digitalWrite(pul_plus_vertical, LOW);
      delayMicroseconds(200);
      current_position_vertical++;
    }
    else if (difference_vertical < 0) {
      digitalWrite(dir_plus_vertical, HIGH);
      digitalWrite(pul_plus_vertical, HIGH);
      delayMicroseconds(200); // pulse delay
      digitalWrite(pul_plus_vertical, LOW);
      delayMicroseconds(200);
      current_position_vertical--;
    }
    difference_vertical = desired_position_vertical - current_position_vertical;
  // Deactivate relay after vertical motion
//  digitalWrite(relay_pin, LOW);
//  delay(100);
//  vertical_motion_active = false;
}
void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8,LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9,LOW);
  pinMode(pul_plus_vertical, OUTPUT);
  pinMode(dir_plus_vertical, OUTPUT);
  pinMode(ena_plus_vertical, OUTPUT);
//  pinMode(relay_pin, OUTPUT);  // Set relay pin as OUTPUT
  nh.initNode();
  nh.subscribe(command_sub_vertical);
  nh.advertise(state_pub);
}
void loop() {
//  setHorizontalMotion();
  setVerticalMotion();
  counter=counter+1;
  // Publish Arduino state
  if(counter%100==0)
  {
    state_msg.x = 0;
    state_msg.y = current_position_vertical;
    state_msg.z = (difference_vertical != 0) ? 1 : 0; // 1 if working, 0 if idle
    state_pub.publish(&state_msg);
  // Spin the rosserial loop.
    nh.spinOnce();
    counter=0;
  }
}