#include "Export.h"
#include "ScriptBehaviour.h"
#include "WorldSpaceUISorter.h"
#include "WorldSpaceUI.h"

using namespace MMMEngine;

ObjPtr<WorldSpaceUISorter> WorldSpaceUISorter::Instance = nullptr;

void MMMEngine::WorldSpaceUISorter::RegisterUI(ObjPtr<WorldSpaceUI> ui)
{
	if (ui.IsValid()) m_uiElements.push_back(ui);
}

void MMMEngine::WorldSpaceUISorter::UnregisterUI(ObjPtr<WorldSpaceUI> ui)
{
	m_uiElements.erase(std::remove_if(m_uiElements.begin(), m_uiElements.end(),
		[&](const ObjPtr<WorldSpaceUI>& ptr) { return !ptr.IsValid() || ptr == ui; }), m_uiElements.end());
}

void MMMEngine::WorldSpaceUISorter::Awake()
{
	if (!Instance.IsValid())
	{
		Instance = SelfPtr(this);
	}
	else
	{
		GetGameObject()->Destroy(GetGameObject());
	}
}

void MMMEngine::WorldSpaceUISorter::LateUpdate()
{
	// 1. 유효하지 않은 포인터 정리
	m_uiElements.erase(std::remove_if(m_uiElements.begin(), m_uiElements.end(),
		[](const ObjPtr<WorldSpaceUI>& ptr) { return !ptr.IsValid(); }), m_uiElements.end());

    // 2. 두 그룹으로 분류: 정렬할 대상 vs 정렬 안 할 대상
    std::vector<ObjPtr<WorldSpaceUI>> sortGroup;
    std::vector<ObjPtr<WorldSpaceUI>> staticGroup;

    for (auto& ui : m_uiElements) {
        if (ui->DontSortByDistance) {
            staticGroup.push_back(ui);
        }
        else {
            sortGroup.push_back(ui);
        }
    }

    // 3. 정렬 대상 그룹만 거리순 정렬 (먼 것 -> 낮은 오더)
    std::sort(sortGroup.begin(), sortGroup.end(), [](const ObjPtr<WorldSpaceUI>& a, const ObjPtr<WorldSpaceUI>& b) {
        return a->GetLastDistance() > b->GetLastDistance();
        });

    // 4. Render Order 할당
    int baseOrder = -1000;

    // 정렬된 그룹 먼저 할당
    for (auto& ui : sortGroup) {
        ui->SetUIRenderOrder(baseOrder++);
    }
}
