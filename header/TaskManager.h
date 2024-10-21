#pragma once

class TaskManager
{
public:
	inline TaskManager();
	inline ~TaskManager();

	inline const int* CursorPitchGetter() { return &op_X; };
	inline const int* CursorYawGetter() { return &op_Y; };
	inline void CursorPitchSetter(const int s_Flg) { op_X = s_Flg; };
	inline void CursorYawSetter(const int s_Flg) { op_Y = s_Flg; };

	inline const int* StageNunberGetter() { return &stgNo; };
	inline void StageNunberSetter(const int* s_No) { stgNo = *s_No; };

	inline const int* BGMGetter() { return &BGM; };
	inline const int* SEGetter() { return &SE; };
	inline void BGMSetter(const int s_BGM) { BGM = s_BGM; };
	inline void SESetter(const int s_SE) { SE = s_SE; };

private:
	int op_X, op_Y;
	int stgNo;
	int BGM, SE;
};


TaskManager::TaskManager()
{
	FILE* p_Data;
	fopen_s(&p_Data, "data/txt/config.dat", "r");

	if (p_Data)
	{
		fscanf_s(p_Data, "MouseX,Y=%d,%d\n", &op_X, &op_Y);
		switch (op_X)
		{
		case -1:
		case 1:
			break;

		default:
			op_X = 1;
			break;
		}
		switch (op_Y)
		{
		case -1:
		case 1:
			break;

		default:
			op_X = 1;
			break;
		}

		fscanf_s(p_Data, "BGM_Volume=%d\n", &BGM);
		if (BGM > 100) BGM = 100;
		if (BGM < 0) BGM = 0;

		fscanf_s(p_Data, "SE_Volume=%d\n", &SE);
		if (SE > 100) SE = 100;
		if (SE < 0) SE = 0;

		fclose(p_Data);
	}
	else
	{
		op_X = 1, op_Y = 1;
		BGM = 100;
		SE = 100;
	}
}
TaskManager::~TaskManager()
{
	FILE* p_Data;

	fopen_s(&p_Data, "data/txt/config.dat", "w");

	fprintf_s(p_Data, "MouseX,Y=%d,%d\n", op_X, op_Y);
	fprintf_s(p_Data, "BGM_Volume=%d\n", BGM);
	fprintf_s(p_Data, "SE_Volume=%d\n", SE);

	fclose(p_Data);
}