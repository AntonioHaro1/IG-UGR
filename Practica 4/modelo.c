/*	Prácticas de Informática Gráfica

	Grupos C y D				Curso 2023-24
 	
	Codigo base para la realización de las practicas de IG
	
	Estudiante: Antonio Haro Arriaza

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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "file_ply_stl.h"
#include <vector>
#include <string>
using namespace std;
#include "practicasIG.h"
#include  "lector-jpg.h"


/**	void initModel()

Inicializa el modelo y de las variables globales


**/
void
initModel ()
{

}


//Practica 1

int modo = GL_FILL;
int luz = 1;

void setModo(int M){
  modo = M;
}

void setLuz(){
  if(luz == 1){
    glDisable(GL_LIGHTING);
    luz = 0;
  }else{
    glEnable(GL_LIGHTING);
    luz = 1;
  }
}

class Ejes:Objeto3D 
{ 
public: 
    float longitud = 30;
// Dibuja el objeto
void draw( )
{
  glBegin (GL_LINES);
  {
    glColor3f (0, 1, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (0, longitud, 0);

    glColor3f (1, 0, 0);
    glVertex3f (0, 0, 0);
    glVertex3f (longitud, 0, 0);

    glColor3f (0, 0, 1);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, longitud);
  }
  glEnd ();
  

}
} ; 

Ejes ejesCoordenadas;

class Cubo:Objeto3D{
  public:
    float lado;

    Cubo(float lado){
      this->lado = lado;
    }

  void draw(){
    float MitadLado = lado / 2.0f;

    glBegin(GL_QUADS);

    // Cara frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-MitadLado, 0.0f, MitadLado);     
    glVertex3f( MitadLado, 0.0f, MitadLado);    
    glVertex3f( MitadLado, lado, MitadLado);    
    glVertex3f(-MitadLado, lado, MitadLado);     

    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-MitadLado, 0.0f, -MitadLado);    
    glVertex3f(-MitadLado, lado, -MitadLado);    
    glVertex3f( MitadLado, lado, -MitadLado);    
    glVertex3f( MitadLado, 0.0f, -MitadLado);    

    // Cara izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-MitadLado, 0.0f, -MitadLado);    
    glVertex3f(-MitadLado, 0.0f, MitadLado);     
    glVertex3f(-MitadLado, lado, MitadLado);     
    glVertex3f(-MitadLado, lado, -MitadLado);   

    // Cara derecha
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(MitadLado, 0.0f, -MitadLado);    
    glVertex3f(MitadLado, lado, -MitadLado);     
    glVertex3f(MitadLado, lado, MitadLado);      
    glVertex3f(MitadLado, 0.0f, MitadLado);     

    // Cara superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-MitadLado, lado, -MitadLado);    
    glVertex3f(MitadLado, lado, -MitadLado);    
    glVertex3f(MitadLado, lado, MitadLado);      
    glVertex3f(-MitadLado, lado, MitadLado);     

    // Cara inferior (base)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-MitadLado, 0.0f, -MitadLado);    
    glVertex3f(-MitadLado, 0.0f, MitadLado);     
    glVertex3f(MitadLado, 0.0f, MitadLado);      
    glVertex3f(MitadLado, 0.0f, -MitadLado);    

    glEnd();
}

};

class Piramide:Objeto3D{
  public:
    float lado;
    float altura;

    Piramide(float lado, float altura){
      this->lado = lado;
      this->altura = altura;
    }

