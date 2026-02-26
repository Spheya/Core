#include <thread>

#include "platform.hpp"
#include "rendering/graphics_context.hpp"
#include "rendering/sprite_atlas.hpp"

// Todo: move to graphics context
static bool pollEvents() {
	MSG msg = {};
	while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT) return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

static int runApp(HINSTANCE hInstance) {
	GraphicsContext::initialize(hInstance, L"virus");
	SpriteAtlas::load();

	[[maybe_unused]] Sprite jacco = SpriteAtlas::getInstance().get("jaccocube.png");

	size_t i = 0;
	while(pollEvents()) {
		logger::log("frame {}", i);
		++i;
		// Frame
		GraphicsContext::getInstance().draw(GraphicsContext::getInstance().getMainSurface());
		GraphicsContext::getInstance().getMainSurface().getSwapchain()->Present(0, 0);
	}

	SpriteAtlas::destroy();
	GraphicsContext::close();
	return 0;
}

#ifdef _CONSOLE
int main() {
	return runApp(GetModuleHandle(nullptr));
}
#endif

#ifdef _WINDOWS
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ PSTR /* lpCmdLine */, _In_ int /* nShowCmd */) {
	return runApp(hInstance);
}
#endif
