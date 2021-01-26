#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iterator>
#include <list>
#include <stack>
#include <queue>
#include <string>
#include <cassert>

using namespace std;

template<typename T>
class Graph
{
	template<typename T>
	class Vertex
	{
		friend Graph;

	public:
		Vertex(T n) : node(n)
		{ }

		T getNode()
		{
			return node;
		}

	private:
		T node;
	};

	class Edge
	{
		friend Graph;

	public:
		Edge() : startVertex(0), endVertex(0), weight(0)
		{ }

		bool operator<(const Edge& e2) const
		{
			return (weight < e2.weight);
		}

		bool operator>(const Edge& e2) const
		{
			return (weight > e2.weight);
		}

		bool operator==(const Edge& e2) const
		{
			return (endVertex == e2.endVertex);
		}

		int startVertex, endVertex;
		int weight;
	};

public:
	Graph(int numVerts, bool weighted = false) : maxVertices(numVerts), adjacencyMatrix(NULL), isWeighted(weighted)
	{
		// Quit if number of vertices is 0 or less
		assert(numVerts > 0);

		// Reserve vector size to store vertex labels
		vertices.reserve(maxVertices);

		// Instantiate adjacency matrix 
		adjacencyMatrix = new char* [maxVertices];
		assert(adjacencyMatrix != NULL);

		// Instantiate array to store visited vertices
		vertexVisits = new char[maxVertices];
		assert(vertexVisits != NULL);

		// Initialize visited vertices array
		for (int i = 0; i < maxVertices; i++)
		{
			vertexVisits[i] = NULL;
		}

		// Initialize adjacency matrix to null
		for (int i = 0; i < maxVertices; i++)
		{
			adjacencyMatrix[i] = new char[maxVertices];

			assert(adjacencyMatrix[i] != NULL);

			for (int j = 0; j < maxVertices; j++)
			{
				adjacencyMatrix[i][j] = NULL;
			}
		}
	}

	~Graph()
	{
		if (adjacencyMatrix != NULL)
		{
			for (int i = 0; i < maxVertices; i++)
			{
				if (adjacencyMatrix[i] != NULL)
				{
					delete[] adjacencyMatrix[i];

					adjacencyMatrix[i] = NULL;
				}
			}

			delete[] adjacencyMatrix;

			adjacencyMatrix = NULL;
		}

		if (vertexVisits != NULL)
		{
			delete[] vertexVisits;

			vertexVisits = NULL;
		}
	}

	bool push(T n)
	{
		// Check if graph is full
		if ((int)vertices.size() >= maxVertices)
			return false;

		// Insert vertex if graph is not full
		vertices.push_back(Vertex<T>(n));

		return true;
	}

	int findIndex(T value)
	{
		// Quit if adjacency matrix is not initialized
		assert(adjacencyMatrix != NULL);

		// Search through list of vertices and return the index where the vertex label was found
		for (int i = 0; i < (int)vertices.size(); i++)
		{
			if (vertices[i].getNode() == value)
				return i;
		}

		// Return -1 if label not found in adjacency matrix
		return -1;
	}

	bool contains(T value)
	{
		auto it = find_if(vertices.begin(), vertices.end(), [value](Vertex<T> Vertice) { return Vertice.getNode() == value; });

		return it != vertices.end();
	}

	void attachEdge(int startIndex, int endIndex, int weight = 0)
	{
		// Quit if adjacency matrix does not exist
		assert(adjacencyMatrix != NULL);

		// If graph is weighted insert weights into adjacency matrix
		if (isWeighted)
		{
			adjacencyMatrix[startIndex][endIndex] = weight;
			adjacencyMatrix[endIndex][startIndex] = weight;
		}
		else
		{
			adjacencyMatrix[startIndex][endIndex] = 1;
			adjacencyMatrix[endIndex][startIndex] = 1;
		}
	}