      void draw() {
        float MitadLado = lado / 2.0f;  

        glBegin(GL_TRIANGLES);


        // Cara frontal
        glNormal3f(0.0f, 0.5f, 0.5f); 
        glVertex3f(0.0f, altura, 0.0f);         
        glVertex3f(-MitadLado, 0.0f, MitadLado);    
        glVertex3f(MitadLado, 0.0f, MitadLado);   

        // Cara derecha
        glNormal3f(0.5f, 0.5f, 0.0f); 
        glVertex3f(0.0f, altura, 0.0f);          
        glVertex3f(MitadLado, 0.0f, MitadLado);   
        glVertex3f(MitadLado, 0.0f, -MitadLado);   

        // Cara trasera
        glNormal3f(0.0f, 0.5f, -0.5f);
        glVertex3f(0.0f, altura, 0.0f);           
        glVertex3f(MitadLado, 0.0f, -MitadLado);    
        glVertex3f(-MitadLado, 0.0f, -MitadLado);   

        // Cara izquierda
        glNormal3f(-0.5f, 0.5f, 0.0f);
        glVertex3f(0.0f, altura, 0.0f);          
        glVertex3f(-MitadLado, 0.0f, -MitadLado);  
        glVertex3f(-MitadLado, 0.0f, MitadLado);   

        glEnd();

        // Dibuja la base de la pirámide (cuadrado)
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f); 

        glVertex3f(-MitadLado, 0.0f, -MitadLado);   
        glVertex3f(MitadLado, 0.0f, -MitadLado);   
        glVertex3f(MitadLado, 0.0f, MitadLado);    
        glVertex3f(-MitadLado, 0.0f, MitadLado);    

        glEnd();
    }
};

Cubo Cubo(5);
Piramide Piramide(5,5);

////////////////////////////////
//Practica 2 

class malla:Objeto3D{
  public:
    vector<float> vertices_ply;
    vector<int> caras_ply;
    vector<float> normales_caras;
    vector<float> normales_vertices;
    string modo;

    malla(){}

    malla(std::string nombrearchivo, string flatSmooth) { 
      ply::read(nombrearchivo.c_str(), vertices_ply, caras_ply);
      this->modo = flatSmooth;
    }

    void draw(){

    } 

    void drawFlat(){
      
      glShadeModel(GL_FLAT);
      
      glBegin(GL_TRIANGLES);

      calcular_normales();

      for (size_t i = 0; i < caras_ply.size(); i += 3) {
        int indice1 = caras_ply[i];
        int indice2 = caras_ply[i + 1];
        int indice3 = caras_ply[i + 2]; 

        // Obtener la normal guardada
        float x = normales_caras[i];
        float y = normales_caras[i + 1];
        float z = normales_caras[i + 2];

        // Aplicar la normal y los vértices  
        //glNormal3f(x, y, z); 
        glVertex3f(vertices_ply[indice1 *3],vertices_ply[indice1 *3 + 1], vertices_ply[indice1 *3 + 2]);
        glVertex3f(vertices_ply[indice2 *3],vertices_ply[indice2 *3 + 1], vertices_ply[indice2 *3 +2]);
        glNormal3f(x, y, z); 
        glVertex3f(vertices_ply[indice3 *3],vertices_ply[indice3 *3 + 1], vertices_ply[indice3 *3 +2]);
      
      }
      glEnd();
    }

    void drawSmooth(){
      
      glShadeModel(GL_SMOOTH);
      
      glBegin(GL_TRIANGLES);

      calcular_normales();


      for (size_t i = 0; i < caras_ply.size(); i += 3) {
        int indice1 = caras_ply[i];
        int indice2 = caras_ply[i + 1];
        int indice3 = caras_ply[i + 2]; 

        // aplicar la normal de vertices antes de cada vertice
        glNormal3f(normales_vertices[indice1 * 3], normales_vertices[indice1 * 3 + 1], normales_vertices[indice1 * 3 + 2]);
        glVertex3f(vertices_ply[indice1 * 3], vertices_ply[indice1 * 3 + 1], vertices_ply[indice1 * 3 + 2]);

        glNormal3f(normales_vertices[indice2 * 3], normales_vertices[indice2 * 3 + 1], normales_vertices[indice2 * 3 + 2]);
        glVertex3f(vertices_ply[indice2 * 3], vertices_ply[indice2 * 3 + 1], vertices_ply[indice2 * 3 + 2]);

        glNormal3f(normales_vertices[indice3 * 3], normales_vertices[indice3 * 3 + 1], normales_vertices[indice3 * 3 + 2]);
        glVertex3f(vertices_ply[indice3 * 3], vertices_ply[indice3 * 3 + 1], vertices_ply[indice3 * 3 + 2]);
      }

      glEnd();
    }



