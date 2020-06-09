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

namespace apollo {

bool CommonComponentSample::Init() {
  AINFO << "Commontest component init";
  return true;
}

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

void ObstacleData::setVelocity(const Point3D& velocity) {
  mVelocity.x = velocity.x();
  mVelocity.y = velocity.y();
  mVelocity.z = velocity.z();
}

void ObstacleData::setPosition(const Point3D& position) {
  mPosition.x = position.x();
  mPosition.y = position.y();
  mPosition.z = position.z();
}

void ObstaclesInformation::printInformation() {
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

std::string ObstaclesInformation::getSubTypeString(SubType type) {
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

std::string ObstaclesInformation::getTypeString(Type type) {
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

std::string ObstaclesInformation::getVecD3String(vecD3 vec) {
  std::string string = "[";
  string += std::to_string(vec.x) + ", ";
  string += std::to_string(vec.y) + ", ";
  string += std::to_string(vec.z) + "]";
  return string;
}
}

