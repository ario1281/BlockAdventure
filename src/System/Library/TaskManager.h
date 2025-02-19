#pragma once
#include"define.h"

class TaskManager
{
public:
	bool GetGoalFlag() { return goalFlg; }
	void SetGoalFlag(bool s_flg = false) { goalFlg = s_flg; }

	~TaskManager() {}
private:
	bool goalFlg = false;

public:
	static TaskManager& GetInstance() {
		static TaskManager Instance;
		return Instance;
	}
private:
	TaskManager() {}
};

#define TASK TaskManager::GetInstance()