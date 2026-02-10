#include "Export.h"
#include "ScriptBehaviour.h"
#include "ExplosionParticles.h"
#include "ParticleRenderer.h"

void MMMEngine::ExplosionParticles::PlayOneShotAllParticle()
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
}
