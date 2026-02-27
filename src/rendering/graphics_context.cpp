#include "graphics_context.hpp"

#include <chrono>
#include <cstdlib>
#include <thread>

GraphicsContext* GraphicsContext::s_instance = nullptr;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_CLOSE: DestroyWindow(hWnd); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

static void initializeWindowClasses(HINSTANCE hInstance) {
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = L"Window";
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hbrBackground = nullptr;
	RegisterClass(&windowClass);
}

void GraphicsContext::initialize(HINSTANCE hInstance, const wchar_t* applicationName) {
	assert(!s_instance);
	initializeWindowClasses(hInstance);

	HWND mainWindow = CreateWindowEx(
	    0, L"Window", applicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 960, 640, nullptr, nullptr, hInstance, nullptr
	);

	ComPtr<IDXGISwapChain> swapchain;
	s_instance = new GraphicsContext(mainWindow, swapchain);
	s_instance->m_mainSurface = std::make_unique<Surface>(mainWindow, std::move(swapchain), glm::ivec2(960, 640));
	s_instance->loadResources();

	ShowWindow(mainWindow, SW_SHOW);
}

void GraphicsContext::close() {
	assert(s_instance);
	delete s_instance;
	s_instance = nullptr;
}

GraphicsContext& GraphicsContext::getInstance() {
	assert(s_instance);
	return *s_instance;
}

GraphicsContext::GraphicsContext(HWND mainWindow, ComPtr<IDXGISwapChain>& swapchain) {
	DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
	swapchainDesc.BufferCount = 2;
	swapchainDesc.BufferDesc.Width = 960;
	swapchainDesc.BufferDesc.Height = 640;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = mainWindow;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.Windowed = TRUE;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	[[maybe_unused]] D3D_FEATURE_LEVEL featureLevel;
	handleFatalError(
	    D3D11CreateDeviceAndSwapChain(
	        nullptr,
	        D3D_DRIVER_TYPE_HARDWARE,
	        nullptr,
	        0,
	        nullptr,
	        0,
	        D3D11_SDK_VERSION,
	        &swapchainDesc,
	        &swapchain,
	        &m_device,
	        &featureLevel,
	        &m_context
	    ),
	    "Could not setup DX11 device"
	);

	ComPtr<IDXGIDevice1> dxgiDevice;
	m_device.As(&dxgiDevice);
	dxgiDevice->SetMaximumFrameLatency(1);

#ifndef SHIPPING
	ComPtr<IDXGIAdapter> adapter;
	DXGI_ADAPTER_DESC desc;
	dxgiDevice->GetAdapter(&adapter);
	adapter->GetDesc(&desc);

	const char* featureLevelString;
	switch(featureLevel) {
	case D3D_FEATURE_LEVEL_1_0_GENERIC: featureLevelString = "1.0 Generic"; break;
	case D3D_FEATURE_LEVEL_1_0_CORE: featureLevelString = "1.0 Core"; break;
	case D3D_FEATURE_LEVEL_9_1: featureLevelString = "9.1"; break;
	case D3D_FEATURE_LEVEL_9_2: featureLevelString = "9.2"; break;
	case D3D_FEATURE_LEVEL_9_3: featureLevelString = "9.3"; break;
	case D3D_FEATURE_LEVEL_10_0: featureLevelString = "10.0"; break;
	case D3D_FEATURE_LEVEL_10_1: featureLevelString = "10.1"; break;
	case D3D_FEATURE_LEVEL_11_0: featureLevelString = "11.0"; break;
	case D3D_FEATURE_LEVEL_11_1: featureLevelString = "11.1"; break;
	case D3D_FEATURE_LEVEL_12_0: featureLevelString = "12.0"; break;
	case D3D_FEATURE_LEVEL_12_1: featureLevelString = "12.1"; break;
	case D3D_FEATURE_LEVEL_12_2: featureLevelString = "12.2"; break;
	}

	logger::log("DirectX version: {}", featureLevelString);

	size_t out;
	char str[256];
	wcstombs_s(&out, str, 256, desc.Description, _TRUNCATE);

	logger::log("DirectX Device: {}", str);
	logger::log("Device Id: {:#x}", desc.DeviceId);
	logger::log("Device Vendor Id: {:#x}", desc.VendorId);
	logger::log("Dedicated Video Memory: {} MB\n", (desc.DedicatedVideoMemory / (1024ull * 1024ull)));
#endif
}

void GraphicsContext::draw(const Surface& surface) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	auto* rtv = surface.getRenderTargetView();

	D3D11_VIEWPORT viewport = {};
	viewport.Width = float(surface.getWidth());
	viewport.Height = float(surface.getHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	float clearColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

	ID3D11Buffer* buffer = m_quadMesh->m_vertexBuffer.Get();

	m_context->OMSetRenderTargets(1, &rtv, nullptr);
	m_context->RSSetViewports(1, &viewport);
	m_context->ClearRenderTargetView(rtv, clearColor);

	m_context->VSSetShader(m_defaultVertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(m_defaultPixelShader.Get(), nullptr, 0);
	m_context->IASetInputLayout(m_defaultInputLayout.Get());
	m_context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	m_context->IASetIndexBuffer(m_quadMesh->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->DrawIndexed(m_quadMesh->getIndexCount(), 0, 0);
}

void GraphicsContext::loadResources() {
	constexpr char defaultVertexSource[] = {
#embed "embed/default_vs.cso"
	};
	constexpr char defaultPixelSource[] = {
#embed "embed/default_ps.cso"
	};

	constexpr D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_device->CreateInputLayout(layout, 1, defaultVertexSource, sizeof(defaultVertexSource), &m_defaultInputLayout);
	m_device->CreateVertexShader(defaultVertexSource, sizeof(defaultVertexSource), nullptr, &m_defaultVertexShader);
	m_device->CreatePixelShader(defaultPixelSource, sizeof(defaultPixelSource), nullptr, &m_defaultPixelShader);

	constexpr Vertex quadVertices[] = {
		{ glm::vec3(-0.5f, +0.5f, 0.0f) },
		{ glm::vec3(+0.5f, +0.5f, 0.0f) },
		{ glm::vec3(+0.5f, -0.5f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, 0.0f) },
	};

	constexpr unsigned quadIndices[] = { 0, 1, 2, 0, 2, 3 };

	m_quadMesh = std::unique_ptr<Mesh>(new Mesh(m_device.Get(), quadVertices, quadIndices));
}
