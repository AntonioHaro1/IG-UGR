/*	Prácticas de Informática Gráfica

  Grupos C y D				Curso 2023-24

  Codigo base para la realización de las practicas de IG

  Estudiante:

=======================================================
  G. Arroyo, J.C. Torres
  Dpto. Lenguajes y Sistemas Informticos
  (Univ. de Granada)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details
 http://www.gnu.org/copyleft/gpl.html

=======================================================/
modulo modelo.c
    Representación del modelo
    Funciones de dibujo
    Función Idle

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
#include "malla.h"
#include "enemigos.h"
#include "visual.h"
#include <mutex>
#include "entradaTeclado.h"

/**	void initModel()

Inicializa el modelo y de las variables globales


**/

int modo = GL_FILL;
int luz = 1;
float rotx = 0, roty = 0, rotz = 0;
float Iz = 0;
float De = 0;
float de = 0;
boolean disparoIzq = false;
boolean disparoDer = false;
int contadorDer = 0;
int rotacionDer = 0;
int contadorIzq = 0;
int rotacionIzq = 0;
int VidaCasa = 10;
int colorId = 1;
int contador = 0;
int magosDerrotados,tanqueDerrotados;

void initModel()
{
}

vector<Objeto3D *> todosLosObjetos;

class Esfera : public Objeto3D
{
public:
  float radio; // Radio
  GLuint texId;
  float offsetX = 0.0f, offsetY = 0.0f;
  float velocidad = 0.7001f;
  float dirX;
  float dirZ;
  float roty;

  // para distinguir mis balas a la magia del mago
  // asi mis balas no hacen daño a la casa etc
  int tipo;

  Esfera(float radioEsfera)
  {
    this->radio = radioEsfera;
    this->texId = 0;
  }

  Esfera(float radioEsfera, float x, float z, float rot, int tipo)
  {
    this->radio = radioEsfera;
    this->texId = 0;
    this->dirX = x;
    this->dirZ = z;
    this->roty = rot;
    this->tipo = tipo;

    todosLosObjetos.push_back(this);
  }

  void avanza()
  {
    this->dirX += cos(this->roty);
    this->dirZ += sin(this->roty);
  }

  float getX()
  {
    return this->dirX;
  }

  float getZ()
  {
    return this->dirZ;
  }

  void CargarTextura(const char *archivo)
  {
    if (texId != 0)
    {
      return;
    }

    unsigned ancho, alto;
    unsigned char *data = LeerArchivoJPEG(archivo, ancho, alto);

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    // Configurar parámetros de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    std::cout << "Textura cargada y aplicada con éxito." << std::endl;
  }

  void draw()
  {

    offsetX += 0.0001f;
    offsetY += 0.0001f / 2;
    if (offsetX > 1.0f)
      offsetX -= 1.0f;
    if (offsetY > 1.0f)
      offsetY -= 1.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    GLUquadric *quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(offsetX, offsetY, 0.0f); // Desplazamos la textura gracias a offset modificandose cada vez que entra en draw
    glMatrixMode(GL_MODELVIEW);

    gluSphere(quadric, radio, 50, 50);

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    gluDeleteQuadric(quadric);
    glDisable(GL_TEXTURE_2D);
  }

  void draw2()
  {

    offsetX += velocidad;
    offsetY += velocidad / 2;
    if (offsetX > 1.0f)
      offsetX -= 1.0f;
    if (offsetY > 1.0f)
      offsetY -= 1.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    GLUquadric *quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(offsetX, offsetY, 0.0f);
    glMatrixMode(GL_MODELVIEW);

    gluSphere(quadric, radio, 50, 50);

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    gluDeleteQuadric(quadric);
    glDisable(GL_TEXTURE_2D);
  }

  int gettipo()
  {
    return this->tipo;
  }
};

class plano : public Objeto3D
{
public:
  float lado;
  GLuint texId;
  float offsetX = 0.0f;
  float offsetY = 0.0f;
  float velocidad = 1.0f;

