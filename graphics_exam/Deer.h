#pragma once

#include "GameObject.h"
#include "Model.h"


class Deer :
    public GameObject
{
private:
    Model m_model;
public:
    Deer();
    ~Deer();
};

