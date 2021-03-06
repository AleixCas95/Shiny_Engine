#ifndef __NodeGraph_Manager_H__
#define __NodeGraph_Manager_H__

#include "Globals.h"
#include <string>
#include "imgui/imgui.h"
#include "Parson/parson.h"
#include <vector>

#define NODE_WINDOW_PADDING ImVec2(8.0f, 8.0f)
#define NODE_SLOT_RADIUS 4.0f


static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

class GameObject;
class Node;
enum NodeFunction;
enum NodeState;

class NodeLink
{
public:
	int     InputIdx, InputSlot, OutputIdx, OutputSlot;

	NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) {  //id of node input, slot number, id of node output, slot number 
		InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot;
	}
};


class NodeGraph_Manager {

public:

	NodeGraph_Manager();
	~NodeGraph_Manager();

	void Draw(std::vector<GameObject*> BB_objects, bool go_active, bool& show_graph);
	void Update(float dt, std::vector<GameObject*> BB_objects, uint num_comp_graph);
	void UpdateOutputNodes(float dt, std::vector<GameObject*> BB_objects, Node* output, NodeState input_updating, uint num_comp_graph);
	void SaveFile(JSON_Array* arr_nodes, JSON_Array* arr_links) const;
	void LoadFile(scriptType uuid, std::string name);

	Node* AddNode(NodeFunction node_function, const ImVec2& pos);
	void AddLink(int input_idx, int input_slot, int output_idx, int output_slot);
	void DeleteLink(int node_id, int slot_num, bool input); //Send true if slot is input (left of the node) or false if it's output (right of the node)
	void DeleteNode(Node* node);
	Node* GetNodeByID(int ID);

	uint GetTimerNum() const;

private:

	bool show_grid = true;
	std::vector<Node*> nodes;
	std::vector<Node*> fst_ev_nodes;
	std::vector<NodeLink> links;

	ImVec2 scrolling = ImVec2(0.0f, 0.0f);

	int last_node_id = -1;

	int input_id_clicked = -1;
	int input_slot_clicked = -1;
	int output_id_clicked = -1;
	int output_slot_clicked = -1;


	bool inited = false;

};

#endif