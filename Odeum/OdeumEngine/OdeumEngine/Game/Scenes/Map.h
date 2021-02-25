#ifndef MAP_H
#define MAP_H

#include "../../pch.h"
#include "../../Engine/Core/OdeumEngine.h"
#include "../../Engine/Core/CameraController.h"
#include <vector>
#include <list>

class MAPScene : public Scene
{
public:
	MAPScene();
	virtual ~MAPScene();

	virtual bool Initialize();
	virtual void Update(const float deltaTime_);
	virtual void UIRender();

	template <class T>
	class UndirectedGraph
	{
	public:
		class Node
		{
			friend UndirectedGraph;

		public:
			Node(const T& data) : data(data)
			{ }

			T& getData()
			{
				return data;
			}

			std::vector<Node*>& getNeighbours()
			{
				return neighbours;
			}

		private:
			T data;
			std::vector<Node*> neighbours;
		};

		Node *addNode(const T& data)
		{
			Node *node = new Node(data);
			nodes.push_back(node);
			return node;
		}

		Node *findNode(const T& data)
		{
			for (unsigned i = 0; i < nodes.size(); i++)
				if (nodes[i]->getData() == data)
					return nodes[i];

			return NULL;
		}
		//to get neighbour
		void addEdge(Node *node1, Node *node2)
		{
			if (!node1 || !node2)
				return;
			node1->getNeighbours().push_back(node2);
			node2->getNeighbours().push_back(node1);
		}

		void addEdge(const T& node1, const T& node2)
		{
			addEdge(findNode(node1), findNode(node2));
		}


		/////////////////////////////////////////////////////////

		// incase we need a way to trigger to the next the iterator can be connected via trigger
		void enter()
		{
			list<Trigger*>::iterator t = triggers.begin();

			while (t != triggers.end())
			{
				(*(*t))();
				t = triggers.erase(t);
			}

			void addTrigger(Trigger *t)
			{
				triggers.push_back(t);
			}



	private:
		std::vector<Node*> nodes;
		list<Trigger*> triggers;


				


	};







private:
	
	CameraController cameraController;

	std::vector<GameObject*> gameObjects;
	
	
	

	float angle;
	float direction;
   
	
};



//////////////////////////////////////////////////////////////////////////////////////////////
//just a struct with some function to trigger logic for future use.

 struct Trigger
{
public:
	Trigger(const std::string& msg) : msg(msg)
	{ }

	void operator() ()
	{
		std::cout << msg;
	}
	
private:

	std::string msg;
};

#endif
