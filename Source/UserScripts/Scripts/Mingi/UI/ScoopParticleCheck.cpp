#include "Export.h"
#include "ScriptBehaviour.h"
#include "ScoopParticleCheck.h"
#include "../../test/PlayerController.h"
#include "ParticleRenderer.h"

void MMMEngine::ScoopParticleCheck::Start()
{
	// todo : 님아 터지면 PC, PT 확인하셈 꼭
}

void MMMEngine::ScoopParticleCheck::Update()
{
    if (pc->IsParticleEnable())
    {
        if (!pt->IsPlaying())
        {
            pt->Play();
        }
    }
    else
    {
        if (pt->IsPlaying())
        {
            pt->Stop();
        }
    }
}
