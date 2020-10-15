#pragma once

// For use in Razor applications
#include "Razor/Application/Application.h"
#include "Razor/Application/Layer.h"
#include "Razor/Core/Log.h"

#include "Razor/Scene/Node.h"
#include "Razor/Imgui/ImGuiLayer.h"
#include "Razor/Input/Input.h"
#include "Razor/Core/TasksManager.h"
#include "Razor/Core/ThreadPool.h"
#include "Razor/Types/Variant.h"
#include "Razor/Types/Array.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Razor/Materials/Presets/PbrMaterial.h"
#include "Razor/Materials/Presets/LandscapeMaterial.h"
#include "Razor/Materials/Presets/ColorMaterial.h"
#include "Razor/Materials/Presets/SkyboxMaterial.h"
#include "Razor/Materials/Presets/AtmosphereMaterial.h"

#include "Razor/Landscape/Landscape.h"

#include "Razor/Network/Http.h"

#include "Razor/Physics/World.h"
#include "Razor/Physics/PhysicsBody.h"
#include "Razor/Physics/Bodies/CubePhysicsBody.h"
#include "Razor/Physics/Bodies/SpherePhysicsBody.h"
#include "Razor/Physics/Bodies/PlanePhysicsBody.h"

#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Lighting/Spot.h"

#include "Razor/Scripting/LuaScript.h"
#include "Razor/Scripting/PythonScript.h"

#include "Editor/Components/AssetsManager.h"

#include "Razor/Filesystem/Serializer.h"
#include "Razor/Filesystem/HuffmanEncoding.h"

#include "Razor/Input/MouseButtons.h"
#include "Razor/Input/KeyCodes.h"
#include "Razor/Core/Engine.h"
#include "Razor/Core/System.h"
#include "Razor/Network/Network.h"
#include "Razor/Network/TCPServer.h"
#include "Razor/Network/Socket.h"
#include "Razor/Network/TCPClient.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

// --- Entrypoint ---
#include "Razor/Application/Entrypoint.h"
