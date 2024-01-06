#include "PlayerMove.h"
#include "wa9Math.h"

void PlayerMove::Initialize(Object3D* object)
{
	obj_ = object;
}

void PlayerMove::Update()
{
	rot_ = obj_->GetRotation();
	pos_ = obj_->GetPosition();
	eye_ = obj_->GetEye();
	target_ = obj_->GetTarget();

	Move();
}

void PlayerMove::Move()
{
	frontVec.x = eye_.x - target_.x;
	frontVec.z = eye_.z - target_.z;

	frontVec.normalize();
	frontVec /= FRONT_VECTOR_RATE;

	if (dash) {
		dashPower_ += DASH_POWTER_VOLUE;

		dashPower_ = min(dashPower_, DASH_POWTER_MAX_VOLUE);

		frontVec.x *= dashPower_;
		frontVec.z *= dashPower_;
	}

	if (inputPad_->InputLStickUp() || input_->keyPush(DIK_UP)) {
		Vector3 tmppos = pos_;

		/*Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);*/

		frontMove_.x = -frontVec.x;
		frontMove_.z = -frontVec.z;
		rot_.x++;
		rot_.x = min(rot_.x, ROT_MAX);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.x > 0) {
			rot_.x--;
			rot_.x = max(rot_.x, 0);
		}

	}

	if (inputPad_->InputLStickDown() || input_->keyPush(DIK_DOWN)) {
		Vector3 tmppos = pos_;

		/*Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);*/

		frontMove_.x = frontVec.x;
		frontMove_.z = frontVec.z;
		rot_.x--;
		rot_.x = max(rot_.x, -ROT_MAX);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.x < 0) {
			rot_.x++;
			rot_.x = min(rot_.x, 0);
		}

		frontMove_ *= friction;
	}

	moveXVec = tmpVecY.cross(frontVec);

	if (inputPad_->InputLStickRight() || input_->keyPush(DIK_RIGHT)) {

		Vector3 tmppos = pos_;

		/*Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);*/

		sideMove_.x = -moveXVec.x;
		sideMove_.z = -moveXVec.z;
		rot_.z--;
		rot_.z = max(rot_.z, -ROT_MAX);
	}
	else {
		sideMove_ *= friction;
		if (!inputPad_->InputLStick() && rot_.z < 0) {
			rot_.z++;
			rot_.z = min(rot_.z, 0);
		}
	}

	if (inputPad_->InputLStickLeft() || input_->keyPush(DIK_LEFT)) {

		Vector3 tmppos = pos_;

		/*Vector3 vel{};
		vel.x = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);
		vel.y = MyRandom::GetFloatRandom(EFFECT_MIN, EFFECT_MAX);

		moveParticle_->Add(PARTICLE_TIME, tmppos, vel, PARTICLE_ACCEL, PARTICLE_START_SCALE, PARTICLE_END_SCALE);*/

		sideMove_.x = moveXVec.x;
		sideMove_.z = moveXVec.z;
		rot_.z++;
		rot_.z = min(rot_.z, ROT_MAX);
	}
	else {
		if (!inputPad_->InputLStick() && rot_.z > 0) {
			rot_.z--;
			rot_.z = max(rot_.z, 0);
		}
		sideMove_ *= friction;
	}

	if (!inputPad_->InputLStick()) {
		dash = false;
		dashPower_ = 1.0f;
	}


	if (inputPad_->RTrigger()) {
		dash = true;
	}

	pos_.x += frontMove_.x;
	pos_.y += frontMove_.y;
	pos_.z += frontMove_.z;

	pos_.x += sideMove_.x;
	pos_.y += sideMove_.y;
	pos_.z += sideMove_.z;

	pos_.y = sinf(wa9Math::PI() * (frame + ADD_FRAME) * FRAME_RATE) * POS_Y_RATE + ADD_POS_Y_VOLUE;

	frame++;

	if (inputPad_->InputRStick() == false && inputPad_->InputLStick() == false) {
		RotateAngle();
	}
	else if (inputPad_->InputLStick() && inputPad_->InputRStick()) {
		RotateAngle();

		rot_.y = -angle_;
	}
	else if (inputPad_->InputLStick() && inputPad_->InputRStick() == false) {
		RotateAngle();

		rot_.y = -angle_;
	}

	obj_->SetRotation(rot_);
	obj_->SetPosition(pos_);
}

void PlayerMove::RotateAngle()
{
	radi_ = std::atan2(pos_.z - eye_.z, pos_.x - eye_.x);
	angle_ = radi_ * (wa9Math::Degree180() / wa9Math::PI()) - wa9Math::Degree90();
}