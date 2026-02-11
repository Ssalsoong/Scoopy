#include "Export.h"
#include "ScriptBehaviour.h"
#include "ExplosionPool.h"
#include "ExplosionParticles.h"
#include "Prefab.h"
#include "Transform.h"

using namespace MMMEngine;

ObjPtr<ExplosionPool> ExplosionPool::Instance = nullptr;

void MMMEngine::ExplosionPool::Spawn(const DirectX::SimpleMath::Vector3& position)
{
    // 1. 비활성 인스턴스 찾기
    ObjPtr<ExplosionParticles> chosen = nullptr;
    for (auto& e : m_pool)
    {
        auto go = e.IsValid() ? e->GetGameObject() : ObjPtr<GameObject>{};
        if (go.IsValid() && !go->IsActiveInHierarchy())
        {
            chosen = e;
            break;
        }
    }

    // 2. 없으면 새로 생성
    if (!chosen.IsValid())
    {
        if (ExplosionPrefab == nullptr)
            return;

        auto newGo = Instantiate(ExplosionPrefab); // 프로젝트에 맞는 생성 API 사용
        if (!newGo.IsValid())
            return;

        auto comp = newGo->GetComponent<ExplosionParticles>();
        if (!comp.IsValid())
            return;

        comp->owner = SelfPtr(this);
        chosen = comp;
        m_pool.push_back(chosen);
    }

    // 3. 위치 세팅 + 활성화
    if (auto go = chosen->GetGameObject(); go.IsValid())
    {
        auto tr = go->GetTransform();
        if (tr)
            tr->SetWorldPosition(position);

        // OnEnable 안에서 PlayOneShot 이 자동 호출됨
        go->SetActive(true);
    }
}

void MMMEngine::ExplosionPool::Release(ObjPtr<ExplosionParticles> effect)
{
    if (!effect.IsValid())
        return;

    if (auto go = effect->GetGameObject(); go.IsValid())
        go->SetActive(false);
}

void MMMEngine::ExplosionPool::Awake()
{
    if (!Instance.IsValid())
    {
        Instance = SelfPtr(this);

        const int prewarmCount = 4;

        for (int i = 0; i < prewarmCount; ++i)
        {
            if (ExplosionPrefab == nullptr)
                break;

            auto newGo = Instantiate(ExplosionPrefab);
            if (!newGo.IsValid())
                continue;

            auto comp = newGo->GetComponent<ExplosionParticles>();
            if (!comp.IsValid())
                continue;

            comp->owner = SelfPtr(this);
            m_pool.push_back(comp);

            // 처음에는 비활성 상태로 풀에 대기
            newGo->SetActive(false);
        }
    }
    else
    {
        GetGameObject()->Destroy(GetGameObject());
    }

   
}

void MMMEngine::ExplosionPool::OnDestroy()
{
    if (Instance == SelfPtr(this))
    {
        Instance = nullptr;
    }
}