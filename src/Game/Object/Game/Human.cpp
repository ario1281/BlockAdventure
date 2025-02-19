#include"Human.h"
#include"Game\Scene\TitleScene.h"

Human::Human()
{
	m_tag		= "Player";
	m_name		= "Human";
	m_spModel	= ResFac.GetModel("data\\field\\human\\human.x");
	m_mWorld.CreateIdentity();
	//m_mWorld.(0, 0, 0);
}

void Human::Deserialize(json11::Json &jObj)
{
	GameObject::Deserialize(jObj);
}

bool Human::Update(const std::list<shared_ptr<GameObject>>* s_obj)
{
	if (TASK.GetGoalFlag()) {
		auto input = m_wpInput.lock();
		if (input->Button(input->Bt_1) == input->UP)
		{
			APP.SetChangeScene(new TitleScene());
			return false;
		}
		return true;
	}

	//　スイッチが押されたか
	if (GIMMICK.GetFlag())
	{
		Matrix rot;
		rot = m_mWorld;
		GIMMICK.Rotation(m_mWorld);
		m_mWorld.CreateTranslation(m_mWorld.GetPos());

		m_mWorld.SetBillBoard(rot);
		return true;
	}

	m_isKey = false;
	bool flg = false;
	Vector3 vMove(0, 0, 0);
	if (Moving(vMove, s_obj))
	{
		m_isKey = true;
		TurnAround(vMove);
	}

	Falling(s_obj);

	for (auto& obj = s_obj->begin(); obj != s_obj->end(); obj++)
	{
		auto o = obj->get();
		if (o->GetTag() != "Field") { continue; }

		bool hit = false;
		float dist = 0.0f;
		Vector3 pos = m_mWorld.GetPos();
		JudgeSphereTriangle(o->GetModel(), o->GetWorldMatrix(), pos, 0.5f);
		m_mWorld.SetPos(pos);
	}
	//ImGui();
	return true;
}

void Human::Draw3D()
{
	D3D.SetWorldMatrix(&m_mWorld);
	m_spModel->Draw();
}

void Human::ImGui()
{
	ImGui::Begin((m_name + "Class_ToolBox").c_str());
	auto pos = m_mWorld.GetPos();
	ImGui::Text("Position x:%2f y:%2f z:%2f", pos.x, pos.y, pos.z);

	ImGui::End();
}

//　動作関数
bool Human::Moving(Vector3& vec, const std::list<shared_ptr<GameObject>>* s_obj)
{
	Matrix mRot;
	mRot.CreateRotationY(D3DXToRadian(CURSOR.Vertical()));
	//　キーが押されたか
	bool flg = false;
	auto input = m_wpInput.lock();
	if (input->Button(input->Bt_Up)		== input->KEEP)
	{
		Vector3 vTmp(0, 0, 1);
		vTmp.TransNormal(mRot);
		vec += vTmp;
		flg = true;
	}
	if (input->Button(input->Bt_Left)	== input->KEEP)
	{
		Vector3 vTmp(-1, 0, 0);
		vTmp.TransNormal(mRot);
		vec += vTmp;
		flg = true;
	}
	if (input->Button(input->Bt_Down)	== input->KEEP)
	{
		Vector3 vTmp(0, 0, -1);
		vTmp.TransNormal(mRot);
		vec += vTmp;
		flg = true;
	}
	if (input->Button(input->Bt_Right)	== input->KEEP)
	{
		Vector3 vTmp(1, 0, 0);
		vTmp.TransNormal(mRot);
		vec += vTmp;
		flg = true;
	}

	vec.Normalize();
	vec *= m_sMove;		//　プレイヤーの速さ
	m_mWorld.Move(vec);

	//for (auto& obj = s_obj->begin(); obj != s_obj->end(); obj++)
	//{
	//	auto o = obj->get();
	//	if (o->GetTag() != "Field") { continue; }

	//	float dist;
	//	bool hit;
	//	Vector3 pos = m_mWorld.GetPos();
	//	JudgeRayWall(o->GetModel(), o->GetWorldMatrix(), vec, pos);
	//	m_mWorld.SetPos(pos);
	//}

	return flg;
}
bool Human::TurnAround(Vector3& vec)
{
	Vector3 vNow(0, 0, 1);
	vNow.TransNormal(m_mWorld);

	vec.Normalize();
	float dotAng = D3DXVec3Dot(&vNow, &vec);

	if (dotAng > 1) { dotAng = 1; }
	if (dotAng < -1) { dotAng = -1; }

	dotAng = (float)D3DXToDegree(acos(dotAng));

	Vector3 vCross;
	D3DXVec3Cross(&vCross, &vNow, &vec);
	vCross.Normalize();

	if (dotAng >= m_sTurn) { dotAng = m_sTurn; }
	if (vCross.y < 0) { dotAng *= -1; }
	m_mWorld.RotateYLocal(D3DXToRadian(dotAng));

	return true;
}
void Human::Falling(const std::list<shared_ptr<GameObject>>* s_obj)
{
	m_Fall += m_axelFall;
	if (m_Fall > m_maxFall) { m_Fall = m_maxFall; }
	//　落下処理
	m_mWorld.Move(0, -m_Fall, 0);

	//　判定部分
	for (auto& obj = s_obj->begin(); obj != s_obj->end(); obj++)
	{
		auto o = obj->get();
		if (o->GetTag() != "Field") { continue; }

		float dist;
		bool hit;
		Vector3 vFall(0, -m_Fall, 0);
		Vector3 pos = m_mWorld.GetPos();
		JudgeRay(
			o->GetModel(), o->GetWorldMatrix(), vFall, pos, hit, dist);
		if (!hit || dist > m_Dist) { continue; }

		m_Fall = 0.0f;

		//　メッシュに埋もれたときに上に少し上げる
		pos.y += m_Dist - dist;
		m_mWorld.SetPos(pos);
	}
}



