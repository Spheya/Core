#pragma once

#include <memory>

#include "mesh.hpp"
#include "surface.hpp"

class GraphicsContext {
private:
	GraphicsContext();

public:
	static void initialize(HINSTANCE hInstance, const wchar_t* applicationName);
	static void close();
	static GraphicsContext& getInstance();

public:
	[[nodiscard]] ID3D11Device* getDevice() const { return m_device.Get(); }
	[[nodiscard]] ID3D11DeviceContext* getDeviceContext() const { return m_context.Get(); }
	[[nodiscard]] IDCompositionDevice* getCompositionDevice() const { return m_compDevice.Get(); }

	[[nodiscard]] const Surface& getMainSurface() const { return *m_mainSurface; }
	[[nodiscard]] Surface& getMainSurface() { return *m_mainSurface; }

	void draw(const Surface& surface);

private:
	void loadResources();

private:
	static GraphicsContext* s_instance;

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<IDCompositionDevice> m_compDevice;

	ComPtr<ID3D11InputLayout> m_defaultInputLayout;
	ComPtr<ID3D11VertexShader> m_defaultVertexShader;
	ComPtr<ID3D11PixelShader> m_defaultPixelShader;

	std::unique_ptr<Mesh> m_quadMesh;
	std::unique_ptr<Surface> m_mainSurface;
};
