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
#include <GL/glut.h> // Libreria de utilidades de OpenGL
#include "practicasIG.h"
using namespace std;
#include "lector-jpg.h"
#include "enemigos.h"

/* Funciones de mago*/
Mago::Mago(string flatSmooth, vector<Objeto3D *> &todosLosObjetos)
{
  this->modo = flatSmooth;

  malla cuerpo("ObjetosPly/wizard_torso.ply", "GL_FLAT", false);
  malla mano("ObjetosPly/wizard_mano.ply", "GL_FLAT", false);
  malla brazo("ObjetosPly/wizard_brazo.ply", "GL_FLAT", false);
  this->cuerpo = cuerpo;
  this->mano = mano;
  this->brazo = brazo;
  this->camino = rand() % 3 + 1;  
  this->vida = 3;
  this->ataque = false;
  this->subir = true;

  todosLosObjetos.push_back(this);
}

void Mago::draw()
{
}

void Mago::drawFlat()
{
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  cuerpo.drawFlat();

  glTranslatef(0.15, 0, 1.1);
  glRotatef(-anguloBrazo, 0, 1, 0);
  glTranslatef(-0.15, 0, -1.1);
  brazo.drawFlat();
  glPushMatrix();
  glTranslatef(0, 0, 0.9);
  glRotatef(anguloManox, 1.0f, 0.0f, 0.0f);
  glRotatef(anguloManoy, 0.0f, 1.0f, 0.0f); 
  glTranslatef(0, 0, -0.87);
  mano.drawFlat();
  glPopMatrix();
  glPopMatrix();
}

void Mago::drawSmooth()
{
  glPushMatrix();
  cuerpo.drawSmooth();
  mano.drawSmooth();
  brazo.drawSmooth();
  glPopMatrix();
}

// get del modo de dibujo
string Mago::getModo()
{
  return this->modo;
}

void Mago::SetcolorMago(unsigned char color)
{
  Setcolor(color);
}

void Mago::setmodoMago()
{
  if (this->modo == "GL_FLAT")
  {
    this->modo = "GL_SMOOTH";
  }
  else
  {
    this->modo = "GL_FLAT";
  }
}

void Mago::animacionAtacar()
{
  if (this->subir)
  {
    if (this->tiempo == 0.0)
    {
      anguloBrazo += 0.5f; 
    }
    if (anguloBrazo >= 40.0f)
    { // Si alcanza el máximo, comienza a girar la muñeca
      this->tiempo += 0.05f; 
      anguloManox = 10.0f * sin(tiempo);
      anguloManoy = 2.5f * sin(2.0f * tiempo);
    }
    if (this->tiempo >= 19.5f && this->tiempo < 19.55f)
    {
        this->setAtaque();
    }

    if (this->tiempo >= 20.0f)
    {
      this->tiempo = 0.0f;
      this->subir = false;
    }
  }
  else
  {
    anguloBrazo -= 0.2f; 

    if (anguloBrazo <= 0.0f)
    { // Cuando el brazo vuelve a la posicion inicial, reinicia la animación
      this->tiempo = 0.0f;
      this->subir = true;

    }
  }
  
}

int Mago::getCamino(){
    return camino;
  }

int Mago::getVida(){
  return vida;
}

void Mago::hit(){
  this->vida = this->vida -1;
}

void Mago::setAtaque(){
  if(!this->ataque){
    this->ataque = true; 
  }else if(this->ataque){
    this->ataque = false;
  }
}

bool Mago::getAtaque(){
  return this->ataque;
}



/* Funciones Tanque*/
Tanque::Tanque(string flatSmooth, vector<Objeto3D *> &todosLosObjetos)
{
  this->modo = flatSmooth;

  this->animacionandar = true;
  this->animacionatacar = false;

  this->cuerpo = malla("ObjetosPly/Tanque_cuerpo.ply", "GL_FLAT", false);;
  this->brazoIzq = malla("ObjetosPly/Tanque_brazoIzq.ply", "GL_FLAT", false);;
  this->brazoDer = malla("ObjetosPly/Tanque_brazoDer.ply", "GL_FLAT", false);;
  this->piernaAbajoIzq = malla("ObjetosPly/Tanque_piernaAbajoIzq.ply", "GL_FLAT", false);
  this->piernaArribaIzq = malla("ObjetosPly/Tanque_piernaArribaIzq.ply", "GL_FLAT", false);
  this->piernaAbajoDer = malla("ObjetosPly/Tanque_piernaAbajoDer.ply", "GL_FLAT", false);
  this->piernaArribaDer = malla("ObjetosPly/Tanque_piernaArribaDer.ply", "GL_FLAT", false);
  this->tipo = "Tanque";
  this->distanciax = 0;
  this->vida = 7;
  this->Ataque = false;
  this->camino = rand() % 3 + 1;
  if(camino == 1){    
    this->x = 1;
    this->y = -4;
  }else if(camino == 2){  
    this->x = -15;
    this->y = -4;
    
  }else if(camino == 3){   
    this->x = 15;
    this->y = -4;
  }

  todosLosObjetos.push_back(this);
}

