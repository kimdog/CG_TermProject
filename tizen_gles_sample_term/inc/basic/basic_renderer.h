#ifndef __BASIC_RENDERER_H__
#define __BASIC_RENDERER_H__

#include <vector>

#include "basic_gl_set.h"
#include "basic_utils.h"
#include "basic_camera.h"
#include "basic_texdata.h"
#include "basic_shader.h"
#include "basic_timer.h"

//#include "test_module.h"
using namespace std;

struct VertexStruct
{
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec2 tex;
};

// =========================================
// struct of each Object
// =========================================
struct ObjectSturct
{

	// vertex buffer
	std::vector<VertexStruct> myVertexBuffer;
	std::vector<GLushort> myIndexBuffer;
	std::vector<glm::vec3> myTangentBuffer;


	// vertex buffer object and index buffer object
	GLuint myVboVertices;
	GLuint myVboIndices;
	GLuint myVboTangents;
	GLuint myVboInstTransforms;


	// variables for texture handling
	bool myHasTexture;
	bool myHasCubemap;
	bool myHasNorMap;
	bool myHasHeightmap;


	// Texture object id
	GLuint myTexId;
	GLuint myTexCubeId;
	GLuint myTexNorId;
	GLuint myTexHeightId;

};

class BasicRenderer
{
public:
	const int V_ATTRIB_POSITION = 0;
	const int V_ATTRIB_NORMAL = 1;
	const int V_ATTRIB_TEX = 2;
	const int V_ATTRIB_TANGENT = 3;
	const int V_ATTRIB_INSTPOSITION = 5;

	const int TEX_POS_NORMAL = 6;
	const int TEX_POS_CUBEMAP = 7;

protected:
	int mWidth;
	int mHeight;
	double mDeltaTime;

	BasicTimer* mTimer;
	BasicCamera* mCamera;
	BasicShader* mShader;

	bool mIsAutoRotateEye;
	bool mIsFill;

	bool mIsTouchOn;
	// Stores Touch Point x, y determined by scrolling screen
	glm::vec2 mTouchPoint;

	// =========================================================
	// all add 2 means just test second object ...
	// later not use this ...
	// =========================================================

	// vertex buffer
	std::vector<VertexStruct> mVertexBuffer;
	std::vector<VertexStruct> mVertexBuffer2;

	std::vector<GLushort> mIndexBuffer;
	std::vector<GLushort> mIndexBuffer2;

	std::vector<glm::vec3> mTangentBuffer;
	std::vector<glm::vec3> mTangentBuffer2;


	// vertex buffer object and index buffer object
	GLuint mVboVertices;
	GLuint mVboVertices2;
	GLuint mVboIndices;
	GLuint mVboIndices2;
	GLuint mVboTangents;
	GLuint mVboTangents2;
	GLuint mVboInstTransforms;
	GLuint mVboInstTransforms2;



	// variables for texture handling
	bool mHasTexture;
	bool mHasTexture2;
	bool mHasCubemap;
	bool mHasCubemap2;
	bool mHasNorMap;
	bool mHasNorMap2;
	bool mHasHeightmap;
	bool mHasHeightmap2;

	// Texture object id
	GLuint mTexId;
	GLuint mTexId2;
	GLuint mTexCubeId;
	GLuint mTexCubeId2;
	GLuint mTexNorId;
	GLuint mTexNorId2;
	GLuint mTexHeightId;
	GLuint mTexHeightId2;



	int mInstCount;

	// Set of each Object Struct
	std::vector<ObjectSturct> myObjects;


public:
	BasicRenderer();

	~BasicRenderer();

	// App interface functions
	// GLES20
	bool SetProgram(const std::string& vertexSource, const std::string& fragmentSource) const;

	bool SetProgram(const char* vertexSource, const char* fragmentSource) const;

	// GLES31
	bool SetProgram(const char* vertexSource, const char* tessControlSource, const char* tessEvalSource,
	                const char* geometrySource, const char* fragmentSource) const;

	void SetNewModel(const std::string& objSource, const float& scale);
	void SetNewModel2(const std::string& objSource, const float& scale);
	void SetNewModel_multi(const std::string& objSource, const float& scale);


	void SetNewModel(char* objSource);

	void SetNewModel(char* objSource, const float& scale);
	void SetNewModel2(char* objSource, const float& scale);
	void SetNewModel_multi(char* objSource, const float& scale);


	void SetTexture(TexData_Type type, const TexData* newTex);
	void SetTexture2(TexData_Type type, const TexData* newTex);
	void SetTexture_multi(TexData_Type type, const TexData* newTex);



	bool Initialize();
	bool Initialize2();
	bool Initialize_multi();


	void SetViewPort(int w, int h);

	void RenderFrame();

	// CANNOT USE IN GLES
	void TogglePolygonMode();


private: // Texture functions
	void CreateTexBuffer(const TexData& newTex, GLuint& target) const;

	void TexBuffer(GLenum type, const TexData& newTex) const;

private:// Rendering functions
	void SetState() const;

	void CreateVbo();
	void CreateVbo2();
	void CreateVbo_multi();



	void PassUniform() const;

	void Draw() const;
	void Draw2() const;


	void Draw(vector<GLushort> mIndexBuffer) const; // case add specific buffer

public:
	void RebindVbo() const;
	void RebindVbo2() const;
	void RebindVbo_multi(const int& i) const;


private:// Utility functions
	void LogInfo() const;

	void CountTickInit() const;

	void ComputeTick();

	void ComputeTangent();
	void ComputeTangent2();
	void ComputeTangent_multi();

	void ImportModel(char* objSource, BasicRenderer* renderer) const;

	void ImportModelScale(char* objSource, BasicRenderer* renderer, const float& scale) const;
	void ImportModelScale2(char* objSource, BasicRenderer* renderer, const float& scale) const;
	void ImportModelScale_multi(char* objSource, BasicRenderer* renderer, const float& scale) const;



private:// Touch handling funtions
	glm::mat4 GetInverseTranspose(const glm::mat4& m) const;

	glm::vec3 GetArcballVector(const glm::vec2& point) const;

	glm::mat4 GetWorldMatrix() const;


private:// Zoom In/Out Button functions
	void btn1_event() const;
	void btn2_event() const;
	void btn3_event() const;
	void btn4_event() const;
	void btn5_event() const;
	void btn6_event() const;


private:// for ObjectStruct functions
	void createObject();

public:// inline functions
	BasicCamera* GetCamera() const;

	void SetTouchPoint(const float& x, const float& y);

	void OnAutoRotate();

	void OffAutoRotate();

	void ToggleRotate();

	void TouchOn();

	void TouchOff();

};

inline BasicCamera* BasicRenderer::GetCamera() const
{
	return mCamera;
}

inline void BasicRenderer::SetTouchPoint(const float& x, const float& y)
{
	LOGI("Touch Point (x,y): (%f,%f)\n", x, y);
	mTouchPoint.x = x;
	mTouchPoint.y = y;
}

inline void BasicRenderer::OnAutoRotate()
{
	mIsAutoRotateEye = true;
}

inline void BasicRenderer::OffAutoRotate()
{
	mIsAutoRotateEye = false;
}

inline void BasicRenderer::ToggleRotate()
{
	mIsAutoRotateEye = !mIsAutoRotateEye;
}

inline void BasicRenderer::TouchOn()
{
	mIsTouchOn = true;
}

inline void BasicRenderer::TouchOff()
{
	mIsTouchOn = false;
}




#endif //__BASIC_RENDERER_H__


