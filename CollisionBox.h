#pragma once
#include "Model.h"
#include "Object3D.h"

class CollisionBox
{
private:

	Model* model_;
	Object3D* object_;

	DirectX::XMFLOAT3 scale_;

public:

	bool hit_ = false;

	bool arive = false;

	DirectX::XMFLOAT3 GetWorldPos();

	const DirectX::XMFLOAT3& GetScale() const { return scale_; }
	void SetScale(const DirectX::XMFLOAT3& scale) { scale_ = scale; }

	bool GetCollision() const { return hit_; }

	void Initialize(Model* Model, Object3D* Object);
	void Update();
	void Draw();

	bool OnCollision();
};

