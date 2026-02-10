#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class Prefab;
    class RectTransform;
    class Transform;

    enum class TrackerType
    {
        Enemy,
        Building
    };

    struct TrackerUISet
    {
        ObjPtr<Transform> target;
        ObjPtr<RectTransform> trackerUI;
        TrackerType type;
    };

    class USERSCRIPTS MiniMap : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
            RTTR_REGISTRATION_FRIEND

            std::unordered_map<Utility::MUID, TrackerUISet, Utility::MUID::Hash> m_trackerUICached;
        std::vector<ObjPtr<RectTransform>> m_buildingTrackerUIPool;
        std::vector<ObjPtr<RectTransform>> m_enemyTrackerUIPool;

        // 풀에서 UI 가져오기
        ObjPtr<RectTransform> GetTrackerUIFromPool(TrackerType type);
        // 풀로 UI 반환
        void ReturnTrackerUIToPool(ObjPtr<RectTransform> ui, TrackerType type);

    public:
        MiniMap()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(LateUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);

        }

        static ObjPtr<MiniMap> Instance;

        USCRIPT_MESSAGE()
            void Awake();

        USCRIPT_MESSAGE()
            void OnDestroy();

        USCRIPT_MESSAGE()
            void LateUpdate();

        // 외부에서 호출할 등록/해제 함수
        void RegisterTracker(const Utility::MUID& id, ObjPtr<Transform> target, TrackerType type);
        void UnregisterTracker(const Utility::MUID& id);

        USCRIPT_PROPERTY()
            float MinimapUnitScale = 10.0f;

        USCRIPT_PROPERTY()
            ObjPtr<Transform> Player;

        USCRIPT_PROPERTY()
            ObjPtr<RectTransform> PlayerRectUI;

        USCRIPT_PROPERTY()
            ResPtr<Prefab> EnemyTrackerUI;

        USCRIPT_PROPERTY()
            ResPtr<Prefab> BuildingTrackerUI;

        USCRIPT_PROPERTY()
            int InitialPoolSize = 20; // 초기 풀 크기
    };
}