    void calcular_normales(){

      size_t numVertices = vertices_ply.size() / 3;
      size_t numCaras = caras_ply.size() / 3;
        
      // Inicializamos las normales a 0
      normales_vertices.resize(vertices_ply.size(), 0.0f);
      normales_caras.resize(numCaras * 3, 0.0f);


      //calculo de normales de las caras
      for (size_t i = 0; i < caras_ply.size(); i += 3) {
        int indice1 = caras_ply[i];
        int indice2 = caras_ply[i + 1];
        int indice3 = caras_ply[i + 2]; 

        float P0[3] = {vertices_ply[indice1 * 3], vertices_ply[indice1 * 3 + 1], vertices_ply[indice1 * 3 + 2]};
        float P1[3] = {vertices_ply[indice2 * 3], vertices_ply[indice2 * 3 + 1], vertices_ply[indice2 * 3 + 2]};
        float P2[3] = {vertices_ply[indice3 * 3], vertices_ply[indice3 * 3 + 1], vertices_ply[indice3 * 3 + 2]};

        float vector1[3] = { P1[0] - P0[0], P1[1] - P0[1], P1[2] - P0[2] };
        float vector2[3] = { P2[0] - P0[0], P2[1] - P0[1], P2[2] - P0[2] };

        // Producto vectorial para calcular la normal
        float normal[3] = {
          vector1[1] * vector2[2] - vector1[2] * vector2[1],
          vector1[2] * vector2[0] - vector1[0] * vector2[2],
          vector1[0] * vector2[1] - vector1[1] * vector2[0]
        };

        // Normalización del vector normal /// modulo del vector 
        float modulo = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
        if (modulo > 0.0f) {  // Para evitar división por cero
          normal[0] /= modulo;
          normal[1] /= modulo;
          normal[2] /= modulo;
        }
          // Guardar la normal de la cara
          size_t cara_index = i / 3;
          normales_caras[i] = normal[0];
          normales_caras[i + 1] = normal[1];
          normales_caras[i + 2] = normal[2];


        // Sumar la normal de la cara a los tres vértices correspondientes
        // asi al final tener la suma de las normales de las caras que pertenece
          for (int j = 0; j < 3; j++) {
            normales_vertices[indice1 * 3 + j] += normal[j];
            normales_vertices[indice2 * 3 + j] += normal[j];
            normales_vertices[indice3 * 3 + j] += normal[j];
          }

      }
    
      // Normalización de los vértices
      for (size_t i = 0; i < numVertices; ++i) {
        float x = normales_vertices[i * 3];
        float y = normales_vertices[i * 3 + 1];
        float z = normales_vertices[i * 3 + 2];

        float modulo = sqrt(x * x + y * y + z * z);

        if (modulo > 0.0f) { // Para evitar división por cero
          normales_vertices[i * 3] /= modulo;
          normales_vertices[i * 3 + 1] /= modulo;
          normales_vertices[i * 3 + 2] /= modulo;
        }
      }
    }

    string getModo(){
      return this->modo;
    }
    
};

// La malla 1 tiene smooth y malla 2 flat
malla malla1("beethoven.ply", "GL_FLAT");
malla malla2("beethoven.ply", "GL_SMOOTH");


void setModoMalla1() {
  if (malla1.modo == "GL_FLAT") {
    malla1.modo = "GL_SMOOTH";
  } else {
    malla1.modo = "GL_FLAT";
  }
}

void setModoMalla2() {
  if (malla2.modo == "GL_FLAT") {
    malla2.modo = "GL_SMOOTH";
  } else {
    malla2.modo = "GL_FLAT";
  }
}

//Practica 3
malla Cabeza("robot_cabeza.ply", "GL_FLAT");
malla Torso("robot_torso.ply", "GL_FLAT");
malla BrazoAlto("robot_brazosArriba.ply", "GL_FLAT");
malla BrazoAbajo("robot_brazosAbajo.ply", "GL_FLAT");
malla ManoDerecha("robotmano.ply", "GL_FLAT");
malla ManoIzquierda("robotmano.ply", "GL_FLAT");


