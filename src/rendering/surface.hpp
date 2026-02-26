#pragma once

#include "math.hpp"
#include "platform.hpp"

class Surface {
public:
	Surface(HWND window, ComPtr<IDXGISwapChain> swapchain, glm::ivec2 initialiDimensions);
	Surface(const Surface&) = delete;
	Surface& operator=(const Surface&) = delete;
	Surface(Surface&& other) noexcept;
	Surface& operator=(Surface&& other) noexcept;
	virtual ~Surface();

	[[nodiscard]] HWND getWindow() const { return m_window; }
	[[nodiscard]] IDXGISwapChain* getSwapchain() const { return m_swapchain.Get(); }
	[[nodiscard]] ID3D11RenderTargetView* getRenderTargetView() const { return m_rtv.Get(); }

	[[nodiscard]] glm::uvec2 getDimensions() const { return m_dimensions; }
	[[nodiscard]] unsigned getWidth() const { return m_dimensions.x; }
	[[nodiscard]] unsigned getHeight() const { return m_dimensions.y; }

	[[nodiscard]] static Surface* get(HWND window);
	[[nodiscard]] static size_t count();

public:
	// Todo: Private functions and voeg WndProc toe aan vriendenboekje
	void updateDimensions(glm::uvec2 dimensions);

private:
	void destroy();
	void loadRtv();

private:
	HWND m_window;
	ComPtr<IDXGISwapChain> m_swapchain;
	ComPtr<ID3D11RenderTargetView> m_rtv;
	glm::uvec2 m_dimensions;
};
