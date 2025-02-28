#include "DebugPoints.h"

std::vector<Vertex> DebugPoints::verticies;
std::vector<uint32_t> DebugPoints::indicies;

Buffer<Vertex>* DebugPoints::vertexBuffer = new Buffer<Vertex>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
Buffer<uint32_t>* DebugPoints::indexBuffer = new Buffer<uint32_t>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

void DebugPoints::AddLines(std::vector<Vertex> verticies, std::vector<uint32_t> indices)
{
	for (int i = 0; i < indices.size(); i++)
	{
		indices[i] += DebugPoints::verticies.size();
		DebugPoints::indicies.push_back(indices[i]);
	}

	DebugPoints::verticies.insert(DebugPoints::verticies.begin(), verticies.begin(), verticies.end());

	vertexBuffer->AddToBuffer(verticies);
	indexBuffer->AddToBuffer(indices);

	vertexBuffer->SendBufferToMemory();
	indexBuffer->SendBufferToMemory();
}

void DebugPoints::ClearLines()
{
	vertexBuffer->ClearBuffer();
	indexBuffer->ClearBuffer();
}

void DebugPoints::DeleteBuffers()
{
	delete vertexBuffer;
	delete indexBuffer;
}