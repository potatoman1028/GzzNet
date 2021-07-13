#pragma once
#include <chrono>
#include <string>	
#include <string_view>

// [2020/10/26 ��ȿ��]
// ���� Ÿ�̸� � ���� Ÿ�̸� �Դϴ�. 
// Nano�� ���е��� �����ϴ�.

class Clock
{
	std::chrono::steady_clock::time_point		StartTime;
	std::chrono::steady_clock::time_point		EndTime;
	std::chrono::steady_clock::time_point		PreTime;

public:
	Clock();
	~Clock();

public:
	// �ð��� ���������� �����մϴ�.
	void Start();
	void Stop();

	// Pause Time (�����ִ� �ð�) ����
	double Restart();

	// ����ð� - ó���ð� üũ
	double CheckTime();

	// ���� Ÿ�� ī��Ʈ, ���е� ��������.
	double	Now();			// base time
	int		Min();			// to min
	int		Hour();			// to hour
	__int64	Nano();			// to nano
	__int64 Micro();		// to micro
	__int64	Milli();		// to milli
	__int64 Sec();			// to sec	


private:
	std::chrono::steady_clock::time_point	Tick();
};
