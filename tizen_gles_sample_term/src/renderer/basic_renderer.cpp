#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "basic/basic_renderer.h"
//#include "basic/test_module.h"
#include "basic/test_forC.h"


using namespace std;
using namespace glm;

// myObjects

BasicRenderer::BasicRenderer() :
	mWidth(0),
	mHeight(0),
	mDeltaTime(0),
	mTimer(NULL),
	mCamera(NULL),
	mShader(NULL),
	mIsAutoRotateEye(true),
	mIsFill(true),
	mIsTouchOn(false),
	mTouchPoint(glm::vec2(0)),
	mVboVertices(0),
	mVboVertices2(0),
	mVboIndices(0),
	mVboIndices2(0),
	mVboTangents(0),
	mVboTangents2(0),
	mVboInstTransforms(0),
	mVboInstTransforms2(0),
	mHasTexture(false),
	mHasTexture2(false),
	mHasCubemap(false),
	mHasCubemap2(false),
	mHasNorMap(false),
	mHasNorMap2(false),
	mHasHeightmap(false),
	mHasHeightmap2(false),
	mTexId(0),
	mTexId2(0),
	mTexCubeId(0),
	mTexCubeId2(0),
	mTexNorId(0),
	mTexNorId2(0),
	mTexHeightId(0),
	mTexHeightId2(0),
	mInstCount(1)
{
	mTimer = new BasicTimer();
	mCamera = new BasicCamera();
	mShader = new BasicShader();
}

BasicRenderer::~BasicRenderer()
{
	delete mTimer;
	delete mShader;
	delete mCamera;
}

// Interface functions
/// Sets vertex shader and fragment shader for rendering
bool BasicRenderer::SetProgram(const std::string& vertexSource, const std::string& fragmentSource) const
{
	mShader->CreateProgram(vertexSource, fragmentSource);

	if (!mShader->GetProgram())
	{
		LOGE("Could not create program.\n");
		return false;
	}

	mShader->Use();

	return true;
}

bool BasicRenderer::SetProgram(const char* vertexSource, const char* fragmentSource) const
{
	mShader->CreateProgram(vertexSource, fragmentSource);

	if (!mShader->GetProgram())
	{
		LOGE("Could not create program.\n");
		return false;
	}

	mShader->Use();

	return true;
}

/****************************
 **** Interface functions ***
 ****************************/
void BasicRenderer::SetNewModel(const std::string& objSource, const float& scale)
{
	char* buffer = new char[objSource.length() + 1];
	strcpy(buffer, objSource.c_str());

	SetNewModel(buffer, scale);

	delete (buffer);
}

void BasicRenderer::SetNewModel2(const std::string& objSource, const float& scale)
{
	char* buffer = new char[objSource.length() + 1];
	strcpy(buffer, objSource.c_str());

	SetNewModel2(buffer, scale);

	delete (buffer);
}

void BasicRenderer::SetNewModel_multi(const std::string& objSource, const float& scale)
{
	char* buffer = new char[objSource.length() + 1];
	strcpy(buffer, objSource.c_str());

	SetNewModel_multi(buffer, scale);

	delete (buffer);
}

void BasicRenderer::SetNewModel(char* objSource)
{
	ImportModel(objSource, this);
}


void BasicRenderer::SetNewModel(char* objSource, const float& scale)
{

	ImportModelScale(objSource, this, scale);
	mHasTexture = true;


}

void BasicRenderer::SetNewModel2(char* objSource, const float& scale)
{
	ImportModelScale2(objSource, this, scale);
	mHasTexture = true;
}

void BasicRenderer::SetNewModel_multi(char* objSource, const float& scale)
{
	createObject();	// create object and add on myObjects

	ImportModelScale_multi(objSource, this, scale);
	//mHasTexture = true;
	//renderer->myObjects.back().myHasTexture = true;
	myObjects.back().myHasTexture = true;

}


