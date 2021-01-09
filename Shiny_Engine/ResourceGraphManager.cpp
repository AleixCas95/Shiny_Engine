#include "Application.h"
#include "ResourceGraphManager.h"
#include "NodeGraph_Manager.h"
#include "ModuleResources.h"




ResourceGraphManager::ResourceGraphManager(const char* path) : Resource(ResourceType::Mesh, path)
{


}


ResourceGraphManager::~ResourceGraphManager()
{

}

bool ResourceGraphManager::LoadInMemory()
{
	node_graph = new NodeGraph_Manager();

	node_graph->LoadFile(uuid, App->resources->names[uuid]);

	return true;

}

bool ResourceGraphManager::UnloadInMemory()
{
	RELEASE(node_graph);

	return true;
}