  plano(float ladoDado)
  {
    this->lado = ladoDado;
    texId = 0;
  }

  void CargarTextura(const char *archivo)
  {

    if (texId != 0)
    {
      return;
    }
    unsigned ancho, alto;
    unsigned char *data = LeerArchivoJPEG(archivo, ancho, alto);

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    // Configuración de parámetros de la textura dados en teoria
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    std::cout << "Textura cargada y aplicada con éxito." << std::endl;
  }

  void draw()
  {
    // Actualizar desplazamiento
    offsetX += velocidad * 0.01f;
    offsetY += velocidad * 0.01f;

    // Limitar desplazamiento
    if (offsetX > 1.0f)
      offsetX -= 1.0f;
    if (offsetY > 1.0f)
      offsetY -= 1.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);

    int repeticiones = 5;
    // Número de repeticiones de la textura con esto
    // hacemos que objetos grandes puedan tener alguna textura mas acorde

    glTexCoord2f(repeticiones + offsetX, repeticiones + offsetY);
    glVertex3f(lado / 2, 0.0, lado / 2);
    glTexCoord2f(repeticiones + offsetX, offsetY);
    glVertex3f(lado / 2, 0.0, -lado / 2);
    glTexCoord2f(offsetX, offsetY);
    glVertex3f(-lado / 2, 0.0, -lado / 2);
    glTexCoord2f(offsetX, repeticiones + offsetY);
    glVertex3f(-lado / 2, 0.0, lado / 2);

    glEnd();

    glDisable(GL_TEXTURE_2D);
  }
};

malla casa("ObjetosPly/Casa.ply", "GL_FLAT", true);
malla muro("ObjetosPly/muromadera.ply", "GL_FLAT", true);
malla city1("ObjetosPly/lowpoly_city.ply", "GL_FLAT", true);
Esfera Cielo(800);
malla camino("ObjetosPly/Camino.ply", "GL_FLAT", true);
malla pistolahielo("ObjetosPly/pistolasimple.ply", "GL_FLAT", true);
malla raygun("ObjetosPly/raygun.ply", "GL_FLAT", true);
plano Portal(150.0f);



void GameOver(){

  cout << endl <<"!El juego ha terminado¡" << endl;
  cout << "Tanques derrotados: " << tanqueDerrotados << endl;
  cout << "Magos derrotados: " << magosDerrotados << endl;
  exit(0);
}

void sethit(int Daño)
{
  VidaCasa -= Daño;
  cout << "Vida de la casa ha disminuido a: " << VidaCasa << endl;
  if(VidaCasa <= 0){
    GameOver();
  }
}

void ResetOpenGLState()
{
  // Restablecer materiales
  GLfloat defaultAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat defaultDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLfloat defaultSpecular[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat defaultEmission[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat defaultShininess = 0.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultAmbient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultDiffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpecular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, defaultShininess);

  // Restablecer color actual
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ConfigurarLuz()
{

  // Activar la iluminación en OpenGL
  glEnable(GL_LIGHTING);

  // Definir las propiedades de la luz (simulando luz solar)
  GLfloat luzPos[] = {0.0f, 1.0f, 0.0f, 0.0f};       // Dirección de la luz (luz direccional)
  GLfloat luzDifusa[] = {1.0f, 1.0f, 1.0f, 1.0f};    // Color de la luz (blanco)
  GLfloat luzEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Color especular (blanco)
  GLfloat luzAmbiente[] = {0.8f, 0.8f, 0.8f, 1.0f};  // Luz ambiental (para dar un toque suave)

  // Configurar la luz 0
  glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);     // Color de la luz difusa
  glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular); // Color especular
  glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);   // Luz ambiental

  // Configurar la atenuación (no es estrictamente necesario para un sol, pero puede mejorar el efecto)
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);  // No atenuación
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);    // No atenuación
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f); // No atenuación

  // La luz direccional está configurada, pero aún necesitas habilitar la luz global
  glEnable(GL_LIGHT0);
}

