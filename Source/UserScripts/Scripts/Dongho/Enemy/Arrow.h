#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"

namespace MMMEngine
{
    class USERSCRIPTS Arrow : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        Arrow()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();

        void Update();
        int atk = 4;
        float speed = 25.0f;
        float hitRadius = 0.25f; //나중에 제거
        ObjPtr<GameObject> owner;
        ObjPtr<GameObject> target;
        DirectX::SimpleMath::Vector3 targetpos;
        void SetOwner(ObjPtr<GameObject> obj) { owner = obj; }
        void SetTarget(ObjPtr<GameObject> obj) { target = obj; }
    };
}