class Robot:Objeto3D{
  public:
    // valores de giros
    int giroCabeza;
    int giroBrazos;
    int giroBrazosAbajo;
    int giroManos;
    GLboolean HayAnimacion= false;
    string modo;  
    GLboolean positivobrazo= true;
    GLboolean positivobrazoabajo = true;

    //Mallas que componen el robot
    malla cabeza;
    malla torso;
    malla brazoAlto;
    malla brazoAbajo;
    malla manoIzquierda;
    malla manoDerecha;

    // constructor
    Robot(string flatSmooth) { 
      this->modo = flatSmooth;

      giroCabeza = 0;
      giroBrazos = 0;
      giroBrazosAbajo = 0;
      giroManos = 0;

      this->cabeza = Cabeza;
      this->torso = Torso;
      this->brazoAlto = BrazoAlto;
      this->brazoAbajo = BrazoAbajo;
      this->manoDerecha = ManoDerecha;
      this->manoIzquierda = ManoIzquierda;
    }

    void draw(){

    }

    void drawFlat(){

      //Primero dibujamos la cabeza y el torso
      float  color2[4] = { 1.0, 0.0, 0.0, 1 };
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,5.3);

      glPushMatrix();
      // movemos hacia arriba pero como estaba todo rotado debido a blender lo hacemos en la z
      glRotatef(giroCabeza,0,0,1);
      cabeza.drawFlat();
      glPopMatrix();

      torso.drawFlat();


      //Ahora dibujamos los brazos
      glPushMatrix();
      glTranslatef(0,0,-0.6); // lo colocamos a su sitio original
      glRotatef(giroBrazos,1,0,0); // para que gire bien en el 0,0,0
      glTranslatef(0,0,0.6); // con esto lo colocamos en el 0,0
      BrazoAlto.drawFlat();
      glTranslatef(0,0,-1.35); // lo colocamos donde estaba para que se junten otra vez
      glRotatef(giroBrazosAbajo,1,0,0);
      glTranslatef(0,0,1.4); // con esto colocamos a 0,0 el brazo de abajo para que haga el giro correcto
      BrazoAbajo.drawFlat();

      // Dibujo las manos
      glPushMatrix(); // aislo las manos para poder colocarlas bien y giro cada una
      glRotatef(35,0,1,0);
      glRotatef(-18,1,0,0);
      glTranslatef(-0.37,0.6,-3.4); // lo colocamos en su sitio
      glRotatef(giroManos, 0, 0, 1); 
      glTranslatef(0.13, 0, 0); // lo colocamos en el centro
      ManoDerecha.drawFlat();
      glPopMatrix();
      glPushMatrix();
      glRotatef(-35,0,1,0);
      glRotatef(-18,1,0,0);
      glTranslatef(0.37,0.6,-3.4); 
      glRotatef(giroManos, 0, 0, 1); 
      glTranslatef(0.13, 0, 0);
      ManoIzquierda.drawFlat();
      glPopMatrix();

      glPopMatrix();
    }

    void drawSmooth(){
      //Primero dibujamos la cabeza y el torso
      float  color2[4] = { 1.0, 0.0, 0.0, 1 };
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,5.3);

      glPushMatrix();
      // movemos hacia arriba pero como estaba todo rotado lo hacemos en la z
      glRotatef(giroCabeza,0,0,1);
      cabeza.drawSmooth();
      glPopMatrix();

      torso.drawSmooth(); 


      //Ahora dibujamos los brazos
      glPushMatrix();
      glTranslatef(0,0,-0.6); // lo colocamos
      glRotatef(giroBrazos,1,0,0);
      glTranslatef(0,0,0.6); // con esto lo colocamos en el 0,0
      BrazoAlto.drawSmooth();
      glTranslatef(0,0,-1.35); // lo colocamos donde estaba para que se junten otra vez
      glRotatef(giroBrazosAbajo,1,0,0);
      glTranslatef(0,0,1.4); // con esto colocamos a 0,0 el brazo de abajo para que haga el giro correcto
      BrazoAbajo.drawSmooth();

      // Dibujo las manos
      glPushMatrix(); 
      glRotatef(35,0,1,0);
      glRotatef(-18,1,0,0);
      glTranslatef(-0.37,0.6,-3.4);
      glRotatef(giroManos, 0, 0, 1); 
      glTranslatef(0.13, 0, 0);
      ManoDerecha.drawSmooth();
      glPopMatrix();
      glPushMatrix();
      glRotatef(-35,0,1,0);
      glRotatef(-18,1,0,0);
      glTranslatef(0.37,0.6,-3.4); 
      glRotatef(giroManos, 0, 0, 1); 
      glTranslatef(0.13, 0, 0);
      ManoIzquierda.drawSmooth();
      glPopMatrix();


      glPopMatrix();
    }

    // get del modo de dibujo
    string getModo(){
      return this->modo;
    }

};