void Human::JudgeRay(const LPD3DXMESH &s_lpMesh, const Matrix &s_mTarget, const Vector3 &s_vDir, Vector3 &s_vPos, bool &s_hit, float &s_dist)
{
	Vector3 vdir = s_vDir;
	vdir.Normalize();
	Vector3 vpos = Vector3(s_vPos.x, s_vPos.y + 0.2f, s_vPos.z);

	s_hit = false;
	s_dist = 0.0f;
	JUDGE.JudgingLayToMesh(s_lpMesh, s_mTarget, vpos, vdir,
		&s_hit, nullptr, &s_dist, nullptr);
}

void Human::JudgeRayTest(const LPD3DXMESH& s_lpMesh, const Matrix& s_mTarget, const Vector3& s_vDir, Vector3& s_vPos, const char* coord)
{
	Vector3 vdir = s_vDir;
	Vector3 vpos = Vector3(s_vPos.x, s_vPos.y + 1.0f, s_vPos.z);

	vdir.Normalize();

	bool hit = false;
	float dist = 0.0f;
	const float cdist = m_Dist;
	JUDGE.JudgingLayToMesh(
		s_lpMesh, s_mTarget, vpos, vdir, &hit, nullptr, &dist, nullptr);
	if (hit && cdist < m_Dist)
	{
		if (coord == "x") { s_vPos.x -= cdist - dist; }
		if (coord == "mx") { s_vPos.x += cdist - dist; }
		if (coord == "z") { s_vPos.z -= cdist - dist; }
		if (coord == "mz") { s_vPos.z += cdist - dist; }
	}
}

void Human::JudgeRayWall(const LPD3DXMESH& s_lpMesh, const Matrix& s_mTarget, const Vector3& s_vDir, Vector3& s_vPos)
{
	JudgeRayTest(s_lpMesh, s_mTarget, Vector3(1, 0, 0), s_vPos, "x");
	JudgeRayTest(s_lpMesh, s_mTarget, Vector3(-1, 0, 0), s_vPos, "mx");
	JudgeRayTest(s_lpMesh, s_mTarget, Vector3(0, 0, 1), s_vPos, "z");
	JudgeRayTest(s_lpMesh, s_mTarget, Vector3(0, 0, -1), s_vPos, "mz");
}

