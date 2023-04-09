#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void DrawTEXT();

    protected:
        const glm::vec3 kTextColor = glm::vec3(1, 1, 1);
        const glm::vec3 kBackgroundColor = NormalizedRGB(127, 255, 212);
        gfxc::TextRenderer* textRenderer;

        int rotate_init = 0;
        int start = 0;
        int win = 0;
        float cx, cy;
        int died_x = 5;
        int died_y = 5;
        float x1;
        float x2;
        float y1;
        float y2;
        float x1_i;
        float y1_i;
        float x2_i;
        float y2_i;
        float translate_square_x;
        float translate_square_y;

        glm::mat3 modelMatrix;
        glm::mat3 modelMatrixDuck;
        float speed = 100;
        int score = 0;
        float time_to_leave;
        int time_to_die;
        int total_ducks = 1;
        int lives = 3;
        int pos_lives = 30;
        int pos_score = 1000;
        int shots = 3;
        int pos_shots = 25;
        int pos_clouds = 100;
        int escaped = 0;
        int killed = 0;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        int randNumX;
        bool moveR;
        bool check;
        bool checkR;
        bool check_init = true;

        int coord_mouse_X;
        int coord_mouse_Y;

        float left;
        float right;
        float top;
        float bottom;

    };
}   // namespace m1
