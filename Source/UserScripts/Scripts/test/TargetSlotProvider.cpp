#include "Export.h"
#include "ScriptBehaviour.h"
#include "TargetSlotProvider.h"
#include "Transform.h"
#include <algorithm>
#include <cmath>
#include "PhysxManager.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"

namespace
{
    constexpr float kTwoPi = 6.28318530718f;
}

void MMMEngine::TargetSlotProvider::Start()
{
    m_rings.clear();
    centerTransform = GetTransform();
}

int MMMEngine::TargetSlotProvider::GetRingCapacity(float radius) const
{
    float spacing = slotSpacing;
    if (spacing < 0.05f) spacing = 0.05f;
    int cap = (int)std::floor((kTwoPi * radius) / spacing);
    return std::max(1, cap);
}

void MMMEngine::TargetSlotProvider::EnsureRing(int ringIndex)
{
    if (ringIndex < 0) return;

    while ((int)m_rings.size() <= ringIndex)
    {
        int idx = (int)m_rings.size();
        Ring ring{};
        ring.radius = baseRadius + ringSpacing * (float)idx;
        int cap = GetRingCapacity(ring.radius);
        ring.slots.resize(cap);
        m_rings.push_back(ring);
    }
}

bool MMMEngine::TargetSlotProvider::FindExisting(ObjPtr<GameObject> enemy, int& outRing, int& outIndex)
{
    if (!enemy.IsValid()) return false;

    for (int r = 0; r < (int)m_rings.size(); ++r)
    {
        auto& ring = m_rings[r];
        for (int i = 0; i < (int)ring.slots.size(); ++i)
        {
            auto& occ = ring.slots[i].occupant;
            if (!occ.IsValid())
            {
                occ = nullptr;
                continue;
            }
            if (occ == enemy)
            {
                outRing = r;
                outIndex = i;
                return true;
            }
        }
    }

    return false;
}

bool MMMEngine::TargetSlotProvider::RequestSlot(ObjPtr<GameObject> enemy, int& outRing, int& outIndex)
{
    if (!enemy.IsValid()) return false;
    if (FindExisting(enemy, outRing, outIndex))
        return true;

    auto enemyPos = enemy->GetTransform()->GetWorldPosition();

    struct Candidate
    {
        int ring, index;
        float distSq;
    };
    std::vector<Candidate> candidates;

    int limit = std::max(1, maxRings);
    for (int r = 0; r < limit; ++r)
    {
        EnsureRing(r);
        auto& ring = m_rings[r];
        for (int i = 0; i < (int)ring.slots.size(); ++i)
        {
            auto& occ = ring.slots[i].occupant;
            if (occ.IsValid()) continue;

            DirectX::SimpleMath::Vector3 pos;
            if (!GetSlotWorldPos(r, i, pos)) continue;
            if (!IsSlotFree(pos, enemy)) continue;

            float d = (pos - enemyPos).LengthSquared();
            candidates.push_back({ r, i, d });
        }
    }

    if (candidates.empty()) return false;

    std::sort(candidates.begin(), candidates.end(),
        [](const Candidate& a, const Candidate& b) { return a.distSq < b.distSq; });

    // 가장 가까운 슬롯 배정
    auto c = candidates.front();
    m_rings[c.ring].slots[c.index].occupant = enemy;
    outRing = c.ring;
    outIndex = c.index;
    return true;
}

void MMMEngine::TargetSlotProvider::ReleaseSlot(int ring, int index, ObjPtr<GameObject> enemy)
{
    if (ring < 0 || index < 0) return;
    if (ring >= (int)m_rings.size()) return;
    auto& slots = m_rings[ring].slots;
    if (index >= (int)slots.size()) return;

    auto& occ = slots[index].occupant;
    if (!occ.IsValid() || occ == enemy)
        occ = nullptr;
}

bool MMMEngine::TargetSlotProvider::GetSlotWorldPos(int ring, int index, DirectX::SimpleMath::Vector3& outPos)
{
    if (ring < 0 || index < 0) return false;
    if (ring >= (int)m_rings.size()) return false;
    const auto& slots = m_rings[ring].slots;
    if (index >= (int)slots.size()) return false;

    auto tr = GetTransform();
    if (!tr.IsValid()) return false;

    auto center = GetCenterWorldPos();
    float radius = m_rings[ring].radius;
    int cap = (int)slots.size();
    if (cap <= 0) return false;

    float angle = kTwoPi * ((float)index / (float)cap);
    outPos.x = center.x + std::cos(angle) * radius;
    outPos.z = center.z + std::sin(angle) * radius;
    outPos.y = center.y + yOffset;
    return true;
}

bool MMMEngine::TargetSlotProvider::IsSlotFree(const DirectX::SimpleMath::Vector3& pos, ObjPtr<GameObject> enemy) const
{
    auto& phys = PhysxManager::Get();
    std::vector<OverlapHit> hits;

    ObjPtr<ColliderComponent> enemyCol = enemy.IsValid() ? enemy->GetComponent<ColliderComponent>() : nullptr;
    ObjPtr<RigidBodyComponent> enemyRb = enemy.IsValid() ? enemy->GetComponent<RigidBodyComponent>() : nullptr;

    bool any = phys.OverlapSphere(pos, slotCheckRadius, hits, slotBlockLayer, enemyCol, enemyRb, includeTriggerInOverlap);
    if (!any) return true;

    for (auto& h : hits)
    {
        if (!h.gameObject.IsValid()) continue;
        if (enemy.IsValid() && h.gameObject == enemy) continue;
        if (h.gameObject == GetGameObject()) continue; // 타겟 자신은 무시
        return false;
    }

    return true;
}


DirectX::SimpleMath::Vector3 MMMEngine::TargetSlotProvider::GetCenterWorldPos() const
{
    if (!centerTransform.IsValid())
        return {};

    return centerTransform->GetWorldPosition();
}