Robot Robot("GL_FLAT");


void setmodoRobot() {
  if (Robot.modo == "GL_FLAT") {
    Robot.modo = "GL_SMOOTH";
  } else {
    Robot.modo = "GL_FLAT";
  }
}

void setGiroCabeza(){
  Robot.giroCabeza += 10;
}

void setGiroBrazos(int i){ 
  if(i == 1 && Robot.giroBrazos <= 40){
    Robot.giroBrazos += 2.5;
  }else if(i == 0 && Robot.giroBrazos >= -70){
    Robot.giroBrazos -= 2.5;
  }

  if(Robot.giroBrazos >= 40){
    Robot.positivobrazo = true;
  }else if(Robot.giroBrazos <= -70){
    Robot.positivobrazo = false;
  }
}

void setGiroBrazosAbajo(int i){
  if(i == 1 && Robot.giroBrazosAbajo <= 20){
    Robot.giroBrazosAbajo += 2;
  }else if(i == 0 && Robot.giroBrazosAbajo >= -70){
    Robot.giroBrazosAbajo -= 2;
  }

  if(Robot.giroBrazosAbajo >= 20){
    Robot.positivobrazoabajo = true;
  }else if(Robot.giroBrazosAbajo <= -70){
    Robot.positivobrazoabajo = false;
  }
}

void setGiroManos(){
   Robot.giroManos += 10;
}


void setAnimacion(){
  if(Robot.HayAnimacion){
    Robot.HayAnimacion = false;
  }else{
    Robot.HayAnimacion = true;
  }
}

    void Animacion(){
      setGiroCabeza();

      if(Robot.positivobrazo){
      setGiroBrazos(0);
      }else{
        setGiroBrazos(1);
      }

      if(Robot.positivobrazoabajo){
        setGiroBrazosAbajo(0);
      }else{
        setGiroBrazosAbajo(1);
      }
      setGiroManos();         // Gira las manos
    }


// PRACTICA 4
class Dado:public Objeto3D {
public:
    float lado;
    GLuint texId;

    Dado(float ladoDado){
      this->lado = ladoDado;
      texId = 0;
    }


    void CargarTextura(const char* archivo) {

      if(texId != 0){
        return;
      }
        unsigned ancho, alto;
        unsigned char* data = LeerArchivoJPEG(archivo, ancho, alto); 


        // Genera un identificador de textura y la activa
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        // Configuración de parámetros de la textura dados en teoria
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Envia los datos de la imagen a OpenGL y libera la memoria
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


        std::cout << "Textura cargada y aplicada con éxito." << std::endl;
    }

    void draw() {

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId);

        float MitadLado = lado / 2.0f;

        glBegin(GL_TRIANGLES);

        //Cara frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        
        glTexCoord2f(0.5f,0.75f); glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glTexCoord2f(0.75f,0.75f); glVertex3f(MitadLado, 0.0f, MitadLado);    
        glTexCoord2f(0.7f,0.5f); glVertex3f(MitadLado, lado, MitadLado);

        glTexCoord2f(0.5f,0.75f); glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glTexCoord2f(0.75f,0.5f); glVertex3f(MitadLado, lado, MitadLado);    
        glTexCoord2f(0.5f,0.5f); glVertex3f(-MitadLado, lado, MitadLado);     

