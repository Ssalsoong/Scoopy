#include "rttr/type"
#include "ScriptBehaviour.h"
#include "SimpleMath.h"
#include "UserScriptsCommon.h"

namespace MMMEngine
{
    class USERSCRIPTS Player : public ScriptBehaviour
    {
    private:
        RTTR_ENABLE(ScriptBehaviour)
        RTTR_REGISTRATION_FRIEND
    public:
		Player()
		{
        REGISTER_BEHAVIOUR_MESSAGE(Start);
        REGISTER_BEHAVIOUR_MESSAGE(Update);

        }
		USCRIPT_MESSAGE()
		void Start();
		USCRIPT_MESSAGE()
		void Update();
		USCRIPT_PROPERTY()
		int level = 0;
		USCRIPT_PROPERTY()
		int HP = 100;
		USCRIPT_PROPERTY()
		int maxHP = 100;
		USCRIPT_PROPERTY()
		float battledist = 0.7f;
		USCRIPT_PROPERTY()
		int atk = 15;
		float attackDelay = 0.65f;
		int maxpoint = 10;
		float velocity = 8.0f;
		int healHP = 10;
		float healDelay = 1.0f;
		float nonfightDelay = 10.0f;
		float damageDelay = 0.1f;
		float baseRadius = 0.5f;
		float k = 1.4f;
		int prevHP = 100;
		float attackTimer = 0.0f;
		float healTimer = 0.0f;
		float nonfightTimer = 0.0f;
		bool fighting = false;
		float damageTimer = 0.0f;
		bool isMoving = false;
		bool scoopHeld = false;
		bool buildchance = true;

		float velocitydown = 0.0f;

		DirectX::SimpleMath::Vector3 pos;
		ObjPtr<GameObject> matchedSnowball = nullptr;
		void HandleMovement();
		void AutoHeal();
		void HandleAttack();
		void UpdateScoop();
		void GetDamage(int t);
		bool PlayerDeath() const { return HP <= 0; }
		bool AttachSnowball(ObjPtr<GameObject> snow);
		void DetachSnowball();
		void SnapToSnowball(ObjPtr<GameObject> snow);
		void LookAt(const DirectX::SimpleMath::Vector3& target);
		ObjPtr<GameObject> GetMatchedSnowball()const { return matchedSnowball; }

		void BuildOn();
		void LevelUp();
		void Velocitydown();
    };
}
