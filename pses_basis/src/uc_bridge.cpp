#include <ros/ros.h>
#include <pses_basis/servicefunctions.h>
#include <pses_basis/communication.h>
#include <pses_basis/communicationconfig.h>
#include <ros/package.h>
#include <std_msgs/builtin_uint8.h>
#include <std_msgs/builtin_uint16.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/BatteryState.h>

void publishSensorGroupMessage1(
    SensorGroup* grp, std::unordered_map<std::string, ros::Publisher*>& pub)
{
  // do stuff
  // ROS_INFO_STREAM("doing stuff");
  sensor_msgs::Range usl, usr, usf;
  unsigned short l, r, f;
  try{
    grp->getChannelValue("USL", l);
    grp->getChannelValue("USF", f);
    grp->getChannelValue("USR", r);
    usl.range = l;
    usl.max_range = 1;
    //ROS_INFO_STREAM(usl);
    pub["USL"]->publish(usl);
    usf.range = f;
    usf.max_range = 2;
    //ROS_INFO_STREAM(usf);
    pub["USF"]->publish(usf);
    usr.range = r;
    usr.max_range = 3;
    //ROS_INFO_STREAM(usr);
    pub["USR"]->publish(usr);
  }catch(std::exception& e){
    ROS_INFO_STREAM("Sensor grp1: "<<e.what());
  }
}

void publishSensorGroupMessage2(
    SensorGroup* grp, std::unordered_map<std::string, ros::Publisher*>& pub)
{
  // do stuff
  // ROS_INFO_STREAM("doing stuff");
  sensor_msgs::Imu imu;
  short gx, gy, gz, ax, ay, az;
  try{
    grp->getChannelValue("GX", gx);
    grp->getChannelValue("GY", gy);
    grp->getChannelValue("GZ", gz);
    imu.angular_velocity.x = gx;
    imu.angular_velocity.y = gy;
    imu.angular_velocity.z = gz;
    //ROS_INFO_STREAM(imu);
    pub["IMU"]->publish(imu);
  }catch(std::exception& e){
    ROS_INFO_STREAM("Sensor grp2: "<<e.what());
  }

}

void publishSensorGroupMessage3(
    SensorGroup* grp, std::unordered_map<std::string, ros::Publisher*>& pub)
{
  std_msgs::UInt8 hallcnt;
  std_msgs::UInt16 halldt, halldt8;
  try{
    grp->getChannelValue("HALL_CNT", hallcnt.data);
    grp->getChannelValue("HALL_DT", halldt.data);
    grp->getChannelValue("HALL_DT8", halldt8.data);
    //ROS_INFO_STREAM(hallcnt);
    //ROS_INFO_STREAM(halldt);
    //ROS_INFO_STREAM(halldt8);
    pub["HALL_CNT"]->publish(hallcnt);
    pub["HALL_DT"]->publish(halldt);
    pub["HALL_DT8"]->publish(halldt8);
  }catch(std::exception& e){
    ROS_INFO_STREAM("Sensor grp3: "<<e.what());
  }
}

void publishSensorGroupMessage4(
    SensorGroup* grp, std::unordered_map<std::string, ros::Publisher*>& pub)
{
  sensor_msgs::MagneticField mag;
  short mx, my, mz;
  try{
    grp->getChannelValue("MX",mx);
    grp->getChannelValue("MY",my);
    grp->getChannelValue("MZ",mz);
    mag.magnetic_field.x = mx;
    mag.magnetic_field.y = my;
    mag.magnetic_field.z = mz;
    //ROS_INFO_STREAM(mag);
    pub["MAG"]->publish(mag);
  }catch(std::exception& e){
    ROS_INFO_STREAM("Sensor grp4: "<<e.what());
  }

}

void publishSensorGroupMessage5(
    SensorGroup* grp, std::unordered_map<std::string, ros::Publisher*>& pub)
{
  sensor_msgs::BatteryState batVD, batVS;
  unsigned short vsbat, vdbat;
  try{
    grp->getChannelValue("VDBAT", vdbat);
    grp->getChannelValue("VSBAT", vsbat);
    batVD.voltage=vdbat;
    batVS.voltage=vsbat;
    //ROS_INFO_STREAM(batVD);
    //ROS_INFO_STREAM(batVS);
    pub["VDBAT"]->publish(batVD);
    pub["VSBAT"]->publish(batVS);
  }catch(std::exception& e){
    ROS_INFO_STREAM("Sensor grp5: "<<e.what());
  }

}

