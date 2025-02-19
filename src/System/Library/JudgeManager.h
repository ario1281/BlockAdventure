#pragma once

class JudgeManager
{
public:
	void TransformInverse(
		const Matrix& s_mInv,
		Vector3 &s_vPos,
		Vector3 &s_vDir);

	//�@���C����
	void JudgingLayToMesh(
		const LPD3DXMESH	&lpMesh,	//�@�E[in]	��������郁�b�V��
		const Matrix		&mTarget,	//�@�E[in]	���b�V���̃}�g���b�N�X
		const Vector3		&vPos,		//�@�E[in]	���C�̔��ˈʒu
		const Vector3		&vDir,		//�@�E[in]	���C�̔��˕���
		bool				*pHit,		//	�E[out] ���b�V���ɓ�������������
		DWORD				*pPolyNo,	//�@�E[out] ���������|���S���̃i���o�[
		float				*pDist,		//�@�E[out] ���C�̈ʒu����M���b�V���܂ł̒���
		DWORD				*pHitCnt);	//�@�E[out] �ꔭ�̃��C�Ƀq�b�g������

	//�@������
	bool JudgeSphere(
		const Vector3		&vTarget,	//�@�E[in]	�^�[�Q�b�g�̈ʒu
		const Vector3		&vBase,		//�@�E[in]	��^�[�Q�b�g�̈ʒu
		float				Dist);		//�@�E[in]	�o�ꂭ�炢�̒����Ŕ��肷�邩


	~JudgeManager() {}
private:

#pragma region �V���O���g��
public:
	static JudgeManager& GetInstance() {
		static JudgeManager Instance;
		return Instance;
	}
private:
	JudgeManager() {}
#pragma endregion

};

#define JUDGE JudgeManager::GetInstance()