void Tanque::draw()
{
}

void Tanque::drawAndando()
{
  if (this->modo == "GL_FLAT")
  {
    drawFlat();
  }
  else
  {
    //drawSmooth();
  }
}

void Tanque::drawFlat()
{
  glPushMatrix();

  glRotatef(-90, 1, 0, 0);
  glRotatef(-90, 0, 0, 1);
  glPushMatrix();
  glRotatef(desplazamientoCuerpo, 0, 0, 1);
  cuerpo.drawFlatNotexture();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5, 1.75, 4.9);
  glRotatef(anguloBrazoIzq, 0, 1, 0);
  brazoIzq.drawFlatNotexture();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.35, -1.75, 4.9);
  glRotatef(anguloBrazoDer, 0, 1, 0);
  brazoDer.drawFlatNotexture();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, 2.2);
  glRotatef(anguloPiernaIzqSup, 0, 1, 0);
  piernaArribaIzq.drawFlatNotexture();
  glTranslatef(0, 0, -1);
  glRotatef(anguloPiernaIzqInf, 0, 1, 0);
  piernaAbajoIzq.drawFlatNotexture();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, 2.2);
  glRotatef(anguloPiernaDerSup, 0, 1, 0);
  piernaArribaDer.drawFlatNotexture();
  glTranslatef(0, 0, -1);
  glRotatef(anguloPiernaDerInf, 0, 1, 0);
  piernaAbajoDer.drawFlatNotexture();
  glPopMatrix();

  glPopMatrix();
}

// get del modo de dibujo
string Tanque::getModo()
{
  return this->modo;
}

void Tanque::SetcolorTanque(unsigned char color)
{
  Setcolor(color);
}

void Tanque::setmodoTanque()
{
  if (this->modo == "GL_FLAT")
  {
    this->modo = "GL_SMOOTH";
  }
  else
  {
    this->modo = "GL_FLAT";
  }
}

void Tanque::animacionAndar()
{
  distanciax += 0.1;
  if (avanzar)
  {
    anguloBrazoIzq -= 1.0f; 
    anguloBrazoDer += 1.0f;
    anguloPiernaIzqSup += 1.0f;
    anguloPiernaDerSup -= 1.0f;

    if (anguloPiernaIzqInf < 30)
    {
      anguloPiernaIzqInf += 0.75f;
    }

    desplazamientoCuerpo += 0.15;

    if (anguloPiernaDerInf > 0)
    {
      anguloPiernaDerInf -= 1.0f;
    }
  }
  else
  {
    anguloBrazoIzq += 1.0f;
    anguloBrazoDer -= 1.0f;
    anguloPiernaIzqSup -= 1.0f;
    anguloPiernaDerSup += 1.0f;

    if (anguloPiernaIzqInf > 0)
    {
      anguloPiernaIzqInf -= 1.0f;
    }

    desplazamientoCuerpo -= 0.15;

    if (anguloPiernaDerInf < 30)
    {
      anguloPiernaDerInf += 0.75f;
    }
  }

  // Alternar movimiento 
  if (anguloBrazoIzq > 35.0f || anguloBrazoIzq < -35.0f)
  {
    avanzar = !avanzar;
  }
}

void Tanque::animacionAtacar()
{
  if (anguloBrazoIzq > -180)
  {
    anguloBrazoIzq -= 0.5f;
  }

  if (anguloBrazoDer > -180)
  {
    anguloBrazoDer -= 0.5f;
  }

  if(anguloBrazoIzq <= -179 && anguloBrazoDer <= -179){
    this->setAtaque();
  }
}

bool Tanque::getAndar()
{
  return animacionandar;
}

bool Tanque::getAtacar()
{
  return animacionatacar;
}

void Tanque::setAndar()
{
  animacionatacar = false;
  animacionandar = true;
  desplazamientoCuerpo = 0;
}

void Tanque::setAtacar()
{
  animacionandar = false;
  animacionatacar = true;
}

string Tanque::getTipoTanque()
{
  return getTipo();
}

void Tanque::setTipoTanque()
{
  setTipo("Tanque");
}

void Tanque::CargarTextura(const char *archivo)
{

  unsigned ancho, alto;
  unsigned char *data = LeerArchivoJPEG(archivo, ancho, alto);

  glGenTextures(1, &texturaID);
  glBindTexture(GL_TEXTURE_2D, texturaID);

  // Configuración de parámetros de la textura dados en teoria
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  std::cout << "Textura cargada y aplicada con éxito." << std::endl;
}

GLuint Tanque::getTextura()
{
  return texturaID;
}


float Tanque::getPosicion(){
  return distanciax;
}

int Tanque::getCamino(){
  return camino;
}

int Tanque::getVida(){
  return vida;
}
   
void Tanque::hit(){
  this->vida = this->vida-1;
}

void Tanque::setAtaque(){
  this->Ataque = true;
}

bool Tanque::getAtaque(){
  return this->Ataque;
}