void BasicRenderer::SetTexture(TexData_Type type, const TexData* newTex)
{
	switch (type)
	{
	case TEXDATA_GENERAL: // general texture
		LOGI("Set Texture : general\n");
		mHasTexture = true;
		CreateTexBuffer(*newTex, mTexId);
		break;
	case TEXDATA_NORMAL_MAP: // normal map
		LOGI("Set Texture : normal map\n");
		mHasNorMap = true;
		if (mTangentBuffer.empty()) ComputeTangent();
		CreateTexBuffer(*newTex, mTexNorId);
		break;
	default:
		break;
	}
}

void BasicRenderer::SetTexture2(TexData_Type type, const TexData* newTex)
{
	switch (type)
	{
	case TEXDATA_GENERAL: // general texture
		LOGI("Set Texture : general\n");
		mHasTexture2 = true;
		CreateTexBuffer(*newTex, mTexId2);
		break;
	case TEXDATA_NORMAL_MAP: // normal map
		LOGI("Set Texture : normal map\n");
		mHasNorMap2 = true;
		if (mTangentBuffer2.empty()) ComputeTangent();
		CreateTexBuffer(*newTex, mTexNorId2);
		break;
	default:
		break;
	}
}

void BasicRenderer::SetTexture_multi(TexData_Type type, const TexData* newTex)
{
	switch (type)
	{
	case TEXDATA_GENERAL: // general texture
		LOGI("Set Texture : general\n");
		myObjects.back().myHasTexture = true;
		CreateTexBuffer(*newTex, myObjects.back().myTexId);
		break;
	case TEXDATA_NORMAL_MAP: // normal map
		LOGI("Set Texture : normal map\n");
		myObjects.back().myHasNorMap = true;
		if (myObjects.back().myTangentBuffer.empty()) ComputeTangent();
		CreateTexBuffer(*newTex, myObjects.back().myTexNorId);
		break;
	default:
		break;
	}
}

bool BasicRenderer::Initialize()
{
	LOGI("Initialize renderer.\n");
	LogInfo();

	CountTickInit();

	CreateVbo();
	SetState();

	return true;
}

bool BasicRenderer::Initialize2()
{
	LOGI("Initialize renderer.\n");
	LogInfo();

	CountTickInit();

	CreateVbo2();
	SetState();

	return true;
}

bool BasicRenderer::Initialize_multi()
{
	LOGI("Initialize renderer.\n");
	LogInfo();

	CountTickInit();

	CreateVbo_multi();
	SetState();

	return true;
}


void BasicRenderer::SetViewPort(int w, int h)
{
	LOGI("SetViewPort(%d, %d)\n", w, h);
	mWidth = w;
	mHeight = h;
	glViewport(0, 0, w, h);
	check_gl_error("glViewport");

	mCamera->ComputePerspective(60.0f, w, h);
}

void BasicRenderer::RenderFrame()
{
	//LOGI("BasicRenderer::RenderFrame()");
	ComputeTick();

	// rotate auto.
	//if (mIsAutoRotateEye) mCamera->RotateAuto(mDeltaTime);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	check_gl_error("glClear");

	PassUniform();

	//for multi Draw
	int i;
	for(i=0;i<myObjects.size();i++) {
		RebindVbo_multi(i);
		Draw(myObjects.at(i).myIndexBuffer);
	}

	//	RebindVbo();
	//	Draw();

	//	RebindVbo2();
	//	Draw(mIndexBuffer2);
}

void BasicRenderer::TogglePolygonMode()
{
#ifdef BASIC_OPENGL
    mIsFill = !mIsFill;
    glPolygonMode(GL_FRONT_AND_BACK, mIsFill ? GL_FILL : GL_LINE);
#endif
}


/*****************************
 ***** Texture functions *****
 *****************************/
