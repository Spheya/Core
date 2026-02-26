#pragma once

#include <span>

#include "platform.hpp"
#include "vertex.hpp"

class Mesh {
	friend class GraphicsContext;

public:
	unsigned getVertexCount() const { return m_vertexCount; }
	unsigned getIndexCount() const { return m_indexCount; }

private:
	Mesh(ID3D11Device* device, std::span<const Vertex> vertices, std::span<const unsigned> indices);

private:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	unsigned m_vertexCount;
	unsigned m_indexCount;
};
