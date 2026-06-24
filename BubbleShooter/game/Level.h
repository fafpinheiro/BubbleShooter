#pragma once
#include <vector>
#include <memory>
#include <Renderer/Renderer.h>//
#include <Renderer/TextureManager.h>
#include "Bubble.h"

struct GridPosition {
    int row, col;
    GridPosition() : row(0), col(0) {}          // default
    GridPosition(int r, int c) : row(r), col(c) {}
};

class Bubble;

class Level
{
public:
    // Initializes playing grid
    void InitializeGrid(int logWidth, int logHeight, float cellSize, float rowHeight);
    // Clear playing grid
    void CleanGrid();

    // Check collision with grid bubbles
    GridPosition CheckBubbleCollision(float x, float y, float width) const;
    // Gets the best cell to attach to
    GridPosition FindAttachCell(Bubble* movingBubble, int hitRow, int hitCol) const;

    // Add a bubble to a grid cell
    void AddBubble(std::unique_ptr<Bubble> bubble, int row, int col);
    // Sets a grid cell
    void SetGridPos(std::unique_ptr<Bubble> bubble, int row, int col);
    // Render grid
    void Render(Renderer& renderer, TextureManager& texManager);

    // Matching
    // Starts DFS - Gets all cells connected by color
    std::vector<GridPosition> GetConnectedSameColor(int row, int col);
    //Deletes inputed cell bubbles
    void Remove(const std::vector<GridPosition>& bubbles);

    // Floating
    // Revemos bubbles not connected to the top
    bool RemoveFloating();
    // Gets if grid is empty
    bool IsGridEmpty() const;
    int GetNRows() const { return m_Rows; };
    int GetNCols() const { return m_Cols; };

    // Utils
    // Converts coords from world to grid
    GridPosition WorldToGrid(float x, float y) const;
    // Converts coords from grid to world
    std::pair<float, float> GridToWorld(int row, int col) const;

    std::string GetTexIdFromColor(BubbleColor color) const;

private:

    std::vector<GridPosition> GetNeighbors(int row, int col) const;

    int GetGridIndex(int row, int col) const;

    bool IsValid(int row, int col) const ;

private:
    int m_Rows = 0;                                 // Grid n rows
    int m_Cols = 0;                                 // Grid n cols
    float m_CellSize = 32.0f;                       // Grid cell size
    float m_RowHeight = 27.2f;                      // Grid row size

    std::vector<std::unique_ptr<Bubble>> m_Grid;    // Grid positions and is state
};