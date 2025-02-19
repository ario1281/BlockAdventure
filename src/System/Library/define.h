#pragma once
/*===============================================

	基本

===============================================*/
#define	SCRW		1280	// ウィンドウ幅（Width
#define	SCRH		720		// ウィンドウ高さ（Height

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <commctrl.h>


/*===============================================

	DirectX

===============================================*/
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dsound.lib")

#define DIRECTINPUT_VERSION 0x0800

#include "d3d9.h"
#include "d3dx9.h"
#include "dsound.h"
#include "dinput.h"


/*===============================================

	STL

===============================================*/
#include <ctime>
#include <map>
#include <unordered_map>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>


/*===============================================

	その他ライブラリ

===============================================*/

//　追加ライブラリ
#include<json11\json11.hpp>
#include<imgui\imgui.h>

#include"..\ImGUI\ImGUIManager.h"

//　その他
#include"Helper.h"

#include"Math.h"
#include"JudgeManager.h"

#include"SpriteManager.h"

#include"SceneManager.h"
#include"CameraManager.h"

#include"Direct3D.h"

#include"SceneManager.h"
#include"TextureManager.h"

#include"MeshManager.h"
#include"SquarePolygon.h"
#include"TrailPolygon.h"

#include"SoundManager.h"
#include"wavread.h"

#include"ResourceFactory.h"

#include"InputComponent.h"

#include"TaskManager.h"
#include"Game\GimmickManager.h"

#include"Easing.h"

//　定義
#define RIGHT_ANGLE (90.0f)	//　直角

static float DotCalculation(const Vector3& v1, const Vector3& v2) {
	Vector3 _v1 = v1, _v2 = v2;
	_v1.Normalize();
	_v2.Normalize();
	_v1.y = 0;
	_v2.y = 0;
	float dot = D3DXVec3Dot(&_v1, &_v2);
	Vector3 vCross;
	D3DXVec3Cross(&vCross, &_v1, &_v2);

	return vCross.y > 0 ? dot : dot *= -1;
}