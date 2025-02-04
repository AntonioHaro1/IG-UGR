/*
=======================================================/
Robot.c 
    Objeto de todo lo relacionado al objeto Robot

*/
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
using namespace std; 
#include "lector-jpg.h"
#include "malla.h"
#include <glm/glm.hpp>


class Mago: public Objeto3D{
  public:
    float anguloBrazo = 0.0f, anguloManox = 0.0f, anguloManoy = 0.0f;
    float tiempo = 0.0f;
    bool subir;
    string modo;  
    //Mallas que componen el Mago
    malla cuerpo;
    malla brazo;
    malla mano;
    int camino = 0;
    int vida;
    bool ataque;

    // constructor
    Mago(string flatSmooth,vector<Objeto3D*> &todosLosObjetos);

    void draw();

    void drawFlat();

    void drawSmooth();
    // get del modo de dibujo
    string getModo();
    void setmodoMago();
    void SetcolorMago(unsigned char color);
    void animacionAtacar();
    int getCamino();
    int getVida();
    void hit();
    void setAtaque();
    bool getAtaque();
};


class Tanque: public Objeto3D{
  public:
    float anguloBrazoIzq = 0.0f, anguloBrazoDer = 0.0f;
    float anguloPiernaIzqSup = 0.0f, anguloPiernaIzqInf = 0.0f;
    float anguloPiernaDerSup = 0.0f, anguloPiernaDerInf = 0.0f;
    float desplazamientoCuerpo = 0.0f;  
    bool avanzar = true;
    bool animacionandar;
    bool animacionatacar;
    float distanciax;
    float x,y;
    GLuint texturaID; 
    int camino = 0;
    int vida;
    bool Ataque;


    string modo;
    //Mallas que componen el Tanque
    malla cuerpo;
    malla brazoIzq;
    malla brazoDer;
    malla piernaAbajoIzq;
    malla piernaAbajoDer;
    malla piernaArribaIzq;
    malla piernaArribaDer;

    // constructor
    Tanque(string flatSmooth,vector<Objeto3D*> &todosLosObjetos);

    void draw();


    void drawAndando();
    void drawFlat();
    //void drawSmooth();
    //void drawFlat2();
    //void drawSmooth2();
    // get del modo de dibujo
    string getModo();
    void setmodoTanque();
    void SetcolorTanque(unsigned char color);
    void animacionAndar();
    void animacionAtacar();
    bool getAndar();
    bool getAtacar();
    void setAndar();
    void setAtacar();
    string getTipoTanque();
    void setTipoTanque();
    void CargarTextura(const char* archivoTextura);
    GLuint getTextura();

    float getPosicion();
    int getCamino();
    int getVida();
    void hit();
    void setAtaque();
    bool getAtaque();


};

