#pragma once
class Time
{
private:
	static float lastTime;
public:
	static float deltaTime;

	static void UpdateTime();
};

