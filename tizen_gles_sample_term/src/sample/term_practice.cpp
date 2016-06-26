#include "sample/term_practice.h"

#include "basic/basic_file_loader.h"

using namespace std;

void TermPractice::OnInit()
{
	string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_ter/nor.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_ter/nor.fs");
	mViewRenderer->SetProgram(vs, fs);

	string road = FileLoader::GetInstance()->ReadTxtFile("obj3d/road");
	//new
	//string ground = FileLoader::GetInstance()->ReadTxtFile("obj3d/ground");


	TexData tex, normal_tex;
	FileLoader::GetInstance()->ReadTexture("tex/tex_c_brick.bmp", tex);
	FileLoader::GetInstance()->ReadTexture("tex/tex_n_brick.bmp", normal_tex);

	float scale = 1.0f; // original 1.0f
	mViewRenderer->SetNewModel_multi(road, scale);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex);
	mViewRenderer->SetTexture_multi(TEXDATA_NORMAL_MAP, &normal_tex);

//	mViewRenderer->SetNewModel(ground, scale);

	mViewRenderer->Initialize();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	mViewRenderer->GetCamera()->SetEye(0, 20, 0);
//	mViewRenderer->GetCamera()->SetEye(60, 60, 60);
	mViewRenderer->GetCamera()->SetAt(25, 20, 0);
}

void TermPractice::OnStep()
{
	mViewRenderer->RenderFrame();
}

