#include <iostream>
#include <assert.h>
#include <vector>
#include <unordered_set>

class SetGraph : public IGraph {
public:
	explicit SetGraph(int vertexCount);
	explicit SetGraph(const IGraph& graph);

	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;

	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::unordered_set<int>> sets;
};

SetGraph::SetGraph(int vertexCount) : sets(vertexCount) {};

SetGraph::SetGraph(const IGraph& graph) : sets(graph.VerticesCount()) {
	for (int i = 0; i < sets.size(); i++) {
		std::vector<int> list(graph.GetNextVertices(i));
		for (int j = 0; j < list.size(); j++)
			sets[i].insert(list[j]); // хотелось бы сразу строить хеш на векторе, 
									//но вне конструктора это сделать проблематично..
	}
}

void SetGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < sets.size());
	assert(to >= 0 && to < sets.size());
	sets[from].insert(to);
}

int SetGraph::VerticesCount() const {
	return sets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < sets.size());
	std::vector<int> result(sets[vertex].begin(), sets[vertex].end());
	return result;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < sets.size());
	std::vector<int> result;
	for (int from = 0; from < sets.size(); from++)
		if (sets[from].find(vertex) != sets[from].end()) // для u_set работает за константу
			result.push_back(from);

	return result;
}
