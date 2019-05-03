#pragma once
class Component
{
public:
    Component();
    ~Component();
    static void update(float delta_time);
    static void render();
};

