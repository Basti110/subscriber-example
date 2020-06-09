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
#include <memory>

#include "cyber/class_loader/class_loader.h"
#include "cyber/component/component.h"
#include "modules/perception/proto/perception_obstacle.pb.h"

namespace apollo {

using ::apollo::common::Point3D;
using ::apollo::perception::PerceptionObstacles;
using ::apollo::perception::PerceptionObstacle;
using Type = ::apollo::perception::PerceptionObstacle_Type;
using SubType = ::apollo::perception::PerceptionObstacle_SubType;
using apollo::cyber::Component;
using apollo::cyber::ComponentBase;

struct vecD3
{
    double x;
    double y;
    double z;
};

class ObstacleData {
public:
    void setVelocity(const Point3D& velocity);
    void setPosition(const Point3D& position);
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
    void printInformation();

private:
    std::string getSubTypeString(SubType type);
    std::string getTypeString(Type type);
    std::string getVecD3String(vecD3 vec);

    std::vector<ObstacleData> mObstacles;
};

class CommonComponentSample : public Component<perception::PerceptionObstacles> {
public:
    bool Init() override;
    bool Proc(const std::shared_ptr<perception::PerceptionObstacles>& msg) override;
};
CYBER_REGISTER_COMPONENT(CommonComponentSample)
}
