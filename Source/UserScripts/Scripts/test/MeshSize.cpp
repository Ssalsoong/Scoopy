#include "Export.h"
#include "ScriptBehaviour.h"
#include "MeshSize.h"
#include "Transform.h"

void MMMEngine::MeshSize::Start()
{
	MeshTrans = GetTransform();
	if (!MeshTrans.IsValid()) {
		std::cout << u8"MeshRender Transform 미발견" << std::endl;
	}
	SetMatrixMesh(0);
}

void MMMEngine::MeshSize::SetMatrixMesh(float size)
{
	if (MeshTrans.IsValid())
	{
		auto scale_vec = Vector3{ size, size, size };
		MeshTrans->SetLocalScale(scale_vec);
		auto scale_localpos = MeshTrans->GetLocalPosition();
		scale_localpos.y = size / 2;
		MeshTrans->SetLocalPosition(scale_localpos);
	}
}
