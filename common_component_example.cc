/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include "cyber/examples/common_component_example/common_component_example.h"

#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"

#include <iostream> 

using ::apollo::common::Point3D;
using ::apollo::perception::PerceptionObstacles;
using ::apollo::perception::PerceptionObstacle;
using Type = ::apollo::perception::PerceptionObstacle_Type;
using SubType = ::apollo::perception::PerceptionObstacle_SubType;

namespace apollo {

bool CommonComponentSample::Init() {
  AINFO << "Commontest component init";
  return true;
}

struct vecD3
{
  double x;
  double y;
  double z;
};

class ObstacleData {
public:
    void setVelocity(const Point3D& velocity) {
      mVelocity.x = velocity.x();
      mVelocity.y = velocity.y();
      mVelocity.z = velocity.z();
    }
    void setPosition(const Point3D& position) {
      mPosition.x = position.x();
      mPosition.y = position.y();
      mPosition.z = position.z();
    }
    void setSubType(const SubType& type) {mSubType = type;}
    void setType(const Type& type) {mType = type;}
private:
  friend class ObstaclesInformation;
  vecD3 mVelocity = {0.0, 0.0, 0.0,};
  vecD3 mPosition = {0.0, 0.0, 0.0,};
  Type mType = Type::PerceptionObstacle_Type_UNKNOWN;
  SubType mSubType = SubType::PerceptionObstacle_SubType_ST_UNKNOWN;
};

class ObstaclesInformation {
  public:
    ObstaclesInformation() = delete;
    ObstaclesInformation(int size) : mObstacles(size){};
    ObstacleData& operator [](int idx) {return mObstacles[idx];}
    void printInformation() {
      size_t obstacleNumber = mObstacles.size();
      std::cout << "----- [OBSTACLE INFO] -----" << std::endl;
      std::cout << "Found " << obstacleNumber << " obstacles" << std::endl;
      for(size_t i = 0; i < mObstacles.size(); ++i) {
        std::cout << "Obstacle: "  << i << std::endl;
        std::cout << "  - type: " << getTypeString(mObstacles[i].mType) << std::endl;
        std::cout << "  - sub-type: " << getSubTypeString(mObstacles[i].mSubType) << std::endl;
        std::cout << "  - position: " << getVecD3String(mObstacles[i].mPosition) << std::endl;
        std::cout << "  - velocity: " << getVecD3String(mObstacles[i].mVelocity) << std::endl;
      }
    }

  private:
    std::string getSubTypeString(SubType type) {
      if(type == SubType::PerceptionObstacle_SubType_ST_BUS)
        return "BUS";
      if(type == SubType::PerceptionObstacle_SubType_ST_CAR)
        return "CAR";
      if(type == SubType::PerceptionObstacle_SubType_ST_CYCLIST)
        return "CYCLIST";
      if(type == SubType::PerceptionObstacle_SubType_ST_MOTORCYCLIST)
        return "MOTORCYCLIST";
      if(type == SubType::PerceptionObstacle_SubType_ST_PEDESTRIAN)
        return "PEDESTRIAN";
      if(type == SubType::PerceptionObstacle_SubType_ST_TRICYCLIST)
        return "TRICYCLIST";
      if(type == SubType::PerceptionObstacle_SubType_ST_TRUCK)
        return "TRUCK";
      if(type == SubType::PerceptionObstacle_SubType_ST_UNKNOWN_MOVABLE)
        return "UNKNOWN MOVABLE";
      if(type == SubType::PerceptionObstacle_SubType_ST_UNKNOWN_UNMOVABLE)
        return "UNKNOWN UNMOVABLE";
      return "UNKkOWN";
    }

    std::string getTypeString(Type type) {
      if(type == Type::PerceptionObstacle_Type_BICYCLE)
        return "BICYCLE";
      if(type == Type::PerceptionObstacle_Type_PEDESTRIAN)
        return "PEDESTRIAN";
      if(type == Type::PerceptionObstacle_Type_UNKNOWN_MOVABLE)
        return "MOVABLE";
      if(type == Type::PerceptionObstacle_Type_UNKNOWN_UNMOVABLE)
        return "UNMOVABLE";
      if(type == Type::PerceptionObstacle_Type_VEHICLE)
        return "VEHICLE";
      return "UNKNOWN";
    }

    std::string getVecD3String(vecD3 vec) {
      std::string string = "[";
      string += std::to_string(vec.x) + ", ";
      string += std::to_string(vec.y) + ", ";
      string += std::to_string(vec.z) + "]";
      return string;
    }

    std::vector<ObstacleData> mObstacles;
};

bool CommonComponentSample::Proc(const std::shared_ptr<PerceptionObstacles>& msg) {
  //AINFO << "Start common component Proc [" << msg->perception_obstacle.id << "]";
  AINFO << "[TEST EXAMPLE] received message";

  int obstacleNumber = msg->perception_obstacle_size();
  ObstaclesInformation obstacleInfo(obstacleNumber); 
  for(int i = 0; i < obstacleNumber; ++i) {
    const PerceptionObstacle& obstacle = msg->perception_obstacle(i);
    ObstacleData data;
    if(obstacle.has_position())
      data.setPosition(obstacle.position());
    if(obstacle.has_velocity())
      data.setVelocity(obstacle.velocity());
    if(obstacle.has_type())
      data.setType(obstacle.type());
    if(obstacle.has_sub_type())
      data.setSubType(obstacle.sub_type());
    obstacleInfo[i] = data;
  }
  obstacleInfo.printInformation();
  return true;
}
}