int main(int argc, char** argv)
{
  // set up ros node handle
  ros::init(argc, argv, "uc_bridge");
  ros::NodeHandle nh;
  // load communication config files and init communication
  std::string typesPath = ros::package::getPath("pses_basis") + "/data/";
  Communication com(typesPath);
  // create sensor group publish services
  ros::Publisher grp11 = nh.advertise<sensor_msgs::Range>("USL", 10);
  ros::Publisher grp12 = nh.advertise<sensor_msgs::Range>("USF", 10);
  ros::Publisher grp13 = nh.advertise<sensor_msgs::Range>("USR", 10);
  ros::Publisher grp2 = nh.advertise<sensor_msgs::Imu>("IMU", 10);
  ros::Publisher grp31 = nh.advertise<std_msgs::UInt8>("HALL_CNT", 10);
  ros::Publisher grp32 = nh.advertise<std_msgs::UInt16>("HALL_DT", 10);
  ros::Publisher grp33 = nh.advertise<std_msgs::UInt16>("HALL_DT8", 10);
  ros::Publisher grp4 = nh.advertise<sensor_msgs::MagneticField>("MAG", 10);
  ros::Publisher grp51 = nh.advertise<sensor_msgs::BatteryState>("VDBAT", 10);
  ros::Publisher grp52 = nh.advertise<sensor_msgs::BatteryState>("VSBAT", 10);
  // group publish services by putting them into a map
  //std::unordered_map<std::string, ros::Publisher*> usGrp;
  std::unordered_map<std::string, ros::Publisher*> imuGrp;
  //std::unordered_map<std::string, ros::Publisher*> hallGrp;
  //std::unordered_map<std::string, ros::Publisher*> magGrp;
  //std::unordered_map<std::string, ros::Publisher*> batGrp;
  //usGrp.insert(std::make_pair("USL", &grp11));
  //usGrp.insert(std::make_pair("USF", &grp12));
  //usGrp.insert(std::make_pair("USR", &grp13));
  imuGrp.insert(std::make_pair("IMU", &grp2));
  //hallGrp.insert(std::make_pair("HALL_CNT", &grp31));
  //hallGrp.insert(std::make_pair("HALL_DT", &grp32));
  //hallGrp.insert(std::make_pair("HALL_DT8", &grp33));
  //magGrp.insert(std::make_pair("MAG", &grp4));
  //batGrp.insert(std::make_pair("VDBAT", &grp51));
  //batGrp.insert(std::make_pair("VSBAT", &grp52));

  // register publish callbacks with the uc-board communication framework
  //com.registerSensorGroupCallback(
  //    1, boost::bind(&publishSensorGroupMessage1, _1, usGrp));
  com.registerSensorGroupCallback(
      2, boost::bind(&publishSensorGroupMessage2, _1, imuGrp));
  /*
  com.registerSensorGroupCallback(
      3, boost::bind(&publishSensorGroupMessage3, _1, hallGrp));
  com.registerSensorGroupCallback(
      4, boost::bind(&publishSensorGroupMessage4, _1, magGrp));
  com.registerSensorGroupCallback(
      5, boost::bind(&publishSensorGroupMessage5, _1, batGrp));
  */
  // Test area
  //com.registerSensorGroups("Set Group");
  // end

  // register uc-board communication services with ros
  ros::ServiceServer deleteGroupService =
      nh.advertiseService<pses_basis::DeleteGroup::Request,
                          pses_basis::DeleteGroup::Response>(
          "delete_group", std::bind(ServiceFunctions::deleteGroup, std::placeholders::_1,
                                    std::placeholders::_2, &com));

  ros::ServiceServer getControllerIDService =
      nh.advertiseService<pses_basis::GetControllerID::Request,
                          pses_basis::GetControllerID::Response>(
          "get_controller_id", std::bind(ServiceFunctions::getControllerID, std::placeholders::_1,
                                         std::placeholders::_2, &com));

  ros::ServiceServer getDAQStatusService =
      nh.advertiseService<pses_basis::GetDAQStatus::Request,
                          pses_basis::GetDAQStatus::Response>(
          "get_daq_status", std::bind(ServiceFunctions::getDAQStatus, std::placeholders::_1,
                                      std::placeholders::_2, &com));

  ros::ServiceServer getFirmwareVersionService =
      nh.advertiseService<pses_basis::GetFirmwareVersion::Request,
                          pses_basis::GetFirmwareVersion::Response>(
          "get_firmware_version",
          std::bind(ServiceFunctions::getFirmwareVersion, std::placeholders::_1,
                    std::placeholders::_2, &com));

  ros::ServiceServer getInfoAllGroupsService =
      nh.advertiseService<pses_basis::GetInfoAllGroups::Request,
                          pses_basis::GetInfoAllGroups::Response>(
          "get_info_all_groups",
          std::bind(ServiceFunctions::getInfoAllGroups, std::placeholders::_1,
                    std::placeholders::_2, &com));

  ros::ServiceServer getInfoGroupService =
      nh.advertiseService<pses_basis::GetInfoGroup::Request,
                          pses_basis::GetInfoGroup::Response>(
          "get_info_group", std::bind(ServiceFunctions::getInfoGroup, std::placeholders::_1,
                                      std::placeholders::_2, &com));

  ros::ServiceServer getKinectStatusService =
      nh.advertiseService<pses_basis::GetKinectStatus::Request,
                          pses_basis::GetKinectStatus::Response>(
          "get_kinect_status", std::bind(ServiceFunctions::getKinectStatus, std::placeholders::_1,
                                         std::placeholders::_2, &com));

  ros::ServiceServer getMotorLevelService =
      nh.advertiseService<pses_basis::GetMotorLevel::Request,
                          pses_basis::GetMotorLevel::Response>(
          "get_motor_level", std::bind(ServiceFunctions::getMotorLevel, std::placeholders::_1,
                                       std::placeholders::_2, &com));

  ros::ServiceServer getSessionIDService =
      nh.advertiseService<pses_basis::GetSessionID::Request,
                          pses_basis::GetSessionID::Response>(
          "get_session_id", std::bind(ServiceFunctions::getSessionID, std::placeholders::_1,
                                      std::placeholders::_2, &com));

  ros::ServiceServer getSteeringLevelService =
      nh.advertiseService<pses_basis::GetSteeringLevel::Request,
                          pses_basis::GetSteeringLevel::Response>(
          "get_steering_level",
          std::bind(ServiceFunctions::getSteeringLevel, std::placeholders::_1,
                    std::placeholders::_2, &com));

  ros::ServiceServer resetControllerService =
      nh.advertiseService<pses_basis::ResetController::Request,
                          pses_basis::ResetController::Response>(
          "reset_controller", std::bind(ServiceFunctions::resetController, std::placeholders::_1,
                                        std::placeholders::_2, &com));

  ros::ServiceServer setMotorLevelService =
      nh.advertiseService<pses_basis::SetMotorLevel::Request,
                          pses_basis::SetMotorLevel::Response>(
          "set_motor_level", std::bind(ServiceFunctions::setMotorLevel, std::placeholders::_1,
                                       std::placeholders::_2, &com));

  ros::ServiceServer setSessionIDService =
      nh.advertiseService<pses_basis::SetSessionID::Request,
                          pses_basis::SetSessionID::Response>(
          "set_session_id", std::bind(ServiceFunctions::setSessionID, std::placeholders::_1,
                                      std::placeholders::_2, &com));

  ros::ServiceServer setSteeringLevelService =
      nh.advertiseService<pses_basis::SetSteering::Request,
                          pses_basis::SetSteering::Response>(
          "set_steering_level",
          std::bind(ServiceFunctions::setSteeringLevel, std::placeholders::_1,
                    std::placeholders::_2, &com));

  ros::ServiceServer toggleBrakesService =
      nh.advertiseService<pses_basis::ToggleBrakes::Request,
                          pses_basis::ToggleBrakes::Response>(
          "toggle_brakes", std::bind(ServiceFunctions::toggleBrakes, std::placeholders::_1,
                                     std::placeholders::_2, &com));

  ros::ServiceServer toggleDAQService =
      nh.advertiseService<pses_basis::ToggleDAQ::Request,
                          pses_basis::ToggleDAQ::Response>(
          "toggle_daq", std::bind(ServiceFunctions::toggleDAQ, std::placeholders::_1,
                                  std::placeholders::_2, &com));

  ros::ServiceServer toggleGroupService =
      nh.advertiseService<pses_basis::ToggleGroup::Request,
                          pses_basis::ToggleGroup::Response>(
          "toggle_group", std::bind(ServiceFunctions::toggleGroup, std::placeholders::_1,
                                    std::placeholders::_2, &com));

  ros::ServiceServer toggleKinectService =
      nh.advertiseService<pses_basis::ToggleKinect::Request,
                          pses_basis::ToggleKinect::Response>(
          "toggle_kinect", std::bind(ServiceFunctions::toggleKinect, std::placeholders::_1,
                                     std::placeholders::_2, &com));

  ros::ServiceServer toggleMotorService =
      nh.advertiseService<pses_basis::ToggleMotor::Request,
                          pses_basis::ToggleMotor::Response>(
          "toggle_motor", std::bind(ServiceFunctions::toggleMotor, std::placeholders::_1,
                                    std::placeholders::_2, &com));

  // start serial communication
  try
  {
    com.connect();
    com.startCommunication();
    ros::Duration(0.10).sleep();
    // register sensor groups on the uc-board
    bool success = com.registerSensorGroups("Set Group");
    ROS_INFO_STREAM("Success? "<<success);
    //ros::Duration(7.00).sleep();
    //com.stopCommunication();
    //com.disconnect();
  }
  catch (std::exception& e)
  {
    ROS_ERROR("%s", e.what());
  }

  ros::spin();
  return 0;
}