// FUNCIONES PARA EL ESCENARIO
void DibujarMuros()
{
  glPushMatrix();
  if (casa.getText() == 0)
  {
    casa.CargarTextura("Texturas/Material_2_baseColor.jpeg");
  }
  glTranslatef(0, -4, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(5, 5, 5);
  casa.drawFlat(casa.getText());
  glPopMatrix();

  glPushMatrix();
  glPushMatrix();
  if (muro.getText() == 0)
  {
    muro.CargarTextura("Texturas/Diffuse.jpg");
  }
  glTranslatef(24, 1, 12);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(24, 1, 36);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(24, 1, 60);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(24, 1, 84);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(24, 1, 108);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  // muro izq
  glPushMatrix();
  glTranslatef(-24, 1, 12);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-24, 1, 36);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-24, 1, 60);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-24, 1, 84);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-24, 1, 108);
  glRotatef(-90, 1, 0, 0);
  glScalef(1.5, 1.5, 1);
  muro.drawFlat(muro.getText());
  glPopMatrix();
  glPopMatrix();
}

void DibujarEdificios()
{
  glPushMatrix();
  if (city1.getText() == 0)
  {
    city1.CargarTextura("Texturas/material_baseColor.jpeg");
  }
  glTranslatef(400, -4, 250);
  glRotatef(-90, 1, 0, 0);
  glScalef(250, 350, 250);
  glRotatef(-200, 0, 0, 1);
  glDisable(GL_CULL_FACE);
  city1.drawFlat(city1.getText());
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-500, -4, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(250, 350, 250);
  glRotatef(-260, 0, 0, 1);
  glDisable(GL_CULL_FACE);
  city1.drawFlat(city1.getText());
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-500, -4, -700);
  glRotatef(-90, 1, 0, 0);
  glScalef(250, 350, 250);
  glRotatef(-80, 0, 0, 1);
  glDisable(GL_CULL_FACE);
  city1.drawFlat(city1.getText());
  glPopMatrix();
}

void DibujarCielo()
{
  glPushMatrix();
  if (Cielo.texId == 0)
  {
    Cielo.CargarTextura("Texturas/luchtzee.jpg");
  }

  glRotatef(180, 1, 0, 1);
  Cielo.draw();
  glPopMatrix();
}