	void attachEdge(T startVertex, T endVertex, int weight = 0)
	{
		// Quit if adjacency matrix does not exist
		assert(adjacencyMatrix != NULL);

		int startIndex, endIndex;

		// Find indicies for labels
		startIndex = findIndex(startVertex);
		endIndex = findIndex(endVertex);

		// Quit if a label is not a vertex
		assert(startIndex != -1);
		assert(endIndex != -1);

		// If graph is weighted insert weights into adjacency matrix
		if (isWeighted)
		{
			adjacencyMatrix[startIndex][endIndex] = weight;
			adjacencyMatrix[endIndex][startIndex] = weight;
		}
		else
		{
			adjacencyMatrix[startIndex][endIndex] = 1;
			adjacencyMatrix[endIndex][startIndex] = 1;
		}
	}

	void attachDirectedEdge(int startIndex, int endIndex, int weight = 0)
	{
		// Quit if adjacency matrix does not exist
		assert(adjacencyMatrix != NULL);

		// If graph is weighted insert weights into adjacency matrix
		if (isWeighted)
			adjacencyMatrix[startIndex][endIndex] = weight;
		else
			adjacencyMatrix[startIndex][endIndex] = 1;
	}

	void attachDirectedEdge(char startVertex, char endVertex, int weight = 0)
	{
		// Quit if adjacency matrix does not exist
		assert(adjacencyMatrix != NULL);

		// Find indicies for labels
		int startIndex, endIndex;

		startIndex = findIndex(startVertex);
		endIndex = findIndex(endVertex);

		// Quit if a label is not a vertex
		assert(startIndex != -1);
		assert(endIndex != -1);

		// If graph is weighted insert weights into adjacency matrix
		if (isWeighted)
			adjacencyMatrix[startIndex][endIndex] = weight;
		else
			adjacencyMatrix[startIndex][endIndex] = 1;
	}

	int getNextUnvisitedVertex(int index)
	{
		assert(adjacencyMatrix != NULL);
		assert(vertexVisits != NULL);

		for (int i = 0; i < (int)vertices.size(); i++)
			if (adjacencyMatrix[index][i] == 1 && vertexVisits[i] == 0)
				return i;

		return -1;
	}

	bool breadthFirstSearch(char startVertex, char endVertex)
	{
		// Quit if the adjacency matrix and vertices is empty
		assert(adjacencyMatrix != NULL);
		assert(vertexVisits != NULL);

		// Find the index of the start and end vertices
		int startIndex = findIndex(startVertex);
		int endIndex = findIndex(endVertex);

		// Quit if the start and end vertices do not exist in the graph
		assert(startIndex != -1);
		assert(endIndex != -1);

		// Flag the start vertex as visited
		vertexVisits[startIndex] = 1;

		cout << vertices[startIndex].getNode() << " ";

		queue<int> searchQueue;
		int vert1 = 0, vert2 = 0;

		// Push the first vertex into the queue
		searchQueue.push(startIndex);

		// Loop until the queue is empty
		while (!searchQueue.empty())
		{
			// Remove the first node in the queue
			vert1 = searchQueue.front();
			searchQueue.pop();

			// If the last vertex is reached, reset vertexVisits and return true
			if (vert1 == endIndex)
			{
				for (int i = 0; i < maxVertices; i++)
				{
					vertexVisits[i] = NULL;
				}

				return true;
			}

			// Find the next vertex that was not visited and push it into the queue
			while ((vert2 = getNextUnvisitedVertex(vert1)) != -1)
			{
				vertexVisits[vert2] = 1;

				cout << vertices[vert2].getNode() << " ";

				searchQueue.push(vert2);
			}
		}

		// Reset vertexVisits and return false
		for (int i = 0; i < maxVertices; i++)
		{
			vertexVisits[i] = NULL;
		}

		return false;
	}