void BasicRenderer::CreateTexBuffer(const TexData& newTex, GLuint& target) const
{
	LOGI("BasicRenderer::CreateTexBuffer\n");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	check_gl_error("glPixelStorei");
	glGenTextures(1, &target);
	check_gl_error("glGenTextures");

	glBindTexture(GL_TEXTURE_2D, target);
	check_gl_error("glBindTexture");

	TexBuffer(GL_TEXTURE_2D, newTex);

	glGenerateMipmap(GL_TEXTURE_2D);
	check_gl_error("glGenerateMipmap");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	check_gl_error("glTexParameteri");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check_gl_error("glTexParameteri");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	check_gl_error("glTexParameteri");

	glBindTexture(GL_TEXTURE_2D, 0);
}

void BasicRenderer::TexBuffer(GLenum type, const TexData& newTex) const
{
	LOGI("BasicRenderer::TexBuffer");

	glTexImage2D(type, 0, newTex.format,
	             newTex.width, newTex.height, 0,
	             static_cast<GLenum>(newTex.format), GL_UNSIGNED_BYTE,
	             static_cast<void *>(newTex.pixels));

	check_gl_error("glTexImage2D");
}


/*******************************
 ***** Rendering functions *****
 *******************************/
void BasicRenderer::SetState() const
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void BasicRenderer::CreateVbo()
{
	LOGI("BasicRenderer::RebindVbo\n");
	glGenBuffers(1, &mVboVertices);
	glGenBuffers(1, &mVboIndices);

	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	glBufferData(GL_ARRAY_BUFFER,
	                            static_cast<GLsizeiptr>(mVertexBuffer.size() * sizeof(VertexStruct)),
	                            &(mVertexBuffer.at(0)), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	                                    static_cast<GLsizeiptr>(mIndexBuffer.size() * sizeof(GLushort)),
	                                    &(mIndexBuffer.at(0)), GL_STATIC_DRAW);


	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	// If renderer has texture, we should enable vertex attribute for texCoord
	if (mHasTexture || mHasNorMap)
	{
		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TEX);
		glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexId);
	}

	if (mHasNorMap)
	{
		// Bump mapping need to change space (world and TBN)
		// mTangentBuffer calculated by ComputeTangent() when normal texture has set
		glGenBuffers(1, &mVboTangents);
		glBindBuffer(GL_ARRAY_BUFFER, mVboTangents);
		glBufferData(GL_ARRAY_BUFFER,
		                            static_cast<GLsizeiptr>(mTangentBuffer.size() * sizeof(vec3)), &(mTangentBuffer.at(0)), GL_STATIC_DRAW);

		offset = 0;
		stride = sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TANGENT);
		glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
		glBindTexture(GL_TEXTURE_2D, mTexNorId);
	}
}


void BasicRenderer::CreateVbo2()
{
	LOGI("BasicRenderer::RebindVbo\n");
	glGenBuffers(1, &mVboVertices2);
	glGenBuffers(1, &mVboIndices2);

	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices2);
	glBufferData(GL_ARRAY_BUFFER,
	                            static_cast<GLsizeiptr>(mVertexBuffer2.size() * sizeof(VertexStruct)),
	                            &(mVertexBuffer2.at(0)), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	                                    static_cast<GLsizeiptr>(mIndexBuffer2.size() * sizeof(GLushort)),
	                                    &(mIndexBuffer2.at(0)), GL_STATIC_DRAW);


	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	// If renderer has texture, we should enable vertex attribute for texCoord
	if (mHasTexture2 || mHasNorMap2)
	{
		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TEX);
		glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexId2);
	}

	if (mHasNorMap2)
	{
		// Bump mapping need to change space (world and TBN)
		// mTangentBuffer calculated by ComputeTangent() when normal texture has set
		glGenBuffers(1, &mVboTangents2);
		glBindBuffer(GL_ARRAY_BUFFER, mVboTangents2);
		glBufferData(GL_ARRAY_BUFFER,
		                            static_cast<GLsizeiptr>(mTangentBuffer2.size() * sizeof(vec3)), &(mTangentBuffer2.at(0)), GL_STATIC_DRAW);

		offset = 0;
		stride = sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TANGENT);
		glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
		glBindTexture(GL_TEXTURE_2D, mTexNorId2);
	}
}


