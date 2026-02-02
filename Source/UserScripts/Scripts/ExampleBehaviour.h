#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include <string>
#include "PhysxManager.h"
#include <SimpleMath.h>
#include <MMMTime.h>

using namespace DirectX::SimpleMath;

namespace MMMEngine
{
    class USERSCRIPTS ExampleBehaviour : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ExampleBehaviour()
        {
        REGISTER_BEHAVIOUR_MESSAGE(FixedUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
        
        float basespeed = 1.0f;
        bool isSlow = false;
        float yawRadians = 0.0f;

        float maxYawSpeed = 0.0f;

        //PID����
        float kp = 0.0f;

        //����
        float alignEps;

        void Start();
        void Update();

        //��ġ����
        float turnSpeed = 25.0f;
        bool is_Scoop = false;


        Vector3 m_InputDir;            // Update���� ���� (����ȭ ��/�� �������)

        void FixedUpdate();

        void OnCollisionEnter(MMMEngine::CollisionInfo info);


        Vector3 ComputeDesiredVelocity();
        void ApplyYawFromVelocity(const Vector3& v);

        float WrapPi(float a);

        float curYaw = 0.0f;
        //Vector3 forward{};
        float dt = 0.0f;

    };
}
