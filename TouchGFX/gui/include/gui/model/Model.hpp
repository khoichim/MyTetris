#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    // Truyền điểm giữa GameScreen và GameOverScreen
    void     setFinalScore(uint32_t s) { finalScore = s; }
    uint32_t getFinalScore()           { return finalScore; }

    void setStartLevel(int l) { startLevel = l; }
    int  getStartLevel()      { return startLevel; }
protected:
    ModelListener* modelListener;

private:
    uint32_t finalScore;
    int startLevel;
};



#endif
