#pragma once
#include <chrono>
#include <string>	
#include <string_view>

// [2020/10/26 김효곤]
// 게임 타이머 등에 쓰일 타이머 입니다. 
// Nano의 정밀도를 갖습니다.

class Clock
{
	std::chrono::steady_clock::time_point		StartTime;
	std::chrono::steady_clock::time_point		EndTime;
	std::chrono::steady_clock::time_point		PreTime;

public:
	Clock();
	~Clock();

public:
	// 시계의 시작지점을 설정합니다.
	void Start();
	void Stop();

	// Pause Time (멈춰있던 시간) 리턴
	double Restart();

	// 현재시간 - 처음시간 체크
	double CheckTime();

	// 현재 타임 카운트, 정밀도 별로있음.
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
