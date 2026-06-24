#include "Level.h"

void Level::InitializeGrid(int logWidth, int logHeight, float cellSize, float rowHeight)
{
    m_CellSize = cellSize;
    m_RowHeight = rowHeight;
    int rows = static_cast<int>(logHeight / m_RowHeight);
    int cols = static_cast<int>(logWidth / cellSize); 
    std::vector<std::vector<int>> m_Layout = {
        {-1,-1,0, 1, 2, 3, 0, 1},
          {-1,-1,1, 2, 3, 0, 1, 2},
        {-1,-1,2, 3, 0, 1, 2, 3},
          {-1,-1,-1, -1, -1, 2, -1, -1},
          {-1,-1,3, 0, 1, 2, 3, 0},
        {-1,-1,0, 3, 3, 3, 3, 3}
    };
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (row < m_Layout.size() && col < m_Layout[row].size())
            {
                // Random color

                if (m_Layout[row][col] != -1)
                {
                    // Add bubble in init cell
                    BubbleColor color = static_cast<BubbleColor>(m_Layout[row][col]);
                    std::unique_ptr<Bubble> bubble = std::make_unique<Bubble>(0.0f, 0.0f, cellSize, color);

                    // Convert grid - world position
                    auto [x, y] = GridToWorld(row, col);
                    bubble->SetPosition(x, y);
                    bubble->StopAtGrid();

                    AddBubble(std::move(bubble), row, col);
                }

                else
                {
                    // Add empty postion
                    AddBubble(nullptr, row, col);
                
                }
            }
            else
            {
                //Fill window empty space with grid
                AddBubble(nullptr, row, col);
            
            }
        }
    }


}

void Level::CleanGrid()
{
    m_Rows = 0;
    m_Cols = 0;
    m_CellSize = 32.0f;
    m_RowHeight = 27.2f;
    m_Grid.clear();
}

GridPosition Level::CheckBubbleCollision(float x, float y, float width) const
{
    GridPosition approxCell = WorldToGrid(x,y);

    for (int r = approxCell.row - 2; r <= approxCell.row + 2; ++r)
    {
        for (int c = approxCell.col - 2; c <= approxCell.col + 2; ++c)
        {
            if (!IsValid(r, c)) continue;

            auto& cell = m_Grid[GetGridIndex(r, c)];
            if (!cell) continue;

            float dx = x - cell->GetX();
            float dy = y - cell->GetY();

            float dist2 = sqrt((dx * dx) + (dy * dy));

            if (dist2 <= (width * 0.9f))
            { // check if close enouth to consider collision
                return GridPosition{ r,c };
            }
        }
    }
    return GridPosition{ -1, -1 };
}


GridPosition Level::FindAttachCell(Bubble* moving, int hitRow, int hitCol) const
{
    std::vector<GridPosition> neighbors = GetNeighbors(hitRow, hitCol);

    float bestDist = FLT_MAX;
    GridPosition bestCell = { -1, -1 };

    if (IsValid(hitRow, hitCol) && m_Grid[GetGridIndex(hitRow, hitCol)] == nullptr) {
        return bestCell = { hitRow, hitCol };
    }

    for (GridPosition neighbor : neighbors)
    {
        if (!IsValid(neighbor.row, neighbor.col)) continue;
        auto& gridPos = m_Grid[GetGridIndex(neighbor.row, neighbor.col)];
        if (gridPos != nullptr) continue;

        auto [wx, wy] = GridToWorld(neighbor.row, neighbor.col);
        float dx = wx - moving->GetX();
        float dy = wy - moving->GetY();
        float dist2 = sqrt((dx * dx) + (dy * dy));

        if (dist2 < bestDist)
        {
            bestDist = dist2;
            bestCell = { neighbor.row, neighbor.col };
        }
    }

    return bestCell;
}

void Level::AddBubble(std::unique_ptr<Bubble> bubble, int row, int col)
{

    if (row >= m_Rows) m_Rows++;
    if (col >= m_Cols) m_Cols++;

    if (bubble != nullptr)
    {
        // Add new bubble at pos
        auto [x, y] = GridToWorld(row, col);
        bubble->SetPosition(x, y);
        bubble->StopAtGrid();

        m_Grid.push_back(std::move(bubble));
    }
    else
    {
        // Empty positions
        m_Grid.push_back(nullptr);

    }
}

void Level::SetGridPos(std::unique_ptr<Bubble> bubble, int row, int col)
{
    //Snap bubble in the cell
    m_Grid[row * m_Cols + col] = std::move(bubble);
    auto [x,y] = GridToWorld(row, col);
    // Update Bubble pos
    m_Grid[GetGridIndex(row, col)]->SetPosition(x,y);
}

void Level::Render(Renderer& renderer, TextureManager& texManager)
{
    for (int r = 0; r < m_Rows; ++r)
    {

        for (int c = 0; c < m_Cols; ++c)
        {

            if (!m_Grid[GetGridIndex(r, c)]) continue;

            Texture* tex = texManager.GetTexture(GetTexIdFromColor(m_Grid[GetGridIndex(r, c)]->GetColor()));

            renderer.DrawTexture(tex,
                m_Grid[GetGridIndex(r, c)]->GetX(),
                m_Grid[GetGridIndex(r, c)]->GetY(),
                m_CellSize,
                m_CellSize);
        }
    }
}

