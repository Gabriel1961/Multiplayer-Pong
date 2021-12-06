#pragma once
class GameObject
{
protected:
	virtual void Update() = 0;
	virtual void RenderPosUpdate() = 0;
	virtual void SetDefaultTransform(bool isLeft) = 0;
	virtual void Render() = 0;
};