void BasicRenderer::CreateVbo_multi()
{
	LOGI("BasicRenderer::RebindVbo\n");
	glGenBuffers(1, &myObjects.back().myVboVertices);
	glGenBuffers(1, &myObjects.back().myVboIndices);

	glBindBuffer(GL_ARRAY_BUFFER, myObjects.back().myVboVertices);
	glBufferData(GL_ARRAY_BUFFER,
	                            static_cast<GLsizeiptr>(myObjects.back().myVertexBuffer.size() * sizeof(VertexStruct)),
	                            &(myObjects.back().myVertexBuffer.at(0)), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myObjects.back().myVboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	                                    static_cast<GLsizeiptr>(myObjects.back().myIndexBuffer.size() * sizeof(GLushort)),
	                                    &(myObjects.back().myIndexBuffer.at(0)), GL_STATIC_DRAW);


	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	// If renderer has texture, we should enable vertex attribute for texCoord
	if (myObjects.back().myHasTexture || myObjects.back().myHasNorMap)
	{
		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TEX);
		glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myObjects.back().myTexId);
	}

	if (myObjects.back().myHasNorMap)
	{
		// Bump mapping need to change space (world and TBN)
		// mTangentBuffer calculated by ComputeTangent() when normal texture has set
		glGenBuffers(1, &myObjects.back().myVboTangents);
		glBindBuffer(GL_ARRAY_BUFFER, myObjects.back().myVboTangents);
		glBufferData(GL_ARRAY_BUFFER,
		                            static_cast<GLsizeiptr>(myObjects.back().myTangentBuffer.size() * sizeof(vec3)), &(myObjects.back().myTangentBuffer.at(0)), GL_STATIC_DRAW);

		offset = 0;
		stride = sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TANGENT);
		glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
		glBindTexture(GL_TEXTURE_2D, myObjects.back().myTexNorId);
	}
}


void BasicRenderer::RebindVbo() const
{
	LOGI("BasicRenderer::RebindVbo\n");
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices);

	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	// If renderer has texture, we should enable vertex attribute for texCoord
	if (mHasTexture || mHasNorMap)
	{
		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TEX);
		glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexId);
	}

	if (mHasNorMap)
	{
		// Bump mapping need to change space (world and TBN)
		// mTangentBuffer calculated by ComputeTangent() when normal texture has set
		glBindBuffer(GL_ARRAY_BUFFER, mVboTangents);

		offset = 0;
		stride = sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TANGENT);
		glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
		glBindTexture(GL_TEXTURE_2D, mTexNorId);
	}
}


void BasicRenderer::RebindVbo2() const
{
	LOGI("BasicRenderer::RebindVbo\n");
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndices2);

	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	// If renderer has texture, we should enable vertex attribute for texCoord
	if (mHasTexture2 || mHasNorMap2)
	{
		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TEX);
		glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexId2);
	}

	if (mHasNorMap2)
	{
		// Bump mapping need to change space (world and TBN)
		// mTangentBuffer calculated by ComputeTangent() when normal texture has set
		glBindBuffer(GL_ARRAY_BUFFER, mVboTangents2);

		offset = 0;
		stride = sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TANGENT);
		glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
		glBindTexture(GL_TEXTURE_2D, mTexNorId2);
	}
}


void BasicRenderer::RebindVbo_multi(const int& i) const
{
	LOGI("BasicRenderer::RebindVbo\n");
	glBindBuffer(GL_ARRAY_BUFFER, myObjects.at(i).myVboVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myObjects.at(i).myVboIndices);

	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;
	glEnableVertexAttribArray(V_ATTRIB_POSITION);
	glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	offset += sizeof(vec3);
	glEnableVertexAttribArray(V_ATTRIB_NORMAL);
	glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

	// If renderer has texture, we should enable vertex attribute for texCoord
	if (myObjects.at(i).myHasTexture || myObjects.at(i).myHasNorMap)
	{
		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TEX);
		glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myObjects.at(i).myTexId);
	}

	if (myObjects.at(i).myHasNorMap)
	{
		// Bump mapping need to change space (world and TBN)
		// mTangentBuffer calculated by ComputeTangent() when normal texture has set
		glBindBuffer(GL_ARRAY_BUFFER, myObjects.at(i).myVboTangents);

		offset = 0;
		stride = sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_TANGENT);
		glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offset));

		glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
		glBindTexture(GL_TEXTURE_2D, myObjects.at(i).myTexNorId);
	}
}


