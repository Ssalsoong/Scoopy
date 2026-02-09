#include "Export.h"
#include "ScriptBehaviour.h"
#include "FXSnowFall.h"
#include "ParticleRenderer.h"
#include "MMMTime.h"

using namespace DirectX::SimpleMath;

void MMMEngine::FXSnowFall::Start()
{
	if (!m_snowParticle.IsValid())
		m_snowParticle = GetComponent<ParticleRenderer>();
}

void MMMEngine::FXSnowFall::Update()
{
	if (m_snowParticle.IsValid())
	{
		float t = Time::GetTotalTime();
		Vector3 wind;
		wind.x = sinf(t * 0.8f) * 0.25f + sinf(t * 2.1f) * 0.08f;
		wind.y = -0.45f;
		wind.z = cosf(t * 0.9f) * 0.22f + sinf(t * 1.7f) * 0.06f;
		m_snowParticle->SetUpdateFormula(wind);
	}
}
