#include <glm/gtx/quaternion.hpp>

#include "basic/basic_camera.h"

using namespace glm;

BasicCamera::BasicCamera() :
	TORADIAN(pi<float>() / 180.0f),
	mEye(vec3(150.0f, 25.0f, -100.0f)),
	mDist(150.0f),
	mUp(vec3(0, 1.0f, 0.0f)),
	mFw(vec3(0, 0, -1.0f)),
	mAngle(vec3(0.0f, -45.0f, 0)),
	//        mUp(vec3(0, 0.0f, -1.0f)),
	m_zNear(2.0f),
	m_zFar(10000.0f),
	mAt(vec3(0)),
	mPerspectiveMat(mat4()),
	mBodyForward(vec3(0.0f, 0.0f, 0.0f)) // real body direct
{
	//    UpdateAt();
	mAt = vec3(0, 0, 0);
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::UpdateAngle()
{
	vec3 dir = mAt - mEye;
	vec3 projDir = projToPlane(dir, mUp);
	if (dot(dir, mUp) > 0)
	{
		mAngle.y = getAngle(projDir, dir) / TORADIAN; // pitch
	}
	else
	{
		mAngle.y = -getAngle(projDir, dir) / TORADIAN;
	}
	mAngle.x = getAngle(mFw, projDir) / TORADIAN; // yaw
}

void BasicCamera::UpdateAt()
{
	vec3 dir = mFw;

	quat qYaw = angleAxis(mAngle.x * TORADIAN, mUp);
	dir = toMat3(qYaw) * dir;
	vec3 pitchAxis = cross(dir, mUp);
	quat qPitch = angleAxis(mAngle.y * TORADIAN, pitchAxis);
	dir = toMat3(qPitch) * dir;
	mAt = dir * mDist + mEye;
}

void BasicCamera::RotateAuto(const double& deltaTime)
{
	float degree = ROTATION_VEL * deltaTime;

	// Add yaw angle;
	mAngle.x += degree;
	limitAngle(mAngle.x);

	vec3 prevAt = mAt;
	UpdateAt();

	vec3 deltaAt = prevAt - mAt;

	mAt = prevAt;
	mEye += deltaAt;
}

void BasicCamera::Rotate(const float& deltaX, const float& deltaY)
{
	mAngle.x += deltaX;
	mAngle.y += deltaY;
	limitAngle(mAngle.x);
	mAngle.y = clamp<float>(mAngle.y, -89.9, 89.9);
	UpdateAt();
}

void BasicCamera::ZoomIn(const float& vel)
{
	vec3 dir = mAt - mEye;
	float remain = glm::length(dir);
	remain -= ZOOM_MIN_LENGTH;
	dir = normalize(dir);

	remain = (remain > vel) ? vel : remain;
	dir *= remain;
	mEye += dir;
	mDist -= remain;
}

void BasicCamera::ZoomOut(const float& vel)
{
	vec3 dir = mEye - mAt;
	dir = normalize(dir);
	dir *= vel;
	mEye += dir;
	mDist += vel;
}


void BasicCamera::SetEye(const float& x, const float& y, const float& z)
{
	mEye.x = x;
	mEye.y = y;
	mEye.z = z;
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

void BasicCamera::SetAt(const float& x, const float& y, const float& z)
{
	mAt.x = x;
	mAt.y = y;
	mAt.z = z;
	mDist = distance(mEye, mAt);
	UpdateAngle();
}

const glm::vec3 BasicCamera::GetEye() const
{
	return mEye;
}

const glm::vec3 BasicCamera::GetAt() const
{
	return mAt;
}

void BasicCamera::MoveForward(const float& vel)
{
//	vec3 dir = mAt - mEye;
	mBodyForward = mAt - mEye;
	mBodyForward = vec3(mBodyForward.x, 0.0f, mBodyForward.z);
	vec3 dir = mBodyForward;
	dir = normalize(dir) * vel;
	mEye += dir;
	mAt += dir;
//	UpdateAt();
}

void BasicCamera::MoveBackward(const float& vel)
{
//	vec3 dir = mEye - mAt;
	mBodyForward = mAt - mEye;
	mBodyForward = vec3(mBodyForward.x, 0.0f, mBodyForward.z);
	vec3 dir = -mBodyForward;
	dir = normalize(dir) * vel;
	mEye += dir;
	mAt += dir;
//	UpdateAt();
}

void BasicCamera::MoveRight(const float& vel)
{
//	vec3 dir = mEye - mAt;
	mBodyForward = mAt - mEye;
	mBodyForward = vec3(mBodyForward.x, 0.0f, mBodyForward.z);
	vec3 dir = mBodyForward;
	dir = normalize(dir);
	dir = -cross(mUp, dir) * vel;
	mEye += dir;
	mAt += dir;
//	UpdateAt();
}

void BasicCamera::MoveLeft(const float& vel)
{
//	vec3 dir = mEye - mAt;
	mBodyForward = mAt - mEye;
	mBodyForward = vec3(mBodyForward.x, 0.0f, mBodyForward.z);
	vec3 dir = mBodyForward;
	dir = normalize(dir);
	dir = cross(mUp, dir) * vel;
	mEye += dir;
	mAt += dir;
//	UpdateAt();
}

void BasicCamera::MoveUp(const float& vel)
{
	vec3 dir = mUp * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::MoveDown(const float& vel)
{
	vec3 dir = -mUp * vel;
	mEye += dir;
	UpdateAt();
}

void BasicCamera::ViewRotate(const float& vel)
{

	// vel is degree
	float degree = vel * TORADIAN;			// degree to change
//	float dist = distance(mEye,mAt);		// distance between mEye and mAt
	vec3 axisN = normalize(mAt - mEye);
	vec3 axisU = normalize(cross(axisN,mUp));
	vec3 axisV = normalize(cross(axisN,axisU));

	mat3 worldToTempSpace = mat3(axisU.x,axisV.x,axisN.x,
			   	   	   	   	   	 axisU.y,axisV.y,axisN.y,
			   	   	   	   	   	 axisU.z,axisV.z,axisN.z);

	mat3 tempToWorldSpace = mat3(axisU.x,axisU.y,axisU.z,
								 axisV.x,axisV.y,axisV.z,
								 axisN.x,axisN.y,axisN.z);

	// {u, v, n} -> rotate by axis v .
	mat3 rotateByDegree = mat3(cos(degree),	 	0.0f, 	  -sin(degree),
							   	   	  0.0f,	 	1.0f, 			  0.0f,
							   sin(degree),		0.0f, 	   cos(degree));


/*


	mat3 worldToTempSpace = mat3(axisY.x,axisY.y,axisY.z,
								 axisZ.x,axisZ.y,axisZ.z,
								 axisX.x,axisX.y,axisX.z);

	mat3 tempToWorldSpace = mat3(axisY.x,axisZ.x,axisX.x,
								 axisY.y,axisZ.y,axisX.y,
								 axisY.z,axisZ.z,axisX.z);

	mat3 rotateByDegree = mat3(cos(degree), sin(degree), 0.0f,
							   -sin(degree), cos(degree), 0.0f,
							   0.0f,	0.0f,	1.0f);
*/

	vec3 dir = mAt - mEye;
	dir = worldToTempSpace * dir;	// move to tempSpace
	dir = rotateByDegree * dir;		// rotate
	dir = tempToWorldSpace * dir;	// move to worldSpace
	dir = mEye + dir;				// set At newly
	mAt = dir;
	mBodyForward = vec3(mAt.x, 0.0f, mAt.z);

	UpdateAngle();

}

// See up and down
void BasicCamera::ViewRotateY(const float& vel)
{

	// vel is degree
	float degree = vel * TORADIAN;			// degree to change
//	float dist = distance(mEye,mAt);		// distance between mEye and mAt
	vec3 axisN = normalize(mAt - mEye);
	vec3 axisU = normalize(cross(axisN,mUp));
	vec3 axisV = normalize(cross(axisN,axisU));

	mat3 worldToTempSpace = mat3(axisU.x,axisV.x,axisN.x,
			   	   	   	   	   	 axisU.y,axisV.y,axisN.y,
			   	   	   	   	   	 axisU.z,axisV.z,axisN.z);

	mat3 tempToWorldSpace = mat3(axisU.x,axisU.y,axisU.z,
								 axisV.x,axisV.y,axisV.z,
								 axisN.x,axisN.y,axisN.z);

	// {u, v, n} -> rotate by axis v .
	mat3 rotateByDegree = mat3(		  1.0f,	  	 	0.0f, 	  		0.0f,
							   	   	  0.0f,  cos(degree), 	-sin(degree),
							   	   	  0.0f,	 sin(degree), 	 cos(degree));

	vec3 dir = mAt - mEye;
	mBodyForward = vec3 (dir.x, 0.0f, dir.z);
	// limit up and down degree 30
	float limitVel = mAt.y - mEye.y;
	if( limitVel <= 10.0f && limitVel >= -10.0f) {


	dir = worldToTempSpace * dir;	// move to tempSpace
	dir = rotateByDegree * dir;		// rotate
	dir = tempToWorldSpace * dir;	// move to worldSpace
	dir = mEye + dir;				// set At newly
	SetAt(dir.x, dir.y, dir.z);

	UpdateAngle();
	}
	else if (limitVel < -10.0f) {
		mAt.y = mEye.y - 10.0f;
		UpdateAngle();
	}
	else if (limitVel > 10.0f ) {
		mAt.y = mEye.y + 10.0f;
		UpdateAngle();
	}
}
