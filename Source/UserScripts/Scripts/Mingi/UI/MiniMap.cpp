#include "MiniMap.h"
#include "Transform.h"
#include "RectTransform.h"
#include "GameObject.h"
#include "Prefab.h"

using namespace MMMEngine;

ObjPtr<MiniMap> MiniMap::Instance = nullptr;


void MMMEngine::MiniMap::OnDestroy()
{
    if (Instance == SelfPtr(this))
        Instance = nullptr;
}

void MMMEngine::MiniMap::Awake()
{
    if (!Instance.IsValid())
    {
        Instance = SelfPtr(this);

        // 풀 초기화
        if (EnemyTrackerUI != nullptr)
        {
            for (int i = 0; i < InitialPoolSize; ++i)
            {
                auto ui = Instantiate(EnemyTrackerUI)->GetComponent<RectTransform>();
                if (ui.IsValid())
                {
                    ui->GetGameObject()->SetActive(false);
                    ui->SetParent(GetTransform()); // 미니맵 UI의 자식으로
                    m_enemyTrackerUIPool.push_back(ui);
                }
            }
        }

        if (BuildingTrackerUI != nullptr)
        {
            for (int i = 0; i < InitialPoolSize; ++i)
            {
                auto ui = Instantiate(BuildingTrackerUI)->GetComponent<RectTransform>();
                if (ui.IsValid())
                {
                    ui->GetGameObject()->SetActive(false);
                    ui->SetParent(GetTransform());
                    m_buildingTrackerUIPool.push_back(ui);
                }
            }
        }
    }
    else
    {
        GetGameObject()->Destroy(GetGameObject());
    }
}

void MMMEngine::MiniMap::LateUpdate()
{
    if (!Player.IsValid() || !PlayerRectUI.IsValid())
        return;

    // 플레이어 UI 업데이트
    const auto& playerPos = Player->GetWorldPosition();
    PlayerRectUI->SetAnchoredPosition({ playerPos.x * MinimapUnitScale, playerPos.z * MinimapUnitScale });

    // 모든 트래커 업데이트
    for (auto it = m_trackerUICached.begin(); it != m_trackerUICached.end();)
    {
        auto& tracker = it->second;

        // 타겟이 유효하지 않으면 제거
        if (!tracker.target.IsValid() || !tracker.trackerUI.IsValid())
        {
            if (tracker.trackerUI.IsValid())
            {
                ReturnTrackerUIToPool(tracker.trackerUI, tracker.type);
            }
            it = m_trackerUICached.erase(it);
            continue;
        }

        // 위치 업데이트
        const auto& targetPos = tracker.target->GetWorldPosition();
        tracker.trackerUI->SetAnchoredPosition({
            targetPos.x * MinimapUnitScale,
            targetPos.z * MinimapUnitScale
            });

        ++it;
    }
}

void MMMEngine::MiniMap::RegisterTracker(const Utility::MUID& id, ObjPtr<Transform> target, TrackerType type)
{
    if (!target.IsValid())
        return;

    // 이미 등록된 경우 무시
    if (m_trackerUICached.find(id) != m_trackerUICached.end())
        return;

    // 풀에서 UI 가져오기
    auto trackerUI = GetTrackerUIFromPool(type);
    if (!trackerUI.IsValid())
        return;

    // 활성화 및 초기 위치 설정
    trackerUI->GetGameObject()->SetActive(true);
    const auto& targetPos = target->GetWorldPosition();
    trackerUI->SetAnchoredPosition({
        targetPos.x * MinimapUnitScale,
        targetPos.z * MinimapUnitScale
        });

    // 캐시에 추가
    m_trackerUICached[id] = { target, trackerUI, type };
}

void MMMEngine::MiniMap::UnregisterTracker(const Utility::MUID& id)
{
    auto it = m_trackerUICached.find(id);
    if (it == m_trackerUICached.end())
        return;

    auto& tracker = it->second;

    // 풀로 반환
    if (tracker.trackerUI.IsValid())
    {
        ReturnTrackerUIToPool(tracker.trackerUI, tracker.type);
    }

    // 캐시에서 제거
    m_trackerUICached.erase(it);
}

MMMEngine::ObjPtr<MMMEngine::RectTransform> MMMEngine::MiniMap::GetTrackerUIFromPool(TrackerType type)
{
    auto& pool = (type == TrackerType::Enemy) ? m_enemyTrackerUIPool : m_buildingTrackerUIPool;
    auto& prefab = (type == TrackerType::Enemy) ? EnemyTrackerUI : BuildingTrackerUI;

    // 풀에서 사용 가능한 UI 찾기
    for (auto it = pool.begin(); it != pool.end(); ++it)
    {
        if ((*it).IsValid() && !(*it)->GetGameObject()->IsActiveInHierarchy())
        {
            auto ui = *it;
            pool.erase(it);
            return ui;
        }
    }

    // 풀이 비어있으면 새로 생성
    if (prefab != nullptr)
    {
        auto ui = Instantiate(prefab)->GetComponent<RectTransform>();
        if (ui.IsValid())
        {
            ui->SetParent(GetTransform());
        }
        return ui;
    }

    return ObjPtr<RectTransform>();
}

void MMMEngine::MiniMap::ReturnTrackerUIToPool(ObjPtr<RectTransform> ui, TrackerType type)
{
    if (!ui.IsValid())
        return;

    if(ui->GetGameObject().IsValid() && !ui->GetGameObject()->IsDestroyed())
        ui->GetGameObject()->SetActive(false);

    auto& pool = (type == TrackerType::Enemy) ? m_enemyTrackerUIPool : m_buildingTrackerUIPool;
    pool.push_back(ui);
}