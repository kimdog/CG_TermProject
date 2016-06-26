#include "sample/frag_lighting_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

void FragLightingView::OnInit()
{
	string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.fs");
	mViewRenderer->SetProgram(vs, fs);

//	string building4 = FileLoader::GetInstance()->ReadTxtFile("obj3d/buildings");
//	string car = FileLoader::GetInstance()->ReadTxtFile("obj3d/carObject");

	string plane = FileLoader::GetInstance()->ReadTxtFile("obj3d/city_003");
	string city_high = FileLoader::GetInstance()->ReadTxtFile("obj3d/city_001");
	string city_low = FileLoader::GetInstance()->ReadTxtFile("obj3d/city_002");


	// add other text data...
//	TexData textJ;
//	FileLoader::GetInstance()->ReadTexture("tex/room_wood.jpg", textJ);

//	TexData tex;
//	FileLoader::GetInstance()->ReadTexture("tex/tex_c_brick.bmp", tex);

	TexData tex_high;
	FileLoader::GetInstance()->ReadTexture("tex/city_high.jpg", tex_high);
	TexData tex_low;
	FileLoader::GetInstance()->ReadTexture("tex/city_low.jpg", tex_low);
	TexData tex_plane;
	FileLoader::GetInstance()->ReadTexture("tex/city_plane.jpg", tex_plane);



/*
	float scale = 10.0f;
	mViewRenderer->SetNewModel(building4, scale);
	mViewRenderer->SetTexture(TEXDATA_GENERAL, &textJ);
	mViewRenderer->Initialize();
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

	float scale2 = 0.3f;
	mViewRenderer->SetNewModel2(car, scale2);
	mViewRenderer->SetTexture2(TEXDATA_GENERAL, &tex);
	mViewRenderer->Initialize2();

	float scale = 10.0f;
	mViewRenderer->SetNewModel_multi(building4, scale);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &textJ);
	mViewRenderer->Initialize_multi();
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

	float scale2 = 0.3f;
	mViewRenderer->SetNewModel_multi(car, scale2);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex);
	mViewRenderer->Initialize_multi();
*/

	float scale = 1.0f;
	mViewRenderer->SetNewModel_multi(plane, scale);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex_plane);
	mViewRenderer->Initialize_multi();
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

	float scale2 = 1.0f;
	mViewRenderer->SetNewModel_multi(city_high, scale2);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex_high);
	mViewRenderer->Initialize_multi();

	float scale3 = 1.0f;
	mViewRenderer->SetNewModel_multi(city_low, scale3);
	mViewRenderer->SetTexture_multi(TEXDATA_GENERAL, &tex_low);
	mViewRenderer->Initialize_multi();



	//	mViewRenderer->SetNewModel(building1, scale);
//	mViewRenderer->SetNewModel(building8, scale);
//	mViewRenderer->SetNewModel(building10, scale);
//	mViewRenderer->SetNewModel(line2, scale);
//	mViewRenderer->SetNewModel(object010, scale);






	mViewRenderer->GetCamera()->SetEye(25, 20, 25);
	mViewRenderer->GetCamera()->SetAt(0, 20, 0);
}

void FragLightingView::OnStep()
{
	mViewRenderer->RenderFrame();
}