        // Cara trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        
        glTexCoord2f(0.25,0.75f); glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(0.25f,0.5f); glVertex3f(-MitadLado, lado, -MitadLado);    
        glTexCoord2f(0.0f,0.50f); glVertex3f(MitadLado, lado, -MitadLado);    
        
        glTexCoord2f(0.25,0.75f); glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(0.0f,0.50f); glVertex3f(MitadLado, lado, -MitadLado);    
        glTexCoord2f(0.0f,0.75f); glVertex3f(MitadLado, 0.0f, -MitadLado);    

        // Cara izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(00.25,0.75f); glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(0.5f,0.75f); glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glTexCoord2f(0.5f,0.5f); glVertex3f(-MitadLado, lado, MitadLado);     
  
        glTexCoord2f(0.25,0.75f); glVertex3f(-MitadLado, 0.0f, -MitadLado);   
        glTexCoord2f(0.5f,0.5f); glVertex3f(-MitadLado, lado, MitadLado);     
        glTexCoord2f(0.25f,0.5f); glVertex3f(-MitadLado, lado, -MitadLado);   

        // Cara derecha
        glNormal3f(1.0f, 0.0f, 0.0f);

        glTexCoord2f(1.0f,0.75f); glVertex3f(MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(1.0f,0.50f); glVertex3f(MitadLado, lado, -MitadLado);     
        glTexCoord2f(0.75f,0.5f); glVertex3f(MitadLado, lado, MitadLado);      

        glTexCoord2f(1.0f,0.75f); glVertex3f(MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(0.75f,0.5f); glVertex3f(MitadLado, lado, MitadLado);      
        glTexCoord2f(0.75f,0.75f); glVertex3f(MitadLado, 0.0f, MitadLado);     

        // Cara superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.5f,0.25f); glVertex3f(-MitadLado, lado, -MitadLado);    
        glTexCoord2f(0.75f,0.25f); glVertex3f(MitadLado, lado, -MitadLado);    
        glTexCoord2f(0.75f,0.5f); glVertex3f(MitadLado, lado, MitadLado);      

        glTexCoord2f(0.5f,0.25f); glVertex3f(-MitadLado, lado, -MitadLado);    
        glTexCoord2f(0.75f,0.5f); glVertex3f(MitadLado, lado, MitadLado);      
        glTexCoord2f(0.5f,0.5f); glVertex3f(-MitadLado, lado, MitadLado);     

        // Cara inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.5f,1.0f); glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(0.5f,0.75f); glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glTexCoord2f(0.75f,0.75f); glVertex3f(MitadLado, 0.0f, MitadLado);      

        glTexCoord2f(0.5f,1.0f); glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glTexCoord2f(0.75f,0.75f); glVertex3f(MitadLado, 0.0f, MitadLado);      
        glTexCoord2f(0.75f,1.0f); glVertex3f(MitadLado, 0.0f, -MitadLado);    

        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
};


Dado Dado(5);

class plano:public Objeto3D {
  public:
    float lado;
    GLuint texId;

    plano(float ladoDado){
      this->lado = ladoDado;
      texId = 0;
    }

    void CargarTextura(const char* archivo) {

      if(texId != 0){
        return;
      }
        unsigned ancho, alto;
        unsigned char* data = LeerArchivoJPEG(archivo, ancho, alto); 


        // Genera un identificador de textura y la activa
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);

        // Configuración de parámetros de la textura dados en teoria
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Envia los datos de la imagen a OpenGL y libera la memoria
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


        std::cout << "Textura cargada y aplicada con éxito." << std::endl;
    }


  void draw(){
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    glBegin(GL_QUADS);
    glNormal3f(0.0,1.0,0.0);
    glTexCoord2f(1.0,1.0); glVertex3f(lado,0.0,lado/2);
    glTexCoord2f(1.0,0.0); glVertex3f(lado,0.0,-lado/2);
    glTexCoord2f(0.0,0.0); glVertex3f(-lado,0.0,-lado/2);
    glTexCoord2f(0.0,1.0); glVertex3f(-lado,0.0,lado/2);

    glEnd();
    glDisable(GL_TEXTURE_2D);
  }

};

plano plano(50);



/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void)
{

  GLfloat pos[] = { -5.0f, 0.0f, 40.0f, 1.0f }; // Luz colocada en otra posición

  float  color[4] = { 0.8, 0.0, 1, 1 };
  float  color2[4] = { 1.0, 0.0, 0.0, 1 };

  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv(GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

  ejesCoordenadas.draw();			// Dibuja los ejes

  // Practica 1
/*
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  Cubo.draw(); // dibuja el cubo

  glTranslatef(5,0,0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  Piramide.draw();

  gl  Disable(GL_CULL_FACE);
*/
//PRACTICA 2
/*
  // las teclas "t" e "y"  cambian los modos de los ply
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
  glTranslatef(-4,0,0);
  if(malla1.getModo() == "GL_FLAT"){
    malla1.drawFlat(); 
  }else{
    malla1.drawSmooth(); 
  }
  glTranslatef(10,0,0);
  glRotatef(270,0,1,0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  if(malla2.getModo() == "GL_FLAT"){
    malla2.drawFlat(); 
  }else{
    malla2.drawSmooth(); 
  }
*/

  // Practica 3
/*
  if(Robot.getModo() == "GL_FLAT"){
    Robot.drawFlat(); 
  }else{
    Robot.drawSmooth(); 
  }

  if(Robot.HayAnimacion){
    Animacion();
  }
*/

// Practica 4
glPushMatrix();
//DADO CON TEXTURA
glDisable(GL_CULL_FACE);
Dado.CargarTextura("dado.jpg");
Dado.draw();


//MATERIALES
// Materiales para la primera malla (es un rojo mate donde no refleja nada de luz)
GLfloat materialDifuso1[] = { 0.8f, 0.1f, 0.1f, 1.0f };   // Rojo 
GLfloat materialEspecular1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // baja especular
GLfloat materialAmbiente1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 

// Configurar material de malla1
glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso1);
glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular1);
glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente1);

