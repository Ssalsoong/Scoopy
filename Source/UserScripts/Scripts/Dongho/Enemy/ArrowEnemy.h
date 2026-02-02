#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "StaticMesh.h"

namespace MMMEngine
{
    class USERSCRIPTS ArrowEnemy : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ArrowEnemy()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();
        void Update();
		int HP = 10;
		int atk = 4;
		float velocity = 13.0f;
		float attackDelay = 1.0f;
		float battledist = 10.0f;
		float checkdist = 12.0f;
        std::queue<ObjPtr<GameObject>> Arrows;
        ResPtr<StaticMesh> arrowmesh;
        ObjPtr<GameObject> target;
        void ApplyStats();
        void ArrowAttack(ObjPtr<GameObject> target);
        void ReturnArrow(ObjPtr<GameObject> obj);
    };
}
