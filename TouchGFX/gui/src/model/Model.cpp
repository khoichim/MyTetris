#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model()
    : modelListener(0), finalScore(0), startLevel(1)
{
}

void Model::tick()
{
    // Không gọi modelTickEvent vì ModelListener không có hàm này
}
