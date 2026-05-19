#pragma once
#include "MeshData.hpp"

namespace Assets::MeshLoader
{
	MeshData LoadMesh(std::string_view filePath);
}
