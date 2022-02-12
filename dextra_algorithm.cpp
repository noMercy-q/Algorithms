#include <iostream>
#include <assert.h>
#include <vector>
#include <queue>
#include <set>

struct IGraph {
	virtual ~IGraph() {};

	virtual void AddEdge(int from, int to, int weight) = 0;
	virtual int VerticesCount() const = 0;

	virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
	virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const = 0;
};


class ListGraph : public IGraph {
public:
	explicit ListGraph(int vertexCount);
	explicit ListGraph(const IGraph& graph);

	virtual void AddEdge(int from, int to, int weight) override;
	virtual int VerticesCount() const override;

	virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;
	virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::vector<std::pair<int, int>>> lists;
};

ListGraph::ListGraph(int vertexCount) : lists(vertexCount) {};

ListGraph::ListGraph(const IGraph& graph) : lists(graph.VerticesCount()) {
	for (int i = 0; i < lists.size(); ++i)
		lists[i] = graph.GetNextVertices(i);
}

void ListGraph::AddEdge(int from, int to, int weight) {
	assert(from >= 0 && from < lists.size());
	assert(to >= 0 && to < lists.size());
	std::pair<int, int> node(weight, to);
	lists[from].push_back(node);
}

int ListGraph::VerticesCount() const {
	return lists.size();
}

std::vector <std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < lists.size());
	return lists[vertex];
}

std::vector<std::pair<int, int>> ListGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < lists.size());
	std::vector<std::pair<int, int>> result;
	for (int from = 0; from < lists.size(); ++from)
		for (int i = 0; i < lists[from].size(); ++i)
			if (lists[from][i].second == vertex) {
				std::pair<int, int> node(lists[from][i].first, from);
				result.push_back(node);
				break;
			}
	return result;
}


int findWays(const IGraph& graph, int start, int finish) {

	assert(start >= 0 && start < graph.VerticesCount());
	assert(finish >= 0 && finish < graph.VerticesCount());

	std::vector<int> shortWays(graph.VerticesCount()); 
	for (int i = 0; i < graph.VerticesCount(); i++) {
		shortWays[i] = 50001; // кратчайший путь не может быть больше количества вершин
	}
	shortWays[start] = 0;

	std::pair<int, int> startPair(0, start);

	std::set<std::pair<int, int>> prQ; // 1: distance, 2: vertex
	prQ.insert(startPair);

	while (!prQ.empty()) {
		std::pair<int, int> cur(*prQ.begin());
		prQ.erase(prQ.begin());

		std::vector<std::pair<int, int>> list = graph.GetNextVertices(cur.second);
		for (int i = 0; i < list.size(); ++i) {

			int oldWeight = shortWays[list[i].second];
			if (oldWeight > shortWays[cur.second] + list[i].first) {

				shortWays[list[i].second] = shortWays[cur.second] + list[i].first;
				std::pair<int, int> nodeToErase(oldWeight, list[i].second);
				(prQ.erase(nodeToErase));
				std::pair<int, int> node(shortWays[list[i].second], list[i].second);
				prQ.insert(node);
			}
		}
	}
	return shortWays[finish];
}


int main()
{
	int n = 0, m = 0;
	std::cin >> n >> m;
	ListGraph graph(n);

	int from = 0, to = 0, weight = 0;
	for (int i = 0; i < m; ++i) {
		std::cin >> from >> to >> weight;
		graph.AddEdge(from, to, weight);
		graph.AddEdge(to, from, weight);
	}
	int start = 0, finish = 0;
	std::cin >> start >> finish;
	std::cout << findWays(graph, start, finish);

	return 0;
}