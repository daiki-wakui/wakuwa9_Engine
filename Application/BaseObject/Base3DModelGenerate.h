#pragma once
class Base3DModelGenerate
{
public:
	virtual ~Base3DModelGenerate() = default;
	//初期化
	virtual void Initialize() = 0;
	//毎フレーム
	virtual void Update() = 0;
};