mat4 BasicRenderer::GetInverseTranspose(const mat4& m) const
{
	return transpose(inverse(m));

}


void BasicRenderer::PassUniform() const
{
	// check button event
	btn1_event();
	btn2_event();
	btn3_event();
	btn4_event();
	btn5_event();
	btn6_event();


	mat4 worldMat = GetWorldMatrix();
	mat4 viewMat = mCamera->GetViewMat();
	mat4 projMat = mCamera->GetPerspectiveMat();
	mat4 invTransWorldMat = GetInverseTranspose(worldMat);

	mShader->SetUniform("worldMat", worldMat);
	mShader->SetUniform("viewMat", viewMat);
	mShader->SetUniform("projMat", projMat);
	mShader->SetUniform("invTransWorldMat", invTransWorldMat);
	mShader->SetUniform("s_tex0", 0);
	mShader->SetUniform("s_texNor", TEX_POS_NORMAL);
	mShader->SetUniform("s_texCube", TEX_POS_CUBEMAP);
	mShader->SetUniform("eyePos", mCamera->GetEye());
	mShader->SetUniform("lightPos", vec3(50.0f, 50.0f, 50.0f));
	mShader->SetUniform("materialDiff", vec3(0.8f, 1.0f, 0.7f));
	mShader->SetUniform("materialSpec", vec3(0.8f, 1.0f, 0.7f));
	mShader->SetUniform("materialAmbi", vec3(0.0f, 0.0f, 0.0f));
	mShader->SetUniform("materialEmit", vec3(0.0f, 0.0f, 0.0f));
	mShader->SetUniform("materialSh", 100.0f);
	mShader->SetUniform("sourceDiff", vec3(0.7f, 0.7f, 0.7f));
	mShader->SetUniform("sourceSpec", vec3(1.0f, 1.0f, 1.0f));
	mShader->SetUniform("sourceAmbi", vec3(0.0f, 0.0f, 0.0f));
}

void BasicRenderer::Draw() const
{
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexBuffer.size()), GL_UNSIGNED_SHORT, static_cast<GLvoid *>(NULL));
	check_gl_error("glDrawElements");
}

void BasicRenderer::Draw2() const
{
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexBuffer2.size()), GL_UNSIGNED_SHORT, static_cast<GLvoid *>(NULL));
	check_gl_error("glDrawElements2");
}

// add case : specific mIndexBuffer
void BasicRenderer::Draw(vector<GLushort> mIndexBuffer) const
{
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndexBuffer.size()), GL_UNSIGNED_SHORT, static_cast<GLvoid *>(NULL));
	check_gl_error("glDrawElements");
}

/*****************************
 ***** Utility functions *****
 *****************************/
void BasicRenderer::LogInfo() const
{
	print_gl_string("Version", GL_VERSION);
	print_gl_string("Vendor", GL_VENDOR);
	print_gl_string("Renderer", GL_RENDERER);
	print_gl_string("Extensions", GL_EXTENSIONS);
	print_gl_string("GLSLversion", GL_SHADING_LANGUAGE_VERSION);
}

void BasicRenderer::CountTickInit() const
{
	mTimer->InitTimer();
}

void BasicRenderer::ComputeTick()
{
	static double lastTick = 0;
	double currTick = mTimer->GetElapsedTime();
	mDeltaTime = currTick - lastTick;
	lastTick = currTick;
	//LOGI("Tick: %f\n", mDeltaTime);
}

