#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"
#include "RectTransform.h"
#include "Graphic.h"

namespace MMMEngine
{
    class USERSCRIPTS WorldSpaceUI : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
		float m_lastDistance = 0.0f;   
        bool m_isTargetInScreen = false;
    public:
        WorldSpaceUI()
        {
        REGISTER_BEHAVIOUR_MESSAGE(Awake);
        REGISTER_BEHAVIOUR_MESSAGE(LateUpdate);
        REGISTER_BEHAVIOUR_MESSAGE(OnDestroy);
        REGISTER_BEHAVIOUR_MESSAGE(Start);

        }

        float GetLastDistance() const { return m_lastDistance; }
        void SetUIRenderOrder(int order) { if (m_rectGraphic.IsValid()) m_rectGraphic->SetRenderOrder(order); }

        bool IsTargetInScreen();

        USCRIPT_MESSAGE()
        void Awake();

        USCRIPT_MESSAGE()
        void Start();

        USCRIPT_PROPERTY()
        ObjPtr<RectTransform> SelfRect;  

		USCRIPT_PROPERTY()
        ObjPtr<Transform> TargetTransform;

        USCRIPT_MESSAGE()
        void LateUpdate();

        USCRIPT_MESSAGE()
        void OnDestroy();

        USCRIPT_PROPERTY()
		bool DontSortByDistance = false;

        USCRIPT_PROPERTY()
        bool ClampToScreenEdge = false;

        USCRIPT_PROPERTY()
        float EdgeYOffset = 25.0f;

        USCRIPT_PROPERTY()
        float EdgeXOffset = 25.0f;

        USCRIPT_PROPERTY()
        bool HideWhenOutsideScreen = false;

        USCRIPT_PROPERTY()
        bool ScaleWithDistance = true;

        USCRIPT_PROPERTY()
        float ReferenceDistance = 10.0f; // 기준 거리 (이 거리일 때 스케일이 1.0)

        USCRIPT_PROPERTY()
        float MinScale = 0.3f; // 최소 크기 제한

        USCRIPT_PROPERTY()
        float MaxScale = 2.0f; // 최대 크기 제한

    private:
		ObjPtr<Graphic> m_rectGraphic;
    };
}
