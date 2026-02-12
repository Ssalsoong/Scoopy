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
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }

        void Awake();

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
            Vector3{-5.5f,0.f,-5.5f},
            Vector3{0.f,0.f,-5.5f},
            Vector3{5.5f,0.f,-5.5f},
            Vector3{5.5f,0.f,0.f},
            Vector3{-5.5f,0.f,5.5f},
            Vector3{0.f,0.f,5.5f},
            Vector3{5.5f,0.f,5.5f},
            Vector3{-5.5f,0.f,0.f}
        };
        DirectX::SimpleMath::Vector3 buildingscale{15.f,15.f,15.f};
        ResPtr<StaticMesh> buildingmesh;
        ResPtr<StaticMesh> HPbuildingmesh;
        ResPtr<StaticMesh> buffbuildingmesh;
        ResPtr<StaticMesh> debuffbuildingmesh;
        ResPtr<StaticMesh> snowbuildingmesh;
        std::vector<ObjPtr<GameObject>> buildingpoints;
        std::vector<ObjPtr<GameObject>> Buildings;

		int GetBuildingCount() const {
			return static_cast<int>(Buildings.size());
		}
        void BuildingsDistUP();
        bool distup = false;

        USCRIPT_PROPERTY()
        ResPtr<Prefab> pre_building;

		USCRIPT_PROPERTY()
		ResPtr<Prefab> mBuildingPoint;

        static ObjPtr<BuildingManager> instance;

        USCRIPT_PROPERTY()
        float baseRadius = 1.0f;
    };
}