/// Computes Tangent vector by using position and texCoord
//  These vectors will be used for making TBN rotation matrix
void BasicRenderer::ComputeTangent()
{
	vector<vec3> triTangents;

	// Compute Tangent Basis
	for (unsigned int i = 0; i < mIndexBuffer.size(); i += 3)
	{
		vec3 p0 = mVertexBuffer.at(mIndexBuffer.at(i)).pos;
		vec3 p1 = mVertexBuffer.at(mIndexBuffer.at(i + 1)).pos;
		vec3 p2 = mVertexBuffer.at(mIndexBuffer.at(i + 2)).pos;

		vec3 uv0 = vec3(mVertexBuffer.at(mIndexBuffer.at(i)).tex, 0);
		vec3 uv1 = vec3(mVertexBuffer.at(mIndexBuffer.at(i + 1)).tex, 0);
		vec3 uv2 = vec3(mVertexBuffer.at(mIndexBuffer.at(i + 2)).tex, 0);

		vec3 deltaPos1 = p1 - p0;
		vec3 deltaPos2 = p2 - p0;

		vec3 deltaUV1 = uv1 - uv0;
		vec3 deltaUV2 = uv2 - uv0;

		// Compute the tangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec3 computedTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
	}

	// Initialize mTangents
	for (int i = 0; i < mVertexBuffer.size(); ++i)
	{
		mTangentBuffer.push_back(vec3(0));
	}

	// Accumulate tangents by indices
	for (unsigned int i = 0; i < mIndexBuffer.size(); ++i)
	{
		mTangentBuffer.at(mIndexBuffer.at(i))
			= mTangentBuffer.at(mIndexBuffer.at(i)) + triTangents.at(i);
	}
}

void BasicRenderer::ComputeTangent2()
{
	vector<vec3> triTangents;

	// Compute Tangent Basis
	for (unsigned int i = 0; i < mIndexBuffer2.size(); i += 3)
	{
		vec3 p0 = mVertexBuffer2.at(mIndexBuffer2.at(i)).pos;
		vec3 p1 = mVertexBuffer2.at(mIndexBuffer2.at(i + 1)).pos;
		vec3 p2 = mVertexBuffer2.at(mIndexBuffer2.at(i + 2)).pos;

		vec3 uv0 = vec3(mVertexBuffer2.at(mIndexBuffer2.at(i)).tex, 0);
		vec3 uv1 = vec3(mVertexBuffer2.at(mIndexBuffer2.at(i + 1)).tex, 0);
		vec3 uv2 = vec3(mVertexBuffer2.at(mIndexBuffer2.at(i + 2)).tex, 0);

		vec3 deltaPos1 = p1 - p0;
		vec3 deltaPos2 = p2 - p0;

		vec3 deltaUV1 = uv1 - uv0;
		vec3 deltaUV2 = uv2 - uv0;

		// Compute the tangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec3 computedTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
	}

	// Initialize mTangents
	for (int i = 0; i < mVertexBuffer2.size(); ++i)
	{
		mTangentBuffer2.push_back(vec3(0));
	}

	// Accumulate tangents by indices
	for (unsigned int i = 0; i < mIndexBuffer2.size(); ++i)
	{
		mTangentBuffer2.at(mIndexBuffer2.at(i))
			= mTangentBuffer2.at(mIndexBuffer2.at(i)) + triTangents.at(i);
	}
}



void BasicRenderer::ImportModel(char* objSource, BasicRenderer* renderer) const
{
	ImportModelScale(objSource, renderer, 1.0f);
}

void BasicRenderer::ImportModelScale(char* objSource, BasicRenderer* renderer, const float& scale) const
{
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector<string> strIndexer;

	float x, y, z;
	char *line, *linePtr;
	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line)
	{
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0])
		{
		case 'v':
			switch (word[1])
			{
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr)))
			{
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
				                                                   strIndexer.end(), word);

				if (findIter != strIndexer.end())
				{
					// this face already in vertex data
					renderer->mIndexBuffer.push_back(std::distance(strIndexer.begin(), findIter));
				}
				else
				{
					unsigned short face_index = static_cast<unsigned short>(strIndexer.size());
					renderer->mIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);

					renderer->mVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}
}