std::vector<GridPosition> Level::GetConnectedSameColor(int row, int col)
{   
    std::vector<GridPosition> result;

    //int startIdx = row * m_Cols + col;
    if (!m_Grid[ GetGridIndex(row, col)]) return result;

    std::vector<char> visited(m_Rows * m_Cols, 0);

    std::vector<GridPosition> stack;
    stack.push_back({ row, col });

    while (!stack.empty())
    {
        // LIFO gets last added
        auto [r, c] = stack.back();
        stack.pop_back(); // Removes row col from stack

        if (!IsValid(r, c)) continue;
        if (visited[GetGridIndex(r, c)]) continue;
        if (!m_Grid[GetGridIndex(r, c)]) continue;
        if (m_Grid[GetGridIndex(r, c)]->GetColor() != m_Grid[GetGridIndex(row, col)]->GetColor()) continue;

        // Adds new connected tho result
        visited[GetGridIndex(r, c)] = 1;
        result.push_back({ r,c });

        // Adds the row and col neighbors to the stack
        for (GridPosition neighbor : GetNeighbors(r, c))
            stack.push_back(GridPosition{ neighbor.row, neighbor.col });
    }
    return result;
}

void Level::Remove(const std::vector<GridPosition>& bubbles)
{
    for (auto [r, c] : bubbles)
        m_Grid[GetGridIndex(r, c)].reset();
}

bool Level::RemoveFloating()
{  
    bool wasRemoved = false;
    std::vector<char> visited(m_Rows * m_Cols, 0);

    std::vector<GridPosition> stack;
    stack.reserve(m_Rows * m_Cols); // avoid reallocations, expect to have full grid worst case

    // Push all top row bubbles into stack
    for (int c = 0; c < m_Cols; ++c)
    {
        int idx = GetGridIndex(0, c);

        if (m_Grid[idx])
        {
            stack.push_back({ 0, c });
            visited[idx] = 1;
        }
    }

    // Iterative DFS adds to stack all bubbles connected to top
    while (!stack.empty())
    {
        auto [r, c] = stack.back();
        stack.pop_back();

        for (GridPosition neighbor : GetNeighbors(r, c))
        {
            if (!IsValid(neighbor.row, neighbor.col)) continue;

            if (visited[GetGridIndex(neighbor.row, neighbor.col)]) continue;
            if (!m_Grid[GetGridIndex(neighbor.row, neighbor.col)]) continue;

            visited[GetGridIndex(neighbor.row, neighbor.col)] = 1;
            stack.push_back(GridPosition{ neighbor.row, neighbor.col });
        }
    }

    // Remove all unvisited bubbles that dont connect to top
    for (int r = 0; r < m_Rows; ++r)
    {
        for (int c = 0; c < m_Cols; ++c)
        {
            int idx = GetGridIndex(r, c);

            if (m_Grid[idx] && !visited[idx])
            {
                m_Grid[idx].reset();
                wasRemoved = true;
            }
        }
    }

    return wasRemoved;
}

bool Level::IsGridEmpty() const
{
    for (const auto& bubble : m_Grid)
    {
        if (bubble != nullptr)
            return false;
    }
    return true;
}

GridPosition Level::WorldToGrid(float x, float y) const
{

    // correct row height for hex grid
    float rowFloat = y / m_RowHeight;
    int row = static_cast<int>(round(rowFloat));

    float offsetX = (row % 2 == 1) ? (m_CellSize * 0.5f) : 0.0f;

    float colFloat = (x - offsetX) / m_CellSize;
    int col = static_cast<int>(round(colFloat));

    return { row, col };
}

std::pair<float, float> Level::GridToWorld(int row, int col) const
{
    // Convert from row and col to grid coords
    float offsetX = (row % 2 == 1) ? (m_CellSize / 2.0f) : 0.0f;

    float x = col * m_CellSize + offsetX;
    float y = row * m_RowHeight;

    return { x, y };
}

std::string Level::GetTexIdFromColor(BubbleColor color) const
{
    std::string tmp;
    switch (color) {
    case BubbleColor::Red:
        tmp = "bubble_circle";
        break;
    case BubbleColor::Green:
        tmp = "bubble_x";
        break;
    case BubbleColor::Blue:
        tmp = "bubble_square";
        break;
    case BubbleColor::Yellow:
        tmp = "bubble_triangle";
        break;
    }
    return tmp;
}

std::vector<GridPosition> Level::GetNeighbors(int row, int col) const
{
    std::vector<GridPosition> neighbors;

    // Offsets for even vs odd rows
    static const int evenRowOffsets[6][2] = {
        {-1,  0}, // top-left
        {-1, +1}, // top-right
        { 0, -1}, // left
        { 0, +1}, // right
        {+1,  0}, // bottom-left
        {+1, +1}  // bottom-right
    };

    static const int oddRowOffsets[6][2] = {
        {-1, -1}, // top-left
        {-1,  0}, // top-right
        { 0, -1}, // left
        { 0, +1}, // right
        {+1, -1}, // bottom-left
        {+1,  0}  // bottom-right
    };
    const auto& offsets = (row % 2 == 1) ? evenRowOffsets : oddRowOffsets;

    for (int i = 0; i < 6; ++i)
    {
        int nRow = row + offsets[i][0];
        int nCol = col + offsets[i][1];

        // Check bounds
        neighbors.push_back(GridPosition{ nRow, nCol });
    }

    return neighbors;
}

int Level::GetGridIndex(int row, int col) const
{
    return row * m_Cols + col;
}

bool Level::IsValid(int row, int col) const
{
    return row >= 0 && row < m_Rows&&
             col >= 0 && col < m_Cols;
}
