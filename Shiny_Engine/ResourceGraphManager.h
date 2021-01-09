#ifndef __ResourceGraphManager_H__
#define __ResourceGraphManager_H__

#include "Resources.h"

class NodeGraph_Manager;

class ResourceGraphManager : public Resource
{
public:
	ResourceGraphManager(const char* path);
	~ResourceGraphManager();

	bool LoadInMemory();
	bool UnloadInMemory();

public:
	NodeGraph_Manager* node_graph;
	scriptType uuid = 0;

};

#endif