void BasicRenderer::ImportModelScale2(char* objSource, BasicRenderer* renderer, const float& scale) const
{
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector<string> strIndexer;

	float x, y, z;
	char *line, *linePtr;
	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line)
	{
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0])
		{
		case 'v':
			switch (word[1])
			{
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr)))
			{
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
				                                                   strIndexer.end(), word);

				if (findIter != strIndexer.end())
				{
					// this face already in vertex data
					renderer->mIndexBuffer2.push_back(std::distance(strIndexer.begin(), findIter));
				}
				else
				{
					unsigned short face_index = static_cast<unsigned short>(strIndexer.size());
					renderer->mIndexBuffer2.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);

					renderer->mVertexBuffer2.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}
}


void BasicRenderer::ImportModelScale_multi(char* objSource, BasicRenderer* renderer, const float& scale) const
{
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector<string> strIndexer;

	float x, y, z;
	char *line, *linePtr;
	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line)
	{
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0])
		{
		case 'v':
			switch (word[1])
			{
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr)))
			{
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
				                                                   strIndexer.end(), word);

				if (findIter != strIndexer.end())
				{
					// this face already in vertex data
					renderer->myObjects.back().myIndexBuffer.push_back(std::distance(strIndexer.begin(), findIter));
				}
				else
				{
					unsigned short face_index = static_cast<unsigned short>(strIndexer.size());
					renderer->myObjects.back().myIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);

					renderer->myObjects.back().myVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}
}



vec3 BasicRenderer::GetArcballVector(const vec2& point) const
{
	const float RADIUS = 1.0;

	vec3 P = vec3(1.0 * point.x / mWidth * 2 - 1.0,
	              1.0 * point.y / mHeight * 2 - 1.0,
	              0);
	P.y = -P.y;

	float OP_squared = P.x * P.x + P.y * P.y;
	if (OP_squared <= RADIUS * RADIUS)
		P.z = sqrt(RADIUS * RADIUS - OP_squared); // Pythagore
	else
		P = glm::normalize(P); // nearest point

	return P;
}

mat4 BasicRenderer::GetWorldMatrix() const
{
	static quat startRotQuat = quat_cast(mat3(1));
	static quat lastRotQuat = startRotQuat;
	static vec2 ancPts = mTouchPoint;	//mTouchPoint
	static bool isUpdateAnc = false;

	if (mIsTouchOn)
	{
		if (!isUpdateAnc)
		{
			ancPts = mTouchPoint;
			isUpdateAnc = true;
			LOGI("Anchor Updated\n");
		}
		else
		{
			if (ancPts.x != mTouchPoint.x || ancPts.y != mTouchPoint.y)
			{
				float subtractX = mTouchPoint.x - ancPts.x;
			//	float subtractY = mTouchPoint.y - ancPts.y;

				if(subtractX >= 0) {
					mCamera->ViewRotate(1.0f);
				}
				else {
					mCamera->ViewRotate(-1.0f);
				}

	//			if(subtractY >= 0) {
	//				mCamera->ViewRotateY(0.5f);
	//			}
	//			else {
	//				mCamera->ViewRotateY(-0.5f);
	//			}

				/*
				// Get the vectors on the arcball
				vec3 va = GetArcballVector(ancPts);
				vec3 vb = GetArcballVector(mTouchPoint);

				// Get the rotation axis and the angle between the vector
				float angle = acos(glm::min(1.0f, dot(va, vb))) * 2.0f;

				vec3 axisInCameraSpace = normalize(cross(va, vb));

				mat4 cameraToObjectSpace = inverse(GetCamera()->GetViewMat());
				vec3 axisInObjectSpace = normalize(mat3(cameraToObjectSpace) * axisInCameraSpace);

				quat curRotQuat = angleAxis(angle, axisInObjectSpace);
				lastRotQuat = normalize(curRotQuat * startRotQuat);
				*/

			}
		}
	}
	else
	{
		startRotQuat = lastRotQuat;
		isUpdateAnc = false;
	}

	mat4 rotationMat = mat4_cast(lastRotQuat);

	return rotationMat;
}

