#include "window_physics.hpp"

#include <algorithm>

#include "platform.hpp"
#include "rendering/graphics_context.hpp"

void WindowPhysics::update() {
	// todo: dont do this every frame
	m_hitboxes.clear();

	for(HWND hwnd = GetTopWindow(nullptr); hwnd; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
		if(!IsWindowVisible(hwnd)) continue;
		if(IsIconic(hwnd)) continue;
		if(GetWindow(hwnd, GW_OWNER) != nullptr) continue;
		if(GetWindowLongPtr(hwnd, GWL_EXSTYLE) & (WS_EX_TOOLWINDOW | WS_EX_TOPMOST)) continue;
		if(GetWindowLongPtr(hwnd, GWL_STYLE) & (WS_CHILD | WS_POPUP)) continue;

		RECT rect;
		if(GetWindowRect(hwnd, &rect)) m_hitboxes.emplace_back(glm::vec2(rect.left, rect.top), glm::vec2(rect.right, rect.bottom));
	}

#ifdef _DEBUG
	for(const auto& box : m_hitboxes) GraphicsContext::getInstance().getDebugRenderer().box(box, glm::vec4(0.0f, 1.0f, 0.0f, 0.3f));
#endif
}

bool WindowPhysics::overlaps(const BoundingBox& box) const {
	return std::ranges::any_of(m_hitboxes, [&box](const BoundingBox& hitbox) { return ::overlaps(hitbox, box); });
}

Intersection WindowPhysics::rayCast(glm::vec2 origin, glm::vec2 direction, float maxDistance) const {
	Intersection miss{ .distance = maxDistance, .normal = glm::vec2(0.0f) };
	Intersection hit{ .distance = maxDistance, .normal = glm::vec2(0.0f) };
	float d = 0.0f;

	for(const auto& hitBox : m_hitboxes) {
		if(::overlaps(hitBox, origin)) {
			Intersection exit = ::rayCast(origin, direction, hitBox, hit.distance, RayCastExclude::Entrance);
			if(exit.distance == hit.distance) break;

			origin += direction * exit.distance;
			d += exit.distance;
			hit.distance -= exit.distance;
		} else {
			auto newHit = ::rayCast(origin, direction, hitBox, hit.distance, RayCastExclude::Exit);
			if(newHit.distance != hit.distance) hit = newHit;
		}
	}

	if(hit.normal != glm::vec2(0.0f)) {
		hit.distance += d;
		return hit;
	}
	return miss;
}

Intersection WindowPhysics::boxCast(BoundingBox origin, glm::vec2 direction, float maxDistance) const {
	Intersection miss{ .distance = maxDistance, .normal = glm::vec2(0.0f) };
	Intersection hit{ .distance = maxDistance, .normal = glm::vec2(0.0f) };
	float d = 0.0f;

	for(const auto& hitBox : m_hitboxes) {
		if(::overlaps(hitBox, origin)) {
			Intersection exit = ::boxCast(origin, direction, hitBox, hit.distance, RayCastExclude::Entrance);
			if(exit.distance == hit.distance) break;

			origin.min += direction * exit.distance;
			origin.max += direction * exit.distance;
			d += exit.distance;
			hit.distance -= exit.distance;
		} else {
			auto newHit = ::boxCast(origin, direction, hitBox, hit.distance, RayCastExclude::Exit);
			if(newHit.distance != hit.distance) hit = newHit;
		}
	}

	if(hit.normal != glm::vec2(0.0f)) {
		hit.distance += d;
		return hit;
	}
	return miss;
}
