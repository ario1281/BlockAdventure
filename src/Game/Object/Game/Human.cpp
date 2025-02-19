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

	//�@�X�C�b�`�������ꂽ��
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

//�@����֐�
bool Human::Moving(Vector3& vec, const std::list<shared_ptr<GameObject>>* s_obj)
{
	Matrix mRot;
	mRot.CreateRotationY(D3DXToRadian(CURSOR.Vertical()));
	//�@�L�[�������ꂽ��
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
	vec *= m_sMove;		//�@�v���C���[�̑���
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
	//�@��������
	m_mWorld.Move(0, -m_Fall, 0);

	//�@���蕔��
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

		//�@���b�V���ɖ����ꂽ�Ƃ��ɏ�ɏ����グ��
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


	//���z�I�ɂ͂�����AABB����������ɂ����
	//�m���ɓ������Ă��Ȃ��ꍇ��return������������I

	float sclRate;
	//�g��s��̋t�s��ɂ���ăx�N�g�����k������邽�߁A
	//�x�N�g���̒����𑪂邱�Ƃŏk�����𑪂�
	sclRate = vScale.Length();

	//�k�����ɍ��킹�ďՓ˔��a���C��
	s_rad *= sclRate;

	WORD* pI; char* pV;
	s_lpMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	s_lpMesh->LockVertexBuffer(0, (LPVOID*)&pV);

	//���_�ЂƂ��̃T�C�Y���擾
	UINT verSize;
	verSize = D3DXGetFVFVertexSize(s_lpMesh->GetFVF());

	//��Q���̎��S�|���S������
	for (int poly = 0; poly < s_lpMesh->GetNumFaces(); poly++)
	{
		//--------------------------------------
		//�|���S���̂����߂��ɂ��邩�ǂ����̔���
		//--------------------------------------
		DWORD verNo[3];
		verNo[0] = *(pI + poly * 3 + 0);
		verNo[1] = *(pI + poly * 3 + 1);
		verNo[2] = *(pI + poly * 3 + 2);

		D3DXVECTOR3 vPos[3];
		memcpy(&vPos[0], pV + verNo[0] * verSize, sizeof(D3DXVECTOR3));
		memcpy(&vPos[1], pV + verNo[1] * verSize, sizeof(D3DXVECTOR3));
		memcpy(&vPos[2], pV + verNo[2] * verSize, sizeof(D3DXVECTOR3));

		Vector3 vEdge[3]; //�|���S���̕ӂ�\���x�N�g��
		vEdge[0] = vPos[1] - vPos[0];
		vEdge[1] = vPos[2] - vPos[1];
		vEdge[2] = vPos[0] - vPos[2];

		Vector3 vCrsNormal;	//�Q�ӂ̊O�ς��g���Ė@�������߂�
		D3DXVec3Cross(&vCrsNormal, &vEdge[0], &vEdge[1]);
		vCrsNormal.Normalize();

		Vector3 vVerToChar[3];	//���_����L�����܂ł̃x�N�g��
		vVerToChar[0] = vpos - vPos[0];
		vVerToChar[1] = vpos - vPos[1];
		vVerToChar[2] = vpos - vPos[2];

		float dist;	//�L��������ǂ܂ł̍ŒZ����
		dist = D3DXVec3Dot(&vVerToChar[0], &vCrsNormal);

		if (dist < 0)
		{
			dist = -dist;
			vCrsNormal = -vCrsNormal;
		}

		if (dist > s_rad) { continue; }

		//--------------------------------------
		//�|���S���̐��ʂɂ��邩�ǂ����̔���
		//--------------------------------------

		Vector3 ptOnWall;	//�L�������W��Ǐ�ɓ��e�������W(A)
		ptOnWall = vpos - vCrsNormal * dist;

		Vector3 vVerToPt[3];	//�e���_����(A)�܂ł̃x�N�g��
		vVerToPt[0] = ptOnWall - vPos[0];
		vVerToPt[1] = ptOnWall - vPos[1];
		vVerToPt[2] = ptOnWall - vPos[2];

		Vector3 vCross[3]; //��L�x�N�g�����g�����O��
		D3DXVec3Cross(&vCross[0], &vEdge[0], &vVerToPt[0]);
		vCross[0].Normalize();
		D3DXVec3Cross(&vCross[1], &vEdge[1], &vVerToPt[1]);
		vCross[1].Normalize();
		D3DXVec3Cross(&vCross[2], &vEdge[2], &vVerToPt[2]);
		vCross[2].Normalize();

		//���ׂĂ̊O�ς��\�����Ȃ�(A)�͎O�p�`���ɂ���̂�
		//�O�ϓ��m�̊p�x�����X�O�x�������ǂ������`�F�b�N����
		//�|���S���͂��܂��܂ȕ����������Ă���̂ŏ�Ƃ͌���Ȃ�
		float dot1, dot2;
		dot1 = D3DXVec3Dot(&vCross[0], &vCross[1]);
		dot2 = D3DXVec3Dot(&vCross[1], &vCross[2]);

		if (dot1 > 0 && dot2 > 0)
		{
			//�@�����O�p�`�ɂ߂荞��ł���Ƃ������ƂȂ̂ō��W���C������
			//�@�@�������[���h���ɕϊ�
			vCrsNormal.TransNormal(s_mTarget);
			vCrsNormal.Normalize();

			//�@�k�������������L������@�������ɉ����o��
			s_vPos += vCrsNormal * ((s_rad - dist) / sclRate);

			//�@�L�������W���X�V���ꂽ�̂Ń��[�J�����W���Čv�Z
			vpos = s_vPos;
			vpos.TransCoord(mInv);

			continue;
		}

		//--------------------------------------
		//�|���S���̕ӂ̊O���߂��ɂ��邩�ǂ����̔���
		//--------------------------------------

		for (int i = 0; i < 3; i++)
		{
			//�ӂ̒����𑪂�
			float lengthEdge;
			lengthEdge = D3DXVec3Length(&vEdge[i]);

			//����̌v�Z�̂��߂ɕӂ̃x�N�g���𐳋K��
			D3DXVec3Normalize(&vEdge[i], &vEdge[i]);

			//�|���S���ӂƃL�������W�����Ԓ��p�O�p�`�Ɋւ��Čv�Z����

			float LenOnEdge;	//���p�O�p�`�̒�ӂ̒���
			LenOnEdge = D3DXVec3Dot(&vVerToChar[i], &vEdge[i]);

			//�ӂ̒����ȏ�ɗ���Ă�����Փ˂��Ă��Ȃ�
			if (LenOnEdge < 0 || LenOnEdge > lengthEdge) { continue; }

			float lenVerToChar;	//���p�O�p�`�̎Εӂ̒���
			lenVerToChar = D3DXVec3Length(&vVerToChar[i]);

			float lenEdgeToChar;	//���p�O�p�`�̍��������߂�
			lenEdgeToChar = sqrt(lenVerToChar * lenVerToChar - LenOnEdge * LenOnEdge);

			if (lenEdgeToChar < s_rad)	//�ӂɋ����H������ł���
			{
				//�ӂɋ߂Â����������A�����Ԃ�
				Vector3 vOut;
				//�ӂ��痣��邽�߂̃x�N�g��
				vOut = vpos - (vPos[i] + vEdge[i] * LenOnEdge);
				vOut.Normalize();

				//�k�������������L������@�������ɉ����o��
				s_vPos += vOut * ((s_rad - lenEdgeToChar) / sclRate);

				//�L�������W���X�V���ꂽ�̂Ń��[�J�����W���Čv�Z
				vpos = s_vPos;
				vpos.TransCoord(mInv);
			}
		}
	}
	s_lpMesh->UnlockVertexBuffer();
	s_lpMesh->UnlockIndexBuffer();
}
