#include "sample/coloring_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

ColoringView::ColoringView(void* data): SampleView(data)
{
}

void ColoringView::OnInit()
{
	string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_color/color.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_color/color.fs");
	mViewRenderer->SetProgram(vs, fs);

	string teapot = FileLoader::GetInstance()->ReadTxtFile("obj3d/teapot");
	string buildings = FileLoader::GetInstance()->ReadTxtFile("obj3d/buildings");

	TexData textJ;
	FileLoader::GetInstance()->ReadTexture("tex/tizen_black.png", textJ);
	TexData tex;
	FileLoader::GetInstance()->ReadTexture("tex/tex_c_brick.bmp", tex);

/*
	float scale = 1.0f;
	mViewRenderer->SetNewModel(teapot, scale);
	mViewRenderer->SetTexture(TEXDATA_GENERAL, &textJ);
	mViewRenderer->Initialize();

	float scale2 = 10.0f;
	mViewRenderer->SetNewModel2(buildings, scale2);
	mViewRenderer->SetTexture2(TEXDATA_GENERAL, &tex);
	mViewRenderer->Initialize2();
*/
	float scale = 1.0f;
	mViewRenderer->SetNewModel_multi(teapot, scale);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &textJ);
	mViewRenderer->Initialize_multi();

	float scale2 = 10.0f;
	mViewRenderer->SetNewModel_multi(buildings, scale2);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex);
	mViewRenderer->Initialize_multi();



	//mViewRenderer->OffAutoRotate();
	mViewRenderer->GetCamera()->SetEye(25, 20, 25);
	mViewRenderer->GetCamera()->SetAt(0, 20, 0);
}