	// Depth First Traversal
	// Variables to be used:
	// adjacencyMatrix - is a pointer to the adjacency matrix
	// vertexVisits - is a character array of vertices - use 0 or 1 to indicate whether they are visited
	// vertices - is a vector of graph vertices
	// maxVertices - the maximum number of vertices
	bool depthFirstSearch(T startVertex, T endVertex)
	{
		assert(adjacencyMatrix != NULL);
		assert(vertexVisits != NULL);

		// Use findIndex() to search for the index of startVertex
		int startIndex = findIndex(startVertex);
		// Use findIndex() to search for the index of endVertex
		int endIndex = findIndex(endVertex);

		// Check is adjacency matrix or vertexVisits are equal to NULL or startIndex or endIndex equal to -1 and return false
		assert(startIndex != -1);
		assert(endIndex != -1);

		// Set the startIndex to visited
		vertexVisits[startIndex] = 1;

		// Print the value at the start index
		cout << vertices[startIndex].getNode() << " ";

		// Declare a stack for searching
		stack<int> searchStack;
		// Declare and initialize to 0 an int that tracks the successor vertex indicies
		int successorIndicies = 0;

		// Push start index into stack
		searchStack.push(startIndex);

		// Loop while stack is not empty
		while (!searchStack.empty())
		{
			int unvisited = getNextUnvisitedVertex(searchStack.top());
			if (unvisited == -1)
				searchStack.pop();
			else
			{
				vertexVisits[unvisited] = 1;
				cout << vertices[unvisited].getNode() << " ";
				searchStack.push(unvisited);
				if (unvisited == endIndex)
				{
					for (int i = 0; i < maxVertices; i++)
						vertexVisits[i] = 0;
					return true;
				}
			}
		}

		// Use getNextUnvisitedVertex() to check if the top of the stack is an unvisited vertex


		// If vertex is -1 pop from stack

		// If vertex is not -1

		// Set visited vertex at the found index to visited


		// Print the value of the current node visited


		// Push vertex index onto stack


		// If the vertex is the end index

		// Loop through vertexVisits reset their values to 0

		// Return true


		// Loop through vertexVisits reset their values to 0
		for (int i = 0; i < maxVertices; i++)
			vertexVisits[i] = 0;

		// Return false
		return false;
	}

	// Topological Sort for Directed Acyclic Graph
	bool topologicalSort(stack<T>& output)
	{
		// Flag to track cycles
		bool hasCycles = false;

		// Copy vertices into temp vector of vertices
		vector<Vertex<T>> tempVerts(vertices);

		// Store total number of vertices
		int tempSize = (int)tempVerts.size();

		// Instantiate temporary adjacency matrix
		char** tempAdjMat = new char* [maxVertices];

		// Quit if temporary adjacency matrix did not instantiate
		assert(tempAdjMat != NULL);

		// Iterate and copy all vertices from original adjancency matrix
		for (int i = 0; i < maxVertices; i++)
		{
			tempAdjMat[i] = new char[maxVertices];

			assert(tempAdjMat[i] != NULL);

			for (int j = 0; j < maxVertices; j++)
			{
				tempAdjMat[i][j] = adjacencyMatrix[i][j];
			}
		}

		// If adjacency matrix size greater than 0
		while (tempSize > 0)
		{
			// Find vertices with no successor
			int v = getVertNoSuccessor(tempAdjMat, tempSize);

			// Exit if vertex cycles
			if (v == -1)
			{
				hasCycles = true;
				break;
			}

			// Store vertex in list if no cycles found
			output.push(tempVerts[v].getNode());

			// Since vertex was visited, remove from temp list and adjacency matrix
			if (v != (tempSize - 1))
			{
				tempVerts.erase(tempVerts.begin() + v);

				for (int row = v; row < tempSize - 1; row++)
				{
					for (int col = 0; col < tempSize; col++)
					{
						tempAdjMat[row][col] = tempAdjMat[row + 1][col];
					}
				}

				for (int col = v; col < tempSize - 1; col++)
				{
					for (int row = 0; row < tempSize; row++)
					{
						tempAdjMat[row][col] = tempAdjMat[row][col + 1];
					}
				}
			}

			tempSize--;
		}

		// Clean-up temporary variables
		if (tempAdjMat != NULL)
		{
			for (int i = 0; i < maxVertices; i++)
			{
				if (tempAdjMat[i] != NULL)
				{
					delete[] tempAdjMat[i];
					tempAdjMat[i] = NULL;
				}
			}

			delete[] tempAdjMat;
			tempAdjMat = NULL;
		}

		// Return whether graph has cycles
		return !hasCycles;
	}

