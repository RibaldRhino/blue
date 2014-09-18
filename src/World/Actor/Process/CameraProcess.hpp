#pragma once

#include <boost/signals2/connection.hpp>
#include "../Camera.hpp"
#include "../../../Process/AbstractProcess.hpp"


class CameraProcess : public AbstractProcess
{
private:
    bool _accelerationOn;
    bool _breakOn;
    bool _steerRight;
    bool _steerLeft;
    glm::highp_vec2 _mouseMove;
public:
    CameraProcess(Camera* camera) : AbstractProcess(), _camera(camera) {}
    void VInit() override;
    void VUpdate(double deltaTime) override;
    void VEnd() override;

    Camera *_camera;
    boost::signals2::connection _connection;
    glm::highp_vec2 _prevPos;
    bool _pitch_up;
    bool _pitch_down;
};