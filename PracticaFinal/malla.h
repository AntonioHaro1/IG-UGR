#ifndef MALLA_H
#define MALLA_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		
#include "practicasIG.h"
using namespace std; 
#include "lector-jpg.h"


class Objeto3D
{
    public: 
    unsigned char color;
    GLboolean seleccion;
    string tipo;

    Objeto3D();

    virtual void draw() = 0; 
    void setSeleccion();
    void Setcolor(unsigned char color);
    GLboolean getSeleccion();
    unsigned char getId();
    string getTipo();
    void setTipo(string cad);

};

class malla: public Objeto3D{
    private:
        vector<float> vertices_ply;
        vector<int> caras_ply;
        vector<float> normales_caras;
        vector<float> normales_vertices;
        string modo;   
        vector<float> texturas_ply;
        GLuint Text;


    public:

    malla();
    malla(std::string nombrearchivo, string flatSmooth,GLboolean si);

    void draw();
    void drawFlat(GLuint texturaID);
    void drawFlatNotexture();
    void drawFlat();
    void drawSmooth();

    void calcular_normales();
    string getModo();

    void SetcolorMalla(unsigned char color);
    GLboolean getSeleccionMalla();
    void setModoMalla();

    void CargarTextura(const char* archivo);
    GLuint getText();
};


#endif // MALLA_H