void DibujarCaminos()
{
  glPushMatrix();
  if (camino.getText() == 0)
  {
    camino.CargarTextura("Texturas/Rock_Floor_Base_baseColor.jpeg");
  }
  glTranslatef(-15, -3.45, 60);
  glRotatef(90, 0, 1, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(0.2, 0.075, 0.1);
  camino.drawFlat(camino.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0, -3.45, 60);
  glRotatef(90, 0, 1, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(0.2, 0.075, 0.1);
  camino.drawFlat(camino.getText());
  glPopMatrix();
  glPushMatrix();
  glTranslatef(15, -3.45, 60);
  glRotatef(90, 0, 1, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(0.2, 0.075, 0.1);
  camino.drawFlat(camino.getText());
  glPopMatrix();
}

void DibujarPortal(){
  glPushMatrix();
  //dibujar portal
  if (Portal.texId == 0)
  {
    Portal.CargarTextura("Texturas/Material_emissive.jpeg");
  }
  glTranslatef(0, 10, 120);
  glRotatef(-90, 1, 0, 0);
  glScalef(0.35, 0.2, 0.5);
  Portal.draw();

  glPopMatrix();
}

void ColorSeleccion(int i)
{
  unsigned char r = (i & 0xFF);
  glColor3ub(r, 0, 0);
}

// FUNCIONES HUD
void dibujarCursor(int screenWidth, int screenHeight)
{

  glPushMatrix();

  // Coloca el cursor en el centro
  glTranslatef(screenWidth / 2, screenHeight / 2, 0.0f);

  // Dibuja la cruz
  glLineWidth(2);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);

  glVertex2f(0.0f, -10.0f);
  glVertex2f(0.0f, 10.0f);

  glVertex2f(-10.0f, 0.0f);
  glVertex2f(10.0f, 0.0f);
  glEnd();

  glPopMatrix();
}

void dibujarVida(int width, int height, int vida)
{
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  ResetOpenGLState();

  glColor3f(1.0f, 0.0f, 0.0f); 

  float anchobarra = 50.0f;
  float altobarra = 30.0f;

  // Dibujar la barra de vida restantes
  for (int i = 0; i <= vida; i++)
  {
    if (i < vida)
    {
      glBegin(GL_QUADS);
      glVertex2f(i * 10 + 10 + anchobarra * i, height + altobarra);
      glVertex2f(i * 10 + 10 + anchobarra * i, height);
      glVertex2f(i * 10 + 10 + anchobarra * (i + 1), height);
      glVertex2f(i * 10 + 10 + anchobarra * (i + 1), height + altobarra);
      glEnd();
    }
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
}

void setupPerspectiveView(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)width / (double)height, 1.0, 1000.0); // Vista en perspectiva para 3D
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void setupOrthoView(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f); // Proyección ortográfica 2D
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// FUNCIONES PARA EL ARMA
void setDisparo()
{
  if (!disparoDer)
  {
    int screenWidth = 1200;  
    int screenHeight = 1000;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // función pick en el centro de la pantalla
    int i = -1;

    pick(centerX, centerY, &i);

    int k = pick(centerX, centerY, &i);
    printf("id: %d\n", k);

    if (k)
    {
      HacerSeleccion(k);
    }

    disparoDer = true;
  }
}

void AnimacionDisparo()
{
  if (contadorDer < 30)
  {
    rotacionDer += 2.66; 
  }

  else if (contadorDer > 470 && contadorDer < 550)
  {
    rotacionDer -= 0.5; 
  }

  contadorDer++;
  //Si termina la animacion el disparo ha terminado
  if (contadorDer == 550)
  {
    disparoDer = false;
    contadorDer = 0;
    rotacionDer = 0;
  }

  glRotatef(rotacionDer, 1, 0, 0);
}

void setDisparo2()
{
  if (!disparoIzq)
  {
    float ay;
    float ejex;
    float ejez;
    get_angulo_xz(ay, ejex, ejez);
    ay = normalizarAngulo(ay);

    float rotyRadianes = (ay - 90) * M_PI / 180.0;
    Esfera *bala = new Esfera(0.1, ejez, ejex, rotyRadianes, 0);
    bala->CargarTextura("Texturas/preview.jpg");
    disparoIzq = true;
  }
}

void AnimacionDisparo2()
{
  if (contadorIzq < 10)
  {
    rotacionIzq += 8;
  }

  else if (contadorIzq > 10 && contadorIzq < 30)
  {
    rotacionIzq -= 4;
  }

  contadorIzq++;
  //Si termina la animacion el disparo ha terminado
  if (contadorIzq == 30)
  {
    disparoIzq = false;
    contadorIzq = 0;
    rotacionIzq = 0;
  }

  glRotatef(rotacionIzq, -1, 0, 0);
}

void DibujarArmas()
{
  getCamara(rotx, roty, rotz, de, Iz, De);

  if (pistolahielo.getText() == 0)
  {
    pistolahielo.CargarTextura("Texturas/Ice_Covered_Albedo.jpeg");
  }

  // Posiciona la pistola en la mano derecha
  glPushMatrix();
  glTranslatef(De, -0.5, Iz);
  glRotatef(-roty, 0.0, 1.0, 0.0);
  glTranslatef(1, 0, -2.3);
  glRotatef(-90, 1, 0, 0);
  if (disparoDer)
  {
    AnimacionDisparo();
  }
  pistolahielo.drawFlat(pistolahielo.getText());
  glPopMatrix();

  glPushMatrix();
  // Material rojo brillante
  GLfloat materialDifuso[] = {0.8f, 0.0f, 0.0f, 1.0f};    
  GLfloat materialEspecular[] = {1.0f, 0.2f, 0.2f, 1.0f}; 
  GLfloat materialAmbiente[] = {0.3f, 0.0f, 0.0f, 1.0f};

  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);

  GLfloat brillo[] = {100.0f};
  glMaterialfv(GL_FRONT, GL_SHININESS, brillo);

  glTranslatef(De, -0.5, Iz);
  glRotatef(-roty, 0.0, 1.0, 0.0);
  glTranslatef(-1, 0, -2);
  glRotatef(-90, 1, 0, 0);
  glRotatef(180, 0, 0, 1);
  if (disparoIzq)
  {
    AnimacionDisparo2();
  }
  raygun.drawFlat(raygun.getText());
  ResetOpenGLState();
  glPopMatrix();
}


void DibujaEscena(boolean selec)
{

  static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0}; // Posicion de la fuente de luz

  float color[4] = {0.8, 0.0, 1, 1};
  float color2[4] = {1.0, 0.0, 0.0, 1};

  glPushMatrix(); // Apila la transformacion geometrica actual

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fondo negro

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Inicializa el buffer de color 

  transformacionVisualizacion(); // Carga transformacion de visualizacion

  glLightfv(GL_LIGHT0, GL_POSITION, pos); // Declaracion de luz. Colocada aqui esta fija en la escena

  ConfigurarLuz();

  // Generar enemigos por probabilidad
  int probabilidad = rand() % 10000 + 1;

  if (probabilidad <= 30 && probabilidad > 20)
  {
    Tanque *tanque = new Tanque("GL_FLAT", todosLosObjetos);
    tanque->Setcolor(colorId);
    colorId++;
  }
  if (probabilidad <= 7)
  {
    Mago *mago = new Mago("GL_FLAT", todosLosObjetos);
    mago->Setcolor(colorId);
    colorId++;
  }

  int indicemuerto = -1;
  int indiceTanqueAtaque = -1;

  // Dibujar Escenario
  if (!selec)
  {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DITHER);
    for (int i = todosLosObjetos.size() - 1; i >= 0; i--)
    {
      if (Tanque *tanque = dynamic_cast<Tanque *>(todosLosObjetos[i]))
      {

        ColorSeleccion(tanque->getId());
        glPushMatrix();
        glRotatef(180, 0, 1, 0);
        if (tanque->getCamino() == 1)
        {

          glTranslatef(1, -4, tanque->getPosicion() - 130);
        }
        else if (tanque->getCamino() == 2)
        {

          glTranslatef(-15, -4, tanque->getPosicion() - 130);
        }
        else if (tanque->getCamino() == 3)
        {

          glTranslatef(15, -4, tanque->getPosicion() - 130);
        }
        glScalef(1.5, 1.5, 1.5);
        tanque->drawFlat();
        glPopMatrix();

        if (tanque->getPosicion() >= 124)
        {
          tanque->setAtacar();
        }

        if (tanque->getAtaque())
        {
          sethit(3);
          indiceTanqueAtaque = i;
        }
        
        if(tanque->getSeleccion()){
          if (tanque->getAndar())
          {
            tanque->animacionAndar();
          }
          else if (tanque->getAtacar())
          {
            tanque->animacionAtacar();
          }
        }
      }

      if (Mago *mago = dynamic_cast<Mago *>(todosLosObjetos[i]))
      { 
        ColorSeleccion(mago->getId());
        glPushMatrix();
        if (mago->getCamino() == 1)
        {
          if (mago->getAtaque())
          {
            Esfera *bala = new Esfera(1, 1, 115, 80.15, 1);
            bala->CargarTextura("Texturas/abstract-fire-desktop-wallpaper-realistic-blazing-flame-image.jpg");
            mago->setAtaque();
          }
          glRotatef(180, 0, 1, 0);
          glTranslatef(1, -4, -115);
        }
        else if (mago->getCamino() == 2)
        {
          if (mago->getAtaque())
          {
            Esfera *bala = new Esfera(1, 15, 115, 80.15, 1);
            bala->CargarTextura("Texturas/abstract-fire-desktop-wallpaper-realistic-blazing-flame-image.jpg");
            mago->setAtaque();
          }
          glRotatef(180, 0, 1, 0);
          glTranslatef(-15, -4, -115);
        }
        else if (mago->getCamino() == 3)
        {
          if (mago->getAtaque())
          {
            Esfera *bala = new Esfera(1, -15, 115, 80.15, 1);
            bala->CargarTextura("Texturas/abstract-fire-desktop-wallpaper-realistic-blazing-flame-image.jpg");
            mago->setAtaque();
          }
          glRotatef(180, 0, 1, 0);
          glTranslatef(15, -4, -115);
        }
        glScalef(2, 3, 2);

        if(mago->getSeleccion()){
          mago->animacionAtacar();
        }
        mago->drawFlat();
        glPopMatrix();
        ResetOpenGLState();
      }
    }
    return;
  }
  else
  {
    glEnable(GL_LIGHTING);
    DibujarMuros();
    DibujarEdificios();
    DibujarCielo();
    DibujarCaminos();

    // Dibujar HUD
    glPushMatrix();
    setupOrthoView(1200, 1000);
    dibujarVida(1200, 1000 - 50, VidaCasa); 
    dibujarCursor(1200, 1000);              
    setupPerspectiveView(1200, 1000);
    glPopMatrix();

    // Dibujar armas
    DibujarArmas();
    DibujarPortal();
  }

  for (int i = todosLosObjetos.size() - 1; i >= 0; i--)
  {

    if (Tanque *tanque = dynamic_cast<Tanque *>(todosLosObjetos[i]))
    { 
      // Material rojo brillante
      GLfloat materialDifuso[] = {1.0f, 1.0f, 1.0f, 1.0f};    
      GLfloat materialEspecular[] = {1.0f, 0.2f, 0.2f, 1.0f}; 
      GLfloat materialAmbiente[] = {0.3f, 0.0f, 0.0f, 1.0f};

      glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
      glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);
      glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);

      GLfloat brillo[] = {100.0f};
      glMaterialfv(GL_FRONT, GL_SHININESS, brillo);

      if (tanque->texturaID == 0)
      { 
        tanque->CargarTextura("Texturas/TexturaTanque.jpeg");
      }
      glPushMatrix();
      glRotatef(180, 0, 1, 0);
      if (tanque->getCamino() == 1)
      {

        glTranslatef(1, -4, tanque->getPosicion() - 130);
      }
      else if (tanque->getCamino() == 2)
      {

        glTranslatef(-15, -4, tanque->getPosicion() - 130);
      }
      else if (tanque->getCamino() == 3)
      {

        glTranslatef(15, -4, tanque->getPosicion() - 130);
      }
      glScalef(1.5, 1.5, 1.5);
      tanque->drawFlat();
      glPopMatrix();

      if (tanque->getPosicion() >= 124)
      {
        tanque->setAtacar();
      }

      if (tanque->getAtaque())
      {
        sethit(3);
        indiceTanqueAtaque = i;
      }

        if(tanque->getSeleccion()){
          if (tanque->getAndar())
          {
            tanque->animacionAndar();
          }
          else if (tanque->getAtacar())
          {
            tanque->animacionAtacar();
          }
        }
    }

    if (Mago *mago = dynamic_cast<Mago *>(todosLosObjetos[i]))
    { 
      // Material verde esmeralda brillante
      GLfloat materialDifuso[] = {0.0f, 0.8f, 0.5f, 1.0f};    
      GLfloat materialEspecular[] = {0.1f, 1.0f, 0.7f, 1.0f}; 
      GLfloat materialAmbiente[] = {0.0f, 0.3f, 0.2f, 1.0f};

      glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
      glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);
      glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);

      GLfloat brillo[] = {100.0f}; // Ajusta para mayor o menor brillo
      glMaterialfv(GL_FRONT, GL_SHININESS, brillo);

      glPushMatrix();
      if (mago->getCamino() == 1)
      {
        if (mago->getAtaque())
        {
          Esfera *bala = new Esfera(1, 1, 115, 80.15, 1);
          bala->CargarTextura("Texturas/abstract-fire-desktop-wallpaper-realistic-blazing-flame-image.jpg");
          mago->setAtaque();
        }
        glRotatef(180, 0, 1, 0);
        glTranslatef(1, -4, -115);
      }
      else if (mago->getCamino() == 2)
      {
        if (mago->getAtaque())
        {
          Esfera *bala = new Esfera(1, 15, 115, 80.15, 1);
          bala->CargarTextura("Texturas/abstract-fire-desktop-wallpaper-realistic-blazing-flame-image.jpg");
          mago->setAtaque();
        }
        glRotatef(180, 0, 1, 0);
        glTranslatef(-15, -4, -115);
      }
      else if (mago->getCamino() == 3)
      {
        if (mago->getAtaque())
        {
          Esfera *bala = new Esfera(1, -15, 115, 80.15, 1);
          bala->CargarTextura("Texturas/abstract-fire-desktop-wallpaper-realistic-blazing-flame-image.jpg");
          mago->setAtaque();
        }
        glRotatef(180, 0, 1, 0);
        glTranslatef(15, -4, -115);
      }
      glScalef(2, 3, 2);

      if(mago->getSeleccion()){
        mago->animacionAtacar();
      }

      mago->drawFlat();
      glPopMatrix();
      ResetOpenGLState();
    }

    if (Esfera *esfera = dynamic_cast<Esfera *>(todosLosObjetos[i]))
    {
      ResetOpenGLState();
      esfera->avanza();
      glPushMatrix();
      glTranslatef(esfera->getX(), -0.3, esfera->getZ());

      for (int j = 0; j < todosLosObjetos.size(); j++)
      {
        if (i == j)
          continue;
        if (Tanque *tanque = dynamic_cast<Tanque *>(todosLosObjetos[j]))
        {
          if (tanque->getCamino() == 1)
          {
            if ((esfera->getX() < 4 && esfera->getX() > -6) && abs(esfera->getZ() - 130 + tanque->getPosicion()) < 1 && esfera->gettipo() == 0)
            {
              std::cout << "Esfera colisionó con un tanque!" << std::endl;
              tanque->hit();
              if (tanque->getVida() == 0)
              {
                // Guardamos el indice del tanque muerto
                // para despues (evita core)
                indicemuerto = j;
                tanqueDerrotados++;
              }
              // Eliminar la esfera
              delete esfera;
              todosLosObjetos.erase(todosLosObjetos.begin() + i);
              i--;
              break;
            }
          }
          else if (tanque->getCamino() == 2)
          {
            if ((esfera->getX() < 20 && esfera->getX() > 10) && abs(esfera->getZ() - 130 + tanque->getPosicion()) < 1 && esfera->gettipo() == 0)
            {
              std::cout << "Esfera colisionó con un tanque!" << std::endl;
              tanque->hit();
              if (tanque->getVida() == 0)
              {
                indicemuerto = j;
                tanqueDerrotados++;
              }
              // Eliminar la esfera
              delete esfera;
              todosLosObjetos.erase(todosLosObjetos.begin() + i);
              i--;
              break;
            }
          }
          else if (tanque->getCamino() == 3)
          {
            if ((esfera->getX() < -10 && esfera->getX() > -20) && abs(esfera->getZ() - 130 + tanque->getPosicion()) < 1 && esfera->gettipo() == 0)
            {
              std::cout << "Esfera colisionó con un tanque!" << std::endl;
              tanque->hit();
              if (tanque->getVida() == 0)
              {
                indicemuerto = j;
                tanqueDerrotados++;
              }
              // Eliminar la esfera
              delete esfera;
              todosLosObjetos.erase(todosLosObjetos.begin() + i);
              i--;
              break;
            }
          }
        }

        if (Mago *mago = dynamic_cast<Mago *>(todosLosObjetos[j]))
        {
          if (mago->getCamino() == 1)
          {
            if ((esfera->getX() < 0 && esfera->getX() > -2.7) && (esfera->getZ() > 114 && esfera->getZ() < 117) && (esfera->gettipo() == 0))
            {
              std::cout << "Esfera colisionó con un mago!" << std::endl;
              mago->hit();
              if (mago->getVida() == 0)
              {
                // igual que el tanque
                indicemuerto = j;
                magosDerrotados++;
              }
              // Eliminar la esfera
              delete esfera;
              todosLosObjetos.erase(todosLosObjetos.begin() + i);
              i--;
              break; 
            }
          }
          else if (mago->getCamino() == 2)
          {
            if ((esfera->getX() > 13 && esfera->getX() < 15.7) && (esfera->getZ() > 114 && esfera->getZ() < 117) && (esfera->gettipo() == 0))
            {
              std::cout << "Esfera colisionó con un mago!" << std::endl;
              mago->hit();
              if (mago->getVida() == 0)
              {
                indicemuerto = j;
                magosDerrotados++;
              }
              // Eliminar la esfera
              delete esfera;
              todosLosObjetos.erase(todosLosObjetos.begin() + i);
              i--;
              break;
            }
          }
          else if (mago->getCamino() == 3)
          {
            if ((esfera->getX() < -14 && esfera->getX() > -16.7) && (esfera->getZ() > 114 && esfera->getZ() < 117) && (esfera->gettipo() == 0))
            {
              std::cout << "Esfera colisionó con un mago!" << std::endl;
              mago->hit();
              if (mago->getVida() == 0)
              {
                indicemuerto = j;
                magosDerrotados++;
              }
              // Eliminar la esfera
              delete esfera;
              todosLosObjetos.erase(todosLosObjetos.begin() + i);
              i--;
              break;
            }
          }
        }
      }

      esfera->draw2();
      glPopMatrix();
      if (esfera->getX() >= 150 || esfera->getZ() >= 150)
      {
        delete esfera;
        todosLosObjetos.erase(todosLosObjetos.begin() + i);
      }
      if (esfera->getZ() <= 0 && esfera->gettipo() == 1)
      {
        delete esfera;
        todosLosObjetos.erase(todosLosObjetos.begin() + i);
        i--;
        sethit(1);
      }
    }
    ResetOpenGLState();
  }

// eliminamos los indices guardados si es que hay
  if (indicemuerto != -1)
  {
    todosLosObjetos.erase(todosLosObjetos.begin() + indicemuerto);
  }
  if (indiceTanqueAtaque != -1)
  {
    cout << "¡Ataque a la casa realizado!" << endl;
    todosLosObjetos.erase(todosLosObjetos.begin() + indiceTanqueAtaque);
  }
  contador++;
  if(contador == 500){
    for(int i = todosLosObjetos.size() - 1; i >= 0; i--){
      if(!todosLosObjetos[i]->getSeleccion()){
        todosLosObjetos[i]->setSeleccion();
      }
    }
  }
  glPopMatrix();
}


int pick(int x, int y, int *i)
{

  GLint viewport[4];
  unsigned char data[4];
  int f = 0;

  glGetIntegerv(GL_VIEWPORT, viewport);
  glDisable(GL_DITHER);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  DibujaEscena(false);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_DITHER);

  glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
  if (data[0] > 0)
  {
    *i = data[0];
  }
  else
  {
    *i = -1;
  }

  return *i;
}

void HacerSeleccion(int k)
{

  for (size_t i = 0; i < todosLosObjetos.size(); i++)
  {
    if (k == todosLosObjetos[i]->getId())
    {
      cout << "Objeto marcardo: " << k << endl
           << flush;
      todosLosObjetos[i]->setSeleccion(); // Marca el objeto como seleccionado
      contador = 0;
    }
  }
}

void Dibuja()
{

  DibujaEscena(true);
  glutSwapBuffers();
}

/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle(int v)
{

  glutPostRedisplay();        // Redibuja
  glutTimerFunc(30, idle, 0); // Vuelve a activarse dentro de 30 ms
}
