#include "surface_manager.hpp"
#include "graphics_context.hpp"

SurfaceManager* SurfaceManager::s_instance;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_CLOSE: DestroyWindow(hWnd); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

static BOOL CALLBACK createScreenSurface(HMONITOR hMonitor, HDC /* hdcMonitor */, LPRECT /* lprcMonitor */, LPARAM lParam) {
	HINSTANCE hInstance = (HINSTANCE)lParam; // NOLINT

	MONITORINFOEX mi = {};
	mi.cbSize = sizeof(MONITORINFOEX);

	if(!GetMonitorInfo(hMonitor, &mi)) fatalError("Could not obtain monitor info");

	unsigned x = mi.rcMonitor.left;
	unsigned y = mi.rcMonitor.top;
	unsigned w = mi.rcMonitor.right - mi.rcMonitor.left;
	unsigned h = mi.rcMonitor.bottom - mi.rcMonitor.top;

	HWND hWnd = CreateWindowEx(
	    WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_NOREDIRECTIONBITMAP | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
	    L"Window",
	    L"",
	    WS_POPUP,
	    int(x),
	    int(y),
	    int(w),
	    int(h),
	    nullptr,
	    nullptr,
	    hInstance,
	    nullptr
	);

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = w;
	swapchainDesc.Height = h;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = FALSE;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

	ComPtr<IDXGISwapChain1> swapchain;
	handleFatalError(
	    GraphicsContext::getInstance().getFactory()->CreateSwapChainForComposition(
	        GraphicsContext::getInstance().getDevice(), &swapchainDesc, nullptr, swapchain.GetAddressOf()
	    ),
	    "Could not create swapchain"
	);

	SurfaceManager::getInstance().m_screenSurfaces.emplace_back(
	    std::make_unique<ScreenSurface>(hWnd, std::move(swapchain), glm::uvec2(w, h), glm::ivec2(x, y))
	);
	ShowWindow(hWnd, SW_SHOW);

	return TRUE;
}

static void initializeWindowClasses(HINSTANCE hInstance) {
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = L"Window";
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&windowClass);
}

void SurfaceManager::initialize(HINSTANCE hInstance) {
	initializeWindowClasses(hInstance);
	s_instance = new SurfaceManager();
	EnumDisplayMonitors(nullptr, nullptr, createScreenSurface, (LPARAM)hInstance); // NOLINT
	GraphicsContext::getInstance().getCompositionDevice()->Commit();
}

void SurfaceManager::close() {
	assert(s_instance);
	delete s_instance;
	s_instance = nullptr;
}

SurfaceManager& SurfaceManager::getInstance() {
	assert(s_instance);
	return *s_instance;
}

Surface* SurfaceManager::getSurface(HWND window) {
	auto it = m_surfaces.find(window);
	if(it == m_surfaces.end()) return nullptr;
	return it->second;
}
