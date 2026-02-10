#pragma once
#include "rttr/type"
#include "ScriptBehaviour.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class ParticleRenderer;
    class USERSCRIPTS ExplosionParticles : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
        ExplosionParticles()
        {
        REGISTER_BEHAVIOUR_MESSAGE(PlayOneShotAllParticle);

        }

        USCRIPT_MESSAGE()
            void PlayOneShotAllParticle();

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle1;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle2;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle3;

        USCRIPT_PROPERTY()
            ObjPtr<ParticleRenderer> Particle4;
    };
}
