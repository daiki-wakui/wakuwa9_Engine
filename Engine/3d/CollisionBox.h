#pragma once
#include "Model.h"
#include "Object3D.h"

class CollisionBox
{
private:

	Model* model_;
	Object3D* object_;

	Vector3 scale_;

public:

	bool hit_ = false;

	bool arive = false;

	Vector3 GetWorldPos();

	const Vector3& GetScale() const { return scale_; }
	void SetScale(const Vector3& scale) { scale_ = scale; }

	bool GetCollision() const { return hit_; }

	void Initialize(Model* Model, Object3D* Object);
	void Update();
	void Draw();

	bool OnCollision();
};