	void MST(string& output)
	{
		if (isWeighted)
		{
			// Quit if adjacency matrix is null
			assert(adjacencyMatrix != NULL);

			// Quit if vertices visited is null
			assert(vertexVisits != NULL);

			// Start at first vertex
			int currentVertex = 0;
			// Set total visited nodes to 0
			int totalChecked = 0;
			// Set size to size of the vertex vector
			int size = (int)vertices.size();

			// Create a vector of edges to track those visited
			vector<Edge> edgeInfo;

			// Loop while the number of checked vertices is less than the total number of vertices
			while (totalChecked < size - 1)
			{
				// Set current vertex to visited
				vertexVisits[currentVertex] = 1;
				// Add a visited vertices
				totalChecked++;

				// Loop through and add all unchecked adjacent vertices and their weights into a priority queue
				for (int i = 0; i < size; i++)
				{
					// Go to next iteration If counter is equal to current vertex or vertex visited is true or if adjacency matrix does not have an edge at current vertex
					if (i == currentVertex || vertexVisits[i] == 1 || adjacencyMatrix[currentVertex][i] == 0)
						continue;


					Edge edge;

					edge.startVertex = currentVertex;
					edge.endVertex = i;
					edge.weight = adjacencyMatrix[currentVertex][i];

					// Create a vector iterator to traverse edges
					auto it = find(edgeInfo.begin(), edgeInfo.end(), edge);

					// If end of the vector is reached push edge into vector
					if (it == edgeInfo.end())
						edgeInfo.push_back(edge);
					// If weight found within the iterator is greater than the edge weight reassign new weight to iterator edge
					else if (edge.weight <= (*it).weight)
					{
						(*it).startVertex = edge.startVertex;
						(*it).endVertex = edge.endVertex;
						(*it).weight = edge.weight;
					}
				}

				// If vector of edgeinfo is empty print error
				if (edgeInfo.empty())
				{
					output = "Error: Graph is not connected";

					return;
				}

				// Orders edges from greatest to lowest
				sort(edgeInfo.rbegin(), edgeInfo.rend());

				int endIndex = (int)edgeInfo.size() - 1;
				int v1 = edgeInfo[endIndex].startVertex;
				currentVertex = edgeInfo[endIndex].endVertex;

				// Assemble output to print the start and end vertices visited
				output += vertices[v1].getNode();
				output += ":";
				output += vertices[currentVertex].getNode();
				output += " ";

				// Remove minimum edge 
				edgeInfo.pop_back();
			}

			// Reset vertices visited
			for (int i = 0; i < maxVertices; i++)
			{
				vertexVisits[i] = 0;
			}
		}
		else
		{
			output = "Cannot solve Minimum Spanning Tree since no weights are assigned";
		}
	}

private:
	vector<Vertex<T>> vertices;
	int maxVertices;

	char** adjacencyMatrix;
	char* vertexVisits;

	bool isWeighted;

	// Find the vetices with no successors
	int getVertNoSuccessor(char** adjMat, int size)
	{
		bool edgeFound = false;

		// Traverse all connected vertices and find ones with no edges
		for (int row = 0; row < size; row++)
		{
			edgeFound = false;

			for (int col = 0; col < size; col++)
			{
				// Exit loop if edge found
				if (adjMat[row][col] != 0)
				{
					edgeFound = true;
					break;
				}
			}

			// Return current row index if edge not found
			if (edgeFound == false)
				return row;
		}

		// Return -1 if edge was found
		return -1;
	}
};

#endif