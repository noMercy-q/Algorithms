#include <iostream>
#include <assert.h>
#include <vector>
#include <queue>
#include <unordered_set>

struct IGraph {
	virtual ~IGraph() {};

	virtual void AddEdge(int from, int to) = 0;
	virtual int VerticesCount() const = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
	explicit ListGraph(int vertexCount);
	explicit ListGraph(const IGraph& graph);

	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;

	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::vector<int>> lists;
};

ListGraph::ListGraph(int vertexCount) : lists(vertexCount) {};

ListGraph::ListGraph(const IGraph& graph) : lists(graph.VerticesCount()) {
	for (int i = 0; i < lists.size(); i++)
		lists[i] = graph.GetNextVertices(i);
}

void ListGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < lists.size());
	assert(to >= 0 && to < lists.size());
	lists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
	return lists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < lists.size());
	return lists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < lists.size());
	std::vector<int> result;
	for (int from = 0; from < lists.size(); from++)
		for (int i = 0; i < lists[from].size(); i++)
			if (lists[from][i] == vertex) {
				result.push_back(from);
				break;
			}
	return result;
}

int findWays(const IGraph& graph, int start, int finish) {

	assert(start >= 0 && start < graph.VerticesCount());
	assert(finish >= 0 && finish < graph.VerticesCount());

	std::vector<bool> visited(graph.VerticesCount(), false);
	std::vector<std::pair<int, int>> routes(graph.VerticesCount()); // 1: amount, 2: distance
	for (int i = 0; i < graph.VerticesCount(); i++) {
		routes[i].first = 0;
		routes[i].second = 50001; // кратчайший путь не может быть больше количества вершин
	}
	routes[start].first = 1;
	routes[start].second = 0;

	std::queue<int> q;
	q.push(start);
	visited[start] = true;

	while (q.size() > 0) {
		int cur = q.front();
		q.pop();

		std::vector<int> list = graph.GetNextVertices(cur);
		for (int i = 0; i < list.size(); i++) {
			
			if (!visited[list[i]]) {
				q.push(list[i]);
				visited[list[i]] = 1;
			}

			// если текущий путь короче уже имеющегося, то меняем количество и длину на значения для более короткого пути
			if (routes[list[i]].second > routes[cur].second + 1) {
				routes[list[i]].second = routes[cur].second + 1;
				routes[list[i]].first = routes[cur].first;
			}
			// если длина имеющегося пути такая же, то складываем количество путей, а длину оставляем
			else if (routes[list[i]].second == routes[cur].second + 1)
				routes[list[i]].first += routes[cur].first;
		}
	}
	assert(routes[finish].first < 50001);
	return routes[finish].first;
}

int main()
{
	int v = 0, n = 0;
	std::cin >> v >> n;
	ListGraph graph(v);

	int from = 0, to = 0;
	for (int i = 0; i < n; i++) {
		std::cin >> from >> to;
		graph.AddEdge(from, to);
		graph.AddEdge(to, from);
	}

	int u = 0, w = 0;
	std::cin >> u >> w;
	std::cout << findWays(graph, u, w);

	return 0;
}