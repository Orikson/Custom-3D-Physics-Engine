// Kernel for SDL fragment shader

#ifndef _KERNEL_H
#define _KERNEL_H

#include "../../common.h"

class Kernel {
    public:
        int start(const char* windowTitle, int rx, int ry);
    private:
        bool isRunning = true;

        int initSDL();
        SDL_Window* createWindow(const char* windowTitle, int width, int height);
        SDL_Renderer* createRenderer(SDL_Window* window);
        void update(vector<Shape*>& shapes);
        void render(SDL_Window* window);
        void cleanUp(SDL_Window* window, SDL_GLContext &glContext);
        void events(SDL_Window* window);
        bool saveImage(SDL_Window* window, SDL_Renderer* renderer, const char* file);
        bool saveImage(SDL_Surface* surface, const char* file);
        void setShader();
        void setPos(float x, float y, float z);
        void setDir(float theta, float phi);

        float cameraPos[3];
        float cameraRot[3];
        float curTheta;
        float curPhi;

        bool wDown; // w
        bool sDown; // s
        bool aDown; // a
        bool dDown; // d
        bool spDown; // spacebar
        bool shDown; // shift
        bool ctrlDown; // ctrl
        bool enDown; // enter

        float frame, curtime = 0;
        GLuint ps, vs, prog, iFrame, iTime;
        GLint cPos, cRot;
        GLuint ssbo = 0;
        SDL_Surface* sumSurface;
        int resolution[2];

};

#include "kernel.cpp"

#endif