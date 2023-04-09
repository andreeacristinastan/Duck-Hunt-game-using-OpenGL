#include "lab_m1/Tema1/tema1.h"
#include "components/text_renderer.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <random>

#include "lab_m1/Tema1/tema1_vis2D.h"
#include "lab_m1/Tema1/object2D_tema1.h"
#include "lab_m1/Tema1/transform2D_tema1.h"

using namespace std;
using namespace m1;

float randAngle;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    cx = 50;
    cy = 50;

    x1_i = 0;
    x2_i = 230;
    y1_i = 0;
    y2_i = 230;
    translate_square_x = -210;
    translate_square_y = -105;

    time_to_leave = 0;
    time_to_die = 0;
    escaped = 0;
    shots = 3;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;
    moveR = false;
    // Initialize sx and sy (the scale factors)
    scaleX = 0;
    scaleY = 0;

    // Initialize angularStep
    angularStep = 0;
    rotate_init = 0;

    Mesh* circle_head = object2D_tema1::CreateCircle("circle_head", 0.0f, 0.0f, 35.0f, glm::vec3(NormalizedRGB(0, 128, 0)), true);
    AddMeshToList(circle_head);

    Mesh* circle_cursor = object2D_tema1::CreateCircle("circle_cursor", 0.0f, 0.0f, 20.0f, glm::vec3(1, 1, 1), false);
    AddMeshToList(circle_cursor);

    Mesh* triangle_wing_st = object2D_tema1::CreateTriangle("triangle_wing_st", corner, 25, glm::vec3(NormalizedRGB(135, 103, 90)), true);
    AddMeshToList(triangle_wing_st);

    Mesh* triangle_wing_nd = object2D_tema1::CreateTriangle("triangle_wing_nd", corner, 25, glm::vec3(NormalizedRGB(135, 103, 90)), true);
    AddMeshToList(triangle_wing_nd);

    Mesh* triangle_body = object2D_tema1::CreateTriangle("triangle_body", corner, 50, glm::vec3(NormalizedRGB(135, 103, 90)), true);
    AddMeshToList(triangle_body);

    Mesh* triangle_mouth = object2D_tema1::CreateTriangle("triangle_mouth", corner, 10, glm::vec3(NormalizedRGB(255, 215, 0)), true);
    AddMeshToList(triangle_mouth);

    Mesh* circle_cloud = object2D_tema1::CreateCircle("circle_cloud", 0.0f, 0.0f, 40.0f, glm::vec3(1, 1, 1), true);
    AddMeshToList(circle_cloud);

    Mesh* circle_eye_st = object2D_tema1::CreateCircle("circle_eye_st", 0.0f, 0.0f, 10.0f, glm::vec3(1, 1, 1), true);
    AddMeshToList(circle_eye_st);

    Mesh* circle_eye_nd = object2D_tema1::CreateCircle("circle_eye_nd", 0.0f, 0.0f, 5.0f, glm::vec3(0,0,0), true);
    AddMeshToList(circle_eye_nd);

    Mesh* circle_lives = object2D_tema1::CreateCircle("circle_lives", 0.0f, 0.0f, 10.0f, glm::vec3(1, 0, 0), true);
    AddMeshToList(circle_lives);

    Mesh* lives_lost = object2D_tema1::CreateCircle("lives_lost", 0.0f, 0.0f, 10.0f, glm::vec3(255, 255, 255), true);
    AddMeshToList(lives_lost);

    Mesh* rectangle = object2D_tema1::CreateSquare("rectangle", corner, 50, 11, glm::vec3(0.5, 1, 0.5), true);
    AddMeshToList(rectangle);

    Mesh* shots_lost = object2D_tema1::CreateSquare("shots_lost", corner, 50, 11, glm::vec3(50.2, 50.2, 50.2), true);
    AddMeshToList(shots_lost);

    Mesh* weed = object2D_tema1::CreateSquare("weed", corner, 50, 1300, glm::vec3(0, 0.5f, 0), true);
    AddMeshToList(weed);

    Mesh* score = object2D_tema1::CreateSquare("score", corner, 50, 250, glm::vec3(1, 1, 1), false);
    AddMeshToList(score);

    Mesh* score_bar = object2D_tema1::CreateSquare("score_bar", corner, 40, 7, glm::vec3(0.5, 1, 0.5), true);
    AddMeshToList(score_bar);

    Mesh* rectangle_duck = object2D_tema1::CreateSquare("rectangle_duck", corner, 230, 230, glm::vec3(0.5, 1, 0.5), true);
    AddMeshToList(rectangle_duck);

    randNumX = rand() % 800 + 20;
    randAngle = rand() % 160 + 20;
    std::cout << randAngle;
    if ((75 <= randAngle && randAngle <= 105) || randAngle >= 170) {
        randAngle = 45;
    }

    if (randNumX <= 10) {
        randNumX = 40;
    }

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 38);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.5, 0.8, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    int pos_clouds = 100;
    pos_lives = 550;
    pos_shots = 50;
    pos_score = resolution.x - 252;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D_tema1::Translate(coord_mouse_X, coord_mouse_Y);
    RenderMesh2D(meshes["circle_cursor"], shaders["VertexColor"], modelMatrix);

    if (lives != 0 && score == 24) {
        win = 1;
        DrawTEXT();
    }

    if (start != 0 && score != 24) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D_tema1::Translate(resolution.x - 255, 1.5);
        RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);

        for (int i = 0; i < score; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D_tema1::Translate(pos_score, 7);
            RenderMesh2D(meshes["score_bar"], shaders["VertexColor"], modelMatrix);

            pos_score += 10;
        }

        if (shots > 0) {
            for (int i = 0; i < shots; i++) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D_tema1::Translate(pos_shots, 0);
                RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

                pos_shots += 50;
            }

            for (int i = shots; i < 3; i++) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D_tema1::Translate(pos_shots, 0);
                RenderMesh2D(meshes["shots_lost"], shaders["VertexColor"], modelMatrix);

                pos_shots += 50;
            }
        }

        for (int i = 0; i < lives; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D_tema1::Translate(pos_lives, 30);
            RenderMesh2D(meshes["circle_lives"], shaders["VertexColor"], modelMatrix);
            pos_lives += 50;
        }

        for (int i = lives; i < 3; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D_tema1::Translate(pos_lives, 30);
            RenderMesh2D(meshes["lives_lost"], shaders["VertexColor"], modelMatrix);
            pos_lives += 50;
        }
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D_tema1::Translate(0, 0);
    RenderMesh2D(meshes["weed"], shaders["VertexColor"], modelMatrix);

    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 3; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D_tema1::Translate(pos_clouds, 650);
            RenderMesh2D(meshes["circle_cloud"], shaders["VertexColor"], modelMatrix);

            pos_clouds += 50;
        }
        pos_clouds += 100;
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(-43, 0);
    RenderMesh2D(meshes["circle_eye_nd"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(-43, 0);
    RenderMesh2D(meshes["circle_eye_st"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(-43, 0);
    RenderMesh2D(meshes["circle_head"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(-153, -10);

    if (checkR) {
        angularStep += deltaTimeSeconds;
        modelMatrix *= transform2D_tema1::Translate(cx, cy);
        modelMatrix *= transform2D_tema1::Rotate(angularStep);
        modelMatrix *= transform2D_tema1::Translate(-cx, -cy);
        if (angularStep > 0.6) {
            checkR = false;
        }
    }
    else {
        angularStep -= deltaTimeSeconds;

        if (angularStep <= 0.3) {
            checkR = true;
        }
    }

    RenderMesh2D(meshes["triangle_wing_st"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(-108, 0);

    if (checkR) {
        modelMatrix *= transform2D_tema1::Translate(cx, cy);
        modelMatrix *= transform2D_tema1::Rotate(-angularStep);
        modelMatrix *= transform2D_tema1::Translate(-cx, -cy);
        if (angularStep > 0.6) {
            checkR = false;
        }
    }
    else {
        if (angularStep <= 0.3) {
            checkR = true;
        }
    }

    RenderMesh2D(meshes["triangle_wing_nd"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(-143, 0);

    RenderMesh2D(meshes["triangle_body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= modelMatrixDuck;
    modelMatrix *= transform2D_tema1::Translate(0, 0);

    RenderMesh2D(meshes["triangle_mouth"], shaders["VertexColor"], modelMatrix);


    if (start != 0) {
        modelMatrixDuck = glm::mat3(1);
        DrawTEXT();

        time_to_leave += deltaTimeSeconds;
        if ((time_to_leave >= 20.0f && time_to_die != -1) || shots <= 0) {
            randAngle = 90;
            escaped = 1;

            if (translateY >= resolution.y + 400) {
                if (lives != 0) {
                    total_ducks++;
                    if (total_ducks % 5 == 0) {
                        speed += 100;
                    }

                    lives--;

                    Init();
                }
            }
        }
        else if (time_to_die == -1) {
            randAngle = -90;
            killed = 1;

            if (translateY <= -400) {
                score++;
                if (lives != 0) {
                    total_ducks++;
                    if (total_ducks % 5 == 0) {
                        speed += 100;
                    }
                    Init();
                }
            }
        }
        else {
            if (translateX + randNumX >= resolution.x + 100 || translateX + randNumX <= 0) {
                randAngle = 180 - randAngle;
                float new_translateX = translateX + speed * deltaTimeSeconds * cos(glm::radians(randAngle));

                if (new_translateX + randNumX >= resolution.x + 100) {
                    translateX = resolution.x - 100;
                }
                else if (new_translateX + randNumX <= 0) {
                    translateX = 100;
                }
            }
            else if (translateY >= resolution.y || translateY < 0) {
                randAngle = -randAngle;
                float new_translateY = translateY + speed * deltaTimeSeconds * sin(glm::radians(randAngle));

                if (new_translateY >= resolution.y) {
                    translateY = resolution.y - 100;
                }
                else if (new_translateY < 0) {
                    translateY = 100;
                }
            }
        }

        translateX += speed * deltaTimeSeconds * cos(glm::radians(randAngle));
        translateY += speed * deltaTimeSeconds * sin(glm::radians(randAngle));

        modelMatrixDuck *= transform2D_tema1::Translate(translateX + randNumX, translateY);

        modelMatrixDuck *= transform2D_tema1::Translate(-98, 10);
        modelMatrixDuck *= transform2D_tema1::Rotate(glm::radians(randAngle));
        modelMatrixDuck *= transform2D_tema1::Translate(98, -10);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D_tema1::Translate(-210, -105);
        modelMatrix *= transform2D_tema1::Translate(translateX + randNumX, translateY);
        //RenderMesh2D(meshes["rectangle_duck"], shaders["VertexColor"], modelMatrix);
    }
    else {
        modelMatrixDuck = glm::mat3(1);
        modelMatrixDuck *= transform2D_tema1::Translate(resolution.x / 2 + 100, resolution.y / 2);
        if (check) {
            scaleX += deltaTimeSeconds;

            if (scaleX >= 2) {
                check = false;
            }
        }
        else {
            scaleX -= deltaTimeSeconds;

            if (scaleX < 0.5) {
                check = true;
            }
        }

        modelMatrixDuck *= transform2D_tema1::Scale(scaleX, scaleX);
    }
}

void Tema1::DrawTEXT()
{
    const float kTopY = 10.f;
    const float kRowHeight = 25.f;

    int rowIndex = 0;
    if (escaped == 1) {
        textRenderer->RenderText("THE DUCK ESCAPED", 400.0f, 500 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    }
    else if (start == 0) {
        textRenderer->RenderText("Press SPACE to let the game begin", 250.0f, 500 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);

    } 
    else if (killed == 1) {
        textRenderer->RenderText("THE DUCK WAS KILLED", 400.0f, 500 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        killed = 0;
    }
    else if (lives == 0) {
        textRenderer->RenderText("GAME OVER :( IF YOU WANT TO START OVER PRESS S", 100.0f, 500 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        randAngle = 0;
        randNumX = -300;
        start = -3;

    }
    else if (win == 1) {
        textRenderer->RenderText("YOU WON :) IF YOU WANT TO START OVER PRESS S", 100.0f, 500 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);

        win = 0;
    }
    else {
        textRenderer->RenderText("YOUR SCORE:", 1025.0f, 630 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("YOUR SHOTS:", 0.0f, 605 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("YOUR LIVES:", 490.0f, 580 + kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    }
}

void Tema1::FrameEnd()
{
    if (start == 0) {
        DrawTEXT();
    }
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (score == 24 || lives == 0 || start != 1) {

        if (key == GLFW_KEY_SPACE)
        {
            start = 1;

        }
        else if (key == GLFW_KEY_S) {
            escaped = 0;
            killed = 0;
            shots = 3;
            lives = 3;
            total_ducks = 1;
            speed = 100;
            score = 0;
            time_to_leave = 0;
            time_to_die = 0;
            translateX = 0;
            translateY = 0;
            moveR = false;
            check_init = true;
            angularStep = 0;
            rotate_init = 0;
            Init();
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();

    coord_mouse_X = mouseX;
    coord_mouse_Y = resolution.y - mouseY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::ivec2 resolution = window->GetResolution();

    mouseY = resolution.y - mouseY;

    x1 = (x1_i + translate_square_x) + translateX + randNumX;
    x2 = (x2_i + translate_square_x) + translateX + randNumX;

    y1 = (y1_i + translate_square_y) + translateY;
    y2 = (y2_i + translate_square_y) + translateY;

    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2) {
        if ((x1 <= mouseX) && (mouseX <= x2) && (y1 <= mouseY) && (mouseY <= y2) && time_to_leave < 20.0f) {
            time_to_die = -1;
        }
        else if (time_to_die != -1 && time_to_leave < 20.0f && score != 24 && lives != 0) {
            shots--;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}