#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "GraphTheory.h"
#include "cGrid2D.h"

static std::vector<std::string>
tokenize(
    const std::string &line)
{
    std::vector<std::string> ret;
    std::stringstream sst(line);
    std::string a;
    while (getline(sst, a, ' '))
        ret.push_back(a);
    return ret;
}

void findpath(std::istream &ifs)
{
    // A directed graph
    raven::graph::sGraphData graphData;
    graphData.g.clear();
    graphData.g.directed();

    // A 2D grid
    cGrid2D grid;
    std::vector<double> vHeight;

    std::string line;
    int colcount = -1;
    int rowCount = 0;
    int startRow, startCol, endRow, endCol;

    // setup grid
    while (getline(ifs, line))
    {
        auto vt = tokenize(line);

        for (auto &e : vt)
        {
            vHeight.push_back(atof(e.c_str()));
        }
        if (colcount == -1)
            colcount = vt.size();
        else if (colcount != vt.size())
            throw std::runtime_error(
                "variable column count");
        rowCount++;
    }
    grid.setDim(colcount, rowCount);

    // Path can move only downwards
    grid.addDownEdges();

    // convert grid to graph with weighted edges
    graphData.edgeWeight.clear();
    for (auto &p : grid.getEdgesVertexIndex())
    {
        // add edges to graph with weight of the destination vertex
        int ei = graphData.g.add(p.first, p.second);
        graphData.edgeWeight.push_back(vHeight[p.second]);
    }
    // start from any vertex in top row
    int startIndex = graphData.g.vertexCount();
    for (int iv = 0; iv < colcount; iv++)
    {
        int ei = graphData.g.add(startIndex, iv);
        graphData.edgeWeight.push_back(vHeight[iv]);
    }
    // end from any vertex in bottom row
    int endIndex = graphData.g.vertexCount();
    for (int iv = endIndex - 1 - colcount; iv < endIndex - 1; iv++)
    {
        int ei = graphData.g.add(iv, endIndex);
        graphData.edgeWeight.push_back(0);
    }

    // Apply Dijsktra
    graphData.startName = graphData.g.userName(startIndex);
    graphData.endName = graphData.g.userName(endIndex);
    auto res = raven::graph::path(graphData);

    // Display path found ( col, row )
    int r, c;
    for (int i = 1; i < res.first.size() - 1; i++)
    {
        grid.coords(c, r, res.first[i]);
        std::cout << c << "," << r << " -> ";
    }
    std::cout << " total energy " << res.second << "\n";
}

main()
{
    std::string sin =
        "1 1 1 0 1 1 1 1 1 1\n"
        "1 1 1 1 0 1 1 1 1 1\n"
        "1 1 1 1 0 1 1 1 1 1\n"
        "1 1 1 1 1 0 1 1 1 1\n"
        "1 1 1 1 1 0 1 1 1 1\n"
        "1 1 1 1 0 1 1 1 1 1\n"
        "1 1 1 0 1 1 1 1 1 1\n"
        "1 1 1 1 0 1 1 1 1 1\n"
        "1 1 1 1 0 1 1 1 1 1\n"
        "1 1 1 1 1 0 1 1 1 1\n";

    // "o 1 2 3\n"
    // "o 2 3 1\n"
    // "o 3 2 1\n";
    std::istringstream ifs(sin);
    findpath(ifs);

    return 0;
}