extern ZoomBtn btns;

// button event
void BasicRenderer::btn1_event() const
{
	//bool mIsBtn1 = TransZoom::mIsBtn1;
	bool mIsBtn1 = false;
	if(btns.Btn1 == 1) {
		mIsBtn1 = true;
	}

	if(mIsBtn1)
	{
		//mCamera->ViewRotate(-1.0f);
		//mCamera->ZoomIn(0.5f);
		//mCamera->MoveForward(0.5f);
		mCamera->MoveLeft(1.0f);
		//TransZoom::mIsBtn1 = false;
		btns.Btn1 = 0;
	}
}

void BasicRenderer::btn2_event() const
{
	//bool mIsBtn2 = TransZoom::mIsBtn2;
	bool mIsBtn2 = false;
	if(btns.Btn2 == 1) {
		mIsBtn2 = true;
	}

	if(mIsBtn2)
	{
		//mCamera->ViewRotate(1.0f);
		//mCamera->ZoomOut(1.0f);
		mCamera->MoveRight(1.0f);
		//TransZoom::mIsBtn2 = false;
		btns.Btn2 = 0;
	}
}

void BasicRenderer::btn3_event() const
{
	//bool mIsBtn2 = TransZoom::mIsBtn2;
	bool mIsBtn3 = false;
	if(btns.Btn3 == 1) {
		mIsBtn3 = true;
	}

	if(mIsBtn3)
	{
		//mCamera->ZoomOut(0.5f);
		mCamera->MoveForward(2.0f);
		//mCamera->MoveBackward(0.5f);
		//TransZoom::mIsBtn3 = false;
		btns.Btn3 = 0;
	}
}


void BasicRenderer::btn4_event() const
{
	bool mIsBtn4 = false;
	if(btns.Btn4 == 1) {
		mIsBtn4 = true;
	}

	if(mIsBtn4)
	{
		//mCamera->MoveForward(1.0f);
		mCamera->MoveBackward(2.0f);
		btns.Btn4 = 0;
	}
}

void BasicRenderer::btn5_event() const
{
	bool mIsBtn5 = false;
	if(btns.Btn5 == 1) {
		mIsBtn5 = true;
	}

	if(mIsBtn5)
	{
		//mCamera->MoveForward(1.0f);
		mCamera->ViewRotateY(-0.5f);
		btns.Btn5 = 0;
	}
}

void BasicRenderer::btn6_event() const
{
	bool mIsBtn6 = false;
	if(btns.Btn6 == 1) {
		mIsBtn6 = true;
	}

	if(mIsBtn6)
	{
		//mCamera->MoveForward(1.0f);
		mCamera->ViewRotateY(0.5f);
		btns.Btn6 = 0;
	}
}


// for Object Struct Functions
void BasicRenderer::createObject()
{
	ObjectSturct tempObject;

	tempObject.myVboVertices = mVboVertices;
	tempObject.myVboIndices = mVboIndices;
	tempObject.myVboTangents = mVboTangents;
	tempObject.myVboInstTransforms = mVboInstTransforms;
	tempObject.myHasTexture = mHasTexture;
	tempObject.myHasCubemap = mHasCubemap;
	tempObject.myHasNorMap = mHasNorMap;
	tempObject.myHasHeightmap = mHasHeightmap;
	tempObject.myTexId = mTexId;
	tempObject.myTexCubeId = mTexCubeId;
	tempObject.myTexNorId = mTexNorId;
	tempObject.myTexHeightId = mTexHeightId;

	// vertex buffer
	// std::vector<VertexStruct> myVertexBuffer;
	// std::vector<GLushort> myIndexBuffer;
	// std::vector<glm::vec3> myTangentBuffer;

	// add 1 Object on myObjects
	myObjects.push_back(tempObject);

}
