#include "Export.h"
#include "ScriptBehaviour.h"
#include "ExplosionParticles.h"
#include "ParticleRenderer.h"
#include "ExplosionPool.h"

void MMMEngine::ExplosionParticles::OnEnable()
{
	if (!Particle1.IsValid() ||
		!Particle2.IsValid() ||
		!Particle3.IsValid() ||
		!Particle4.IsValid())
		return;

	Particle1->PlayOneShot(0.1f);
	Particle2->PlayOneShot(0.1f);
	Particle3->PlayOneShot(0.1f);
	Particle4->PlayOneShot(0.1f);

	m_inUse = true;
}

void MMMEngine::ExplosionParticles::Update()
{
    if (!m_inUse)
        return;

    bool p1 = Particle1.IsValid() && Particle1->IsPlaying();
    bool p2 = Particle2.IsValid() && Particle2->IsPlaying();
    bool p3 = Particle3.IsValid() && Particle3->IsPlaying();

    if (!p1 && !p2 && !p3)
    {
        m_inUse = false;

        // 풀에 돌려보내기
        if (owner)
            owner->Release(SelfPtr(this));
        else
        {
            // 풀 없이도 최소한 자기 자신 비활성화
            auto go = GetGameObject();
            if (go.IsValid())
                go->SetActive(false);
        }
    }
}