// sin brillo para el objeto mate
glMaterialf(GL_FRONT, GL_SHININESS, 0.0);

glTranslatef(10, 0, 0);
if (malla1.getModo() == "GL_FLAT") {
    malla1.drawFlat();
} else {
    malla1.drawSmooth();
}

// Materiales para la segunda malla (algo parecido a una esmeralda o algo parecido con mucho brillo)
GLfloat materialDifuso2[] = { 0.2f, 1.0f, 0.2f, 1.0f };   // Verde
GLfloat materialEspecular2[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // alto especular
GLfloat materialAmbiente2[] = { 0.1f, 0.2f, 0.1f, 1.0f };  

// Configurar material de malla2
glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso2);
glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular2);
glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente2);

// mucho brillo para la esmeralda
glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

glTranslatef(-20, 0, 0);
if (malla1.getModo() == "GL_FLAT") {
    malla1.drawFlat();
} else {
    malla1.drawSmooth();
}

// Materiales para la tercera malla (se quiere asemejar a mármol)
GLfloat materialDifuso3[] = { 0.9f, 0.9f, 0.9f, 1.0f };   // Blanco
GLfloat materialEspecular3[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Especular medio
GLfloat materialAmbiente3[] = { 0.3f, 0.3f, 0.3f, 1.0f }; 

// Configurar material de malla3
glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso3);
glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular3);
glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente3);

// Brillo bajo para mármol
glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

glTranslatef(20, 0, 10);
if (malla1.getModo() == "GL_FLAT") {
    malla1.drawFlat();
} else {
    malla1.drawSmooth();
}
glPopMatrix();

// PLANO MAPEADO
GLfloat materialAmbientebasket[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // ambiente claro para que se vea bien
glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbientebasket);

glTranslatef(0.0,-6.2,0.0);
//si no queremos la textura simplemente comentamos
plano.CargarTextura("baloncesto.jpg");
plano.draw();



glPopMatrix ();		// Desapila la transformacion geometrica


glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion


}



/**	void idle()

Procedimiento de fondo. Es llamado por glut cuando no hay eventos pendientes.

**/
void idle (int v)
{

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}
