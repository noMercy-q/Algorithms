#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "IGraph.h"

class MatrixGraph : public IGraph {
public: 
	explicit MatrixGraph(int vertexCount);
	explicit MatrixGraph(const IGraph& graph);

	virtual void AddEdge(int from, int to) override;
	virtual int VerticesCount() const override;

	virtual std::vector<int> GetNextVertices(int vertex) const override;
	virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
	std::vector<std::vector<bool>> matrix;
};

MatrixGraph::MatrixGraph(int vertexCount) {
	matrix.assign(vertexCount, std::vector<bool>(vertexCount));
	for (int i = 0; i < vertexCount; i++)
		std::fill(matrix[i].begin(), matrix[i].end(), 0);
}

MatrixGraph::MatrixGraph(const IGraph& graph) {
	int vertexCount = graph.VerticesCount();
	matrix.assign(vertexCount, std::vector<bool>(vertexCount));
	for (int i = 0; i < vertexCount; i++)
		std::fill(matrix[i].begin(), matrix[i].end(), 0);


	for (int i = 0; i < vertexCount; i++) {
		std::vector<int> list(graph.GetNextVertices(i));
		for (int j = 0; j < list.size(); j++)
			matrix[i][list[j]] = 1;
	}
}

void MatrixGraph::AddEdge(int from, int to) {
	assert(from >= 0 && from < matrix.size());
	assert(to >= 0 && to < matrix.size());
	matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const {
	return matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
	assert(vertex >= 0 && vertex < matrix.size());
	std::vector<int> list;
	for (int i = 0; i < matrix.size(); i++)
		if (matrix[vertex][i])
			list.push_back(i);
	return list;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
	assert(vertex >= 0 && vertex < matrix.size());
	std::vector<int> list;
	for (int i = 0; i < matrix.size(); i++)
		if (matrix[i][vertex])
			list.push_back(i);
	return list;
}