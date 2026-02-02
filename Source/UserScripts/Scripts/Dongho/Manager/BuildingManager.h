#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"
#include "StaticMesh.h"

namespace MMMEngine
{
    class USERSCRIPTS BuildingManager : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        BuildingManager()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Start();

        void Update();
        void MakeBuilding(const DirectX::SimpleMath::Vector3 pos);
        void LevelUpHP(ObjPtr<GameObject> obj);
        void LevelUpBuff(ObjPtr<GameObject> obj);
        void LevelUpDeBuff(ObjPtr<GameObject> obj);
        void LevelUpSnow(ObjPtr<GameObject> obj);
        void BuildingReturn();
        DirectX::SimpleMath::Vector3 buildingscale{0.5f,0.5f,0.5f};
        ResPtr<StaticMesh> buildingmesh;
        ResPtr<StaticMesh> HPbuildingmesh;
        ResPtr<StaticMesh> buffbuildingmesh;
        ResPtr<StaticMesh> debuffbuildingmesh;
        ResPtr<StaticMesh> snowbuildingmesh;
        std::vector<ObjPtr<GameObject>> Buildings;
        static ObjPtr<BuildingManager> instance;
    };
}
