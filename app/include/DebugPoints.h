#pragma once
#include <vector>
#include "Vertex.h"
#include "Buffer.h"
class DebugPoints
{
private:
	static std::vector<Vertex> verticies;
	static std::vector<uint32_t> indicies;

	static Buffer<Vertex>* vertexBuffer;
	static Buffer<uint32_t>* indexBuffer;
	static void DeleteBuffers();
public:
	static void AddLines(std::vector<Vertex> verticies, std::vector<uint32_t> indices);
	static void ClearLines();
	friend class Commands;
	friend class Application;
};