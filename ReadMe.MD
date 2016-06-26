#Term Project Final Report

Member: 컴퓨터학과 2013210107 김정수

##Topic : 3D travel Simulation ( by Tizen with openGL)

**Features :**

- 1인칭
- 자유롭게 지정된 맵을 돌아다니기
- 각종 맵 선

![참고1](https://github.com/kimdog/CG_TermProject/blob/master/images/term_3.png)
![참고2](https://github.com/kimdog/CG_TermProject/blob/master/images/term_4.png)




**Contents :**

- 메뉴 기능을 통한 맵 선택
- 좌우, 상하 시선이동 ( mAt 좌표 변환, mBodyForward 유지 )
- 이동 ( 앞 뒤 좌 우 )
- 배경음악 On/Off ( 무한 반복 재생 )
- 여러 Object를 복수의 Vbo를 통해 Load 



**Process :**

1. 기본 수업 Homework template 사용

2. test_forC.h : 
	Button event를 다른 class에서 처리하기 위한 Btn 공유변수.

3. basic_camera.cpp : 
	몸의 이동을 위한 Move관련 함수들 수정.
	시선 이동을 위한  ViewRotate,  ViewRotateY 함수 구현.
	( mAt과 mEye의 높이 차를 이용하여 상하 시선 제한 )

4. basic_renderer.cpp : 
	기존 Vbo 생성, bind 및 처리 함수들을 Object의 개수만큼 
	Vbo를 생성하기 위해 _multi 버전으로 재구현 ? 
	sample.c 코드 변화.
	( 하나의 object를 표현하는 ObjectSturct에 대하여
	std::vector<ObjectSturct> myObjects  여러 Object 저장 벡터. )

5. 3D max를 통하여 각 Object의 vertex, texture... 
	데이터들을 obj파일 형태로 저장 후 불러오기 .

6. User_callbacks.c 의 wav_player.h를 이용한 배경음악 기능.
	기본 template를 사용하되, 무한 반복재생을 위한 코드 수정.

