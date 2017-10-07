#include <pses_basis/sensorgroupthread.h>

SensorGroupThread::SensorGroupThread(const std::string& grpMessagePrefix, ThreadDispatcher* dispatcher, const std::unordered_map<unsigned char, std::shared_ptr<SensorGroup> >& sensorGroups)
    : dispatcher(dispatcher), grpMessagePrefix(grpMessagePrefix), sensorGroups(sensorGroups)
{
}

void SensorGroupThread::startThread()
{
  // ROS_INFO_STREAM("ReadingThread starting..");
  active = true;
  worker = std::thread(&SensorGroupThread::workerFunction, this);
  // ROS_INFO_STREAM("ReadingThread started..");
}

void SensorGroupThread::stopThread()
{
  // ROS_INFO_STREAM("ReadingThread stopping..");
  active = false;
  wakeUp();
  worker.join();
  // ROS_INFO_STREAM("ReadingThread stopped..");
}

void SensorGroupThread::workerFunction()
{
  while (active)
  {
    sleep();
    // do stuff
    //ROS_INFO_STREAM("grp thread: Wakeup..." << dispatcher->IsMessageQueueEmpty());
    if(dispatcher->IsMessageQueueEmpty()) continue;
    std::string msg;
    dispatcher->dequeueSensorGroupMessage(msg);
    //boost::remove_erase_if(msg, boost::is_any_of(grpMessagePrefix));
    if(msg.size()<1) continue;
    msg.erase(boost::remove_if(msg, boost::is_any_of(grpMessagePrefix)), msg.end());
    boost::trim_left(msg);
    if(msg.size()<1) continue;
    //ROS_INFO_STREAM("grp thread: post erase "<<msg);
    std::vector<std::string> split;
    boost::split(split, msg, boost::is_any_of(" "));
    //ROS_INFO_STREAM("grp thread: post split "<<split[0]<<" "<<split[0].size());
    if(split.size()<2) continue;
    if(split[0].size()<1) continue;
    unsigned char grpNum = 0;
    try{
      //grpNum = static_cast<unsigned char>(std::stoul(split[0]));
      //ROS_INFO_STREAM(std::stoul(split[0]));
      grpNum = std::stoul(split[0]);
    }catch(std::exception& e){
      // do sth.
      //ROS_INFO_STREAM(e.what());
      continue;
    }
    //ROS_INFO_STREAM("Post Split...");
    int startIdx = split[0].size();
    if(sensorGroups.find(grpNum)==sensorGroups.end()) continue;
    msg = msg.substr(startIdx, std::string::npos);
    boost::trim(msg);
    if(msg.size()<1) continue;
    sensorGroups[grpNum]->processResponse(msg);
    //ROS_INFO_STREAM("sent ...");

  }
}
