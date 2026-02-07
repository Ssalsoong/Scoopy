#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "SimpleMath.h"
#include "StaticMesh.h"
#include <array>
#include "Prefab.h"

using DirectX::SimpleMath::Vector3;
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
        void Build(ObjPtr<GameObject> obj);
        void LevelUpHP(ObjPtr<GameObject> obj);
        void LevelUpBuff(ObjPtr<GameObject> obj);
        void LevelUpDeBuff(ObjPtr<GameObject> obj);
        void LevelUpSnow(ObjPtr<GameObject> obj);
        void BuildingReturn();
        static constexpr int BuildingPointCount = 8;
        std::array<Vector3, BuildingPointCount> BuildingPos =
        {
            Vector3{-11.5f,0.f,-11.5f},
            Vector3{-0.5f,0.f,-9.5f},
            Vector3{11.5f,0.f,-11.5f},
            Vector3{9.5f,0.f,-0.5f},
            Vector3{11.5f,0.f,11.5f},
            Vector3{0.5f,0.f,9.5f},
            Vector3{-11.5f,0.f,11.5f},
            Vector3{-9.5f,0.f,0.5f}
        };
        DirectX::SimpleMath::Vector3 buildingscale{15.f,15.f,15.f};
        ResPtr<StaticMesh> buildingmesh;
        ResPtr<StaticMesh> HPbuildingmesh;
        ResPtr<StaticMesh> buffbuildingmesh;
        ResPtr<StaticMesh> debuffbuildingmesh;
        ResPtr<StaticMesh> snowbuildingmesh;
        std::vector<ObjPtr<GameObject>> buildingpoints;
        std::vector<ObjPtr<GameObject>> Buildings;

        ResPtr<Prefab> pre_building;

        static ObjPtr<BuildingManager> instance;
    };
}