void Human::JudgeSphereTriangle(const LPD3DXMESH& s_lpMesh, const Matrix& s_mTarget, Vector3& s_vPos, float s_rad)
{
	Matrix mInv = s_mTarget;
	Vector3 vpos = s_vPos;
	Vector3 vScale = D3DXVECTOR3(1, 0, 0);
	JUDGE.TransformInverse(mInv,vpos, vScale);


	//理想的にはここでAABBか距離判定によって
	//確実に当たっていない場合はreturnする方が効率的

	float sclRate;
	//拡大行列の逆行列によってベクトルが縮小されるため、
	//ベクトルの長さを測ることで縮小率を測る
	sclRate = vScale.Length();

	//縮小率に合わせて衝突半径を修正
	s_rad *= sclRate;

	WORD* pI; char* pV;
	s_lpMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	s_lpMesh->LockVertexBuffer(0, (LPVOID*)&pV);

	//頂点ひとつ分のサイズを取得
	UINT verSize;
	verSize = D3DXGetFVFVertexSize(s_lpMesh->GetFVF());

	//障害物の持つ全ポリゴンを回す
	for (int poly = 0; poly < s_lpMesh->GetNumFaces(); poly++)
	{
		//--------------------------------------
		//ポリゴンのすぐ近くにいるかどうかの判定
		//--------------------------------------
		DWORD verNo[3];
		verNo[0] = *(pI + poly * 3 + 0);
		verNo[1] = *(pI + poly * 3 + 1);
		verNo[2] = *(pI + poly * 3 + 2);

		D3DXVECTOR3 vPos[3];
		memcpy(&vPos[0], pV + verNo[0] * verSize, sizeof(D3DXVECTOR3));
		memcpy(&vPos[1], pV + verNo[1] * verSize, sizeof(D3DXVECTOR3));
		memcpy(&vPos[2], pV + verNo[2] * verSize, sizeof(D3DXVECTOR3));

		Vector3 vEdge[3]; //ポリゴンの辺を表すベクトル
		vEdge[0] = vPos[1] - vPos[0];
		vEdge[1] = vPos[2] - vPos[1];
		vEdge[2] = vPos[0] - vPos[2];

		Vector3 vCrsNormal;	//２辺の外積を使って法線を求める
		D3DXVec3Cross(&vCrsNormal, &vEdge[0], &vEdge[1]);
		vCrsNormal.Normalize();

		Vector3 vVerToChar[3];	//頂点からキャラまでのベクトル
		vVerToChar[0] = vpos - vPos[0];
		vVerToChar[1] = vpos - vPos[1];
		vVerToChar[2] = vpos - vPos[2];

		float dist;	//キャラから壁までの最短距離
		dist = D3DXVec3Dot(&vVerToChar[0], &vCrsNormal);

		if (dist < 0)
		{
			dist = -dist;
			vCrsNormal = -vCrsNormal;
		}

		if (dist > s_rad) { continue; }

		//--------------------------------------
		//ポリゴンの正面にいるかどうかの判定
		//--------------------------------------

		Vector3 ptOnWall;	//キャラ座標を壁上に投影した座標(A)
		ptOnWall = vpos - vCrsNormal * dist;

		Vector3 vVerToPt[3];	//各頂点から(A)までのベクトル
		vVerToPt[0] = ptOnWall - vPos[0];
		vVerToPt[1] = ptOnWall - vPos[1];
		vVerToPt[2] = ptOnWall - vPos[2];

		Vector3 vCross[3]; //上記ベクトルを使った外積
		D3DXVec3Cross(&vCross[0], &vEdge[0], &vVerToPt[0]);
		vCross[0].Normalize();
		D3DXVec3Cross(&vCross[1], &vEdge[1], &vVerToPt[1]);
		vCross[1].Normalize();
		D3DXVec3Cross(&vCross[2], &vEdge[2], &vVerToPt[2]);
		vCross[2].Normalize();

		//すべての外積が表向きなら(A)は三角形内にあるので
		//外積同士の角度差が９０度未満かどうかをチェックする
		//ポリゴンはさまざまな方向を向いているので上とは限らない
		float dot1, dot2;
		dot1 = D3DXVec3Dot(&vCross[0], &vCross[1]);
		dot2 = D3DXVec3Dot(&vCross[1], &vCross[2]);

		if (dot1 > 0 && dot2 > 0)
		{
			//　球が三角形にめり込んでいるということなので座標を修正する
			//　法線をワールド軸に変換
			vCrsNormal.TransNormal(s_mTarget);
			vCrsNormal.Normalize();

			//　縮小率を加味しつつキャラを法線方向に押し出す
			s_vPos += vCrsNormal * ((s_rad - dist) / sclRate);

			//　キャラ座標が更新されたのでローカル座標を再計算
			vpos = s_vPos;
			vpos.TransCoord(mInv);

			continue;
		}

		//--------------------------------------
		//ポリゴンの辺の外側近くにいるかどうかの判定
		//--------------------------------------

		for (int i = 0; i < 3; i++)
		{
			//辺の長さを測る
			float lengthEdge;
			lengthEdge = D3DXVec3Length(&vEdge[i]);

			//今後の計算のために辺のベクトルを正規化
			D3DXVec3Normalize(&vEdge[i], &vEdge[i]);

			//ポリゴン辺とキャラ座標を結ぶ直角三角形に関して計算する

			float LenOnEdge;	//直角三角形の底辺の長さ
			LenOnEdge = D3DXVec3Dot(&vVerToChar[i], &vEdge[i]);

			//辺の長さ以上に離れていたら衝突していない
			if (LenOnEdge < 0 || LenOnEdge > lengthEdge) { continue; }

			float lenVerToChar;	//直角三角形の斜辺の長さ
			lenVerToChar = D3DXVec3Length(&vVerToChar[i]);

			float lenEdgeToChar;	//直角三角形の高さを求める
			lenEdgeToChar = sqrt(lenVerToChar * lenVerToChar - LenOnEdge * LenOnEdge);

			if (lenEdgeToChar < s_rad)	//辺に球が食い込んでいる
			{
				//辺に近づきすぎた分、押し返す
				Vector3 vOut;
				//辺から離れるためのベクトル
				vOut = vpos - (vPos[i] + vEdge[i] * LenOnEdge);
				vOut.Normalize();

				//縮小率を加味しつつキャラを法線方向に押し出す
				s_vPos += vOut * ((s_rad - lenEdgeToChar) / sclRate);

				//キャラ座標が更新されたのでローカル座標を再計算
				vpos = s_vPos;
				vpos.TransCoord(mInv);
			}
		}
	}
	s_lpMesh->UnlockVertexBuffer();
	s_lpMesh->UnlockIndexBuffer();
}
