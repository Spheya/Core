#pragma once

#include <chrono>

class Time {
public:
	Time() : m_start(std::chrono::steady_clock::now()), m_prevFrame(m_start), m_time(0.0), m_deltaTime(0.0f) {}

	void update() {
		auto currFrame = std::chrono::steady_clock::now();
		m_deltaTime = std::chrono::duration<float>(currFrame - m_prevFrame).count();
		m_time = std::chrono::duration<double>(currFrame - m_start).count();
		m_prevFrame = currFrame;
	}

	[[nodiscard]] double time() const { return m_time; }
	[[nodiscard]] float deltaTime() const { return m_deltaTime; }

private:
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_prevFrame;
	double m_time;
	float m_deltaTime;
};
