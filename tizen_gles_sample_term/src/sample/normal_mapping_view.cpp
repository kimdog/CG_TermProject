#include "sample/normal_mapping_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

void NormalMappingView::OnInit()
{
	string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.fs");
	mViewRenderer->SetProgram(vs, fs);

	string building4 = FileLoader::GetInstance()->ReadTxtFile("obj3d/buildings");
	string car = FileLoader::GetInstance()->ReadTxtFile("obj3d/carObject");


//	TexData tex, normal_tex;
//	FileLoader::GetInstance()->ReadTexture("tex/room_wood.jpg", tex);
//	FileLoader::GetInstance()->ReadTexture("tex/tex_n_brick.bmp", normal_tex);
	TexData tex_building;
	FileLoader::GetInstance()->ReadTexture("tex/room_wood.jpg", tex_building);

	TexData tex_car;
	FileLoader::GetInstance()->ReadTexture("tex/tex_c_brick.bmp", tex_car);


	float scale = 10.0f;
	mViewRenderer->SetNewModel_multi(building4, scale);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex_building);
	mViewRenderer->Initialize_multi();
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

	float scale2 = 0.3f;
	mViewRenderer->SetNewModel_multi(car, scale2);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex_car);
	mViewRenderer->Initialize_multi();


	mViewRenderer->GetCamera()->SetEye(25, 20, 25);
	mViewRenderer->GetCamera()->SetAt(0, 20, 0);
}

void NormalMappingView::OnStep()
{
	mViewRenderer->RenderFrame();
}

