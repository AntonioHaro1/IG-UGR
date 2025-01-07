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
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
using namespace std; 
#include "lector-jpg.h"


/**	void initModel()

Inicializa el modelo y de las variables globales


**/

int modo = GL_FILL;
int luz = 1;

void setModo(int M){
  modo = M;
}


class Objeto3D
{
    public: unsigned char color;
    public: GLboolean seleccion;

    Objeto3D()
    {
    }

    virtual void draw( ) = 0; // Dibuja el objeto

    void setSeleccion(){
      if(seleccion){
        seleccion = false;
      }else if(!seleccion){
        seleccion = true;
      }
    }

    void Setcolor(unsigned char color){
      this->color = color;

    }

    GLboolean getSeleccion(){
      return seleccion;
    }

    unsigned char getId(){
      return color;
    }

};



void
initModel ()
{
  
}

vector<Objeto3D*> todosLosObjetos;



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


////////////////////////////////
//Practica 2 

class malla: public Objeto3D{
  public:
    vector<float> vertices_ply;
    vector<int> caras_ply;
    vector<float> normales_caras;
    vector<float> normales_vertices;
    string modo;

    malla(){}

    malla(std::string nombrearchivo, string flatSmooth,GLboolean si) { 
      ply::read(nombrearchivo.c_str(), vertices_ply, caras_ply);
      this->modo = flatSmooth;
			if(si){
				todosLosObjetos.push_back(this);
			}
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

    void SetcolorMalla(unsigned char color) {
        Setcolor(color); 
    }

    GLboolean getSeleccionMalla(){
      return getSeleccion();
    }
    
};

// La malla 1 tiene smooth y malla 2 flat
malla malla1("beethoven.ply", "GL_FLAT",true);

void setModoMalla() {
  if (malla1.modo == "GL_FLAT") {
    malla1.modo = "GL_SMOOTH";
  } else {
    malla1.modo = "GL_FLAT";
  }
}

//practica 3

malla Cabeza("robot_cabeza.ply", "GL_FLAT",false);
malla Torso("robot_torso.ply", "GL_FLAT",false);
malla BrazoAlto("robot_brazosArriba.ply", "GL_FLAT",false);
malla BrazoAbajo("robot_brazosAbajo.ply", "GL_FLAT",false);
malla ManoDerecha("robotmano.ply", "GL_FLAT",false);
malla ManoIzquierda("robotmano.ply", "GL_FLAT",false);


class Robot: public Objeto3D{
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
			todosLosObjetos.push_back(this);
      
    }

    void draw(){

    }

    void drawFlat(){

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

		void SetcolorRobot(unsigned char color) {
        Setcolor(color); 
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
//PRACTICA 4

class Dado:public Objeto3D {
public:
    float lado;
    GLuint texId;

    Dado(float ladoDado){
      this->lado = ladoDado;
      texId = 0;
			todosLosObjetos.push_back(this);
    }


    void CargarTextura(const char* archivo) {

      if(this->texId != 0){
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

    void drawNoTextura() {

        float MitadLado = lado / 2.0f;

        glBegin(GL_TRIANGLES);

        //Cara frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        
        glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glVertex3f(MitadLado, 0.0f, MitadLado);    
        glVertex3f(MitadLado, lado, MitadLado);

        glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glVertex3f(MitadLado, lado, MitadLado);    
        glVertex3f(-MitadLado, lado, MitadLado);     

        // Cara trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        
        glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glVertex3f(-MitadLado, lado, -MitadLado);    
        glVertex3f(MitadLado, lado, -MitadLado);    
        
        glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glVertex3f(MitadLado, lado, -MitadLado);    
        glVertex3f(MitadLado, 0.0f, -MitadLado);    

        // Cara izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glVertex3f(-MitadLado, lado, MitadLado);     
  
        glVertex3f(-MitadLado, 0.0f, -MitadLado);   
        glVertex3f(-MitadLado, lado, MitadLado);     
        glVertex3f(-MitadLado, lado, -MitadLado);   

        // Cara derecha
        glNormal3f(1.0f, 0.0f, 0.0f);

        glVertex3f(MitadLado, 0.0f, -MitadLado);    
        glVertex3f(MitadLado, lado, -MitadLado);     
        glVertex3f(MitadLado, lado, MitadLado);      

        glVertex3f(MitadLado, 0.0f, -MitadLado);    
        glVertex3f(MitadLado, lado, MitadLado);      
        glVertex3f(MitadLado, 0.0f, MitadLado);     

        // Cara superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-MitadLado, lado, -MitadLado);    
        glVertex3f(MitadLado, lado, -MitadLado);    
        glVertex3f(MitadLado, lado, MitadLado);      

        glVertex3f(-MitadLado, lado, -MitadLado);    
        glVertex3f(MitadLado, lado, MitadLado);      
        glVertex3f(-MitadLado, lado, MitadLado);     

        // Cara inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glVertex3f(-MitadLado, 0.0f, MitadLado);     
        glVertex3f(MitadLado, 0.0f, MitadLado);      

        glVertex3f(-MitadLado, 0.0f, -MitadLado);    
        glVertex3f(MitadLado, 0.0f, MitadLado);      
        glVertex3f(MitadLado, 0.0f, -MitadLado);    

        glEnd();
        glEnable(GL_LIGHTING);
        
    }
};


Dado Dado(5);

/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/
void ResetOpenGLState() {
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



void ColorSeleccion(int i)
{
  unsigned char r = (i & 0xFF);
  glColor3ub(r,0,0);
}

void DibujaEscena (boolean selec)
{

  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };	// Posicion de la fuente de luz

  float  color[4] = { 0.8, 0.0, 1, 1 };
  float  color2[4] = { 1.0, 0.0, 0.0, 1 };

  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena
  
  ejesCoordenadas.draw();			// Dibuja los ejes


  //Objetos Practica 2
  glTranslatef(-7,0,0);
  ColorSeleccion(100);
  malla1.Setcolor(100);
	if(malla1.getSeleccion()){
			glDisable(GL_LIGHTING);
			if(malla1.getModo() == "GL_FLAT"){
        malla1.drawFlat(); 
      }else{
        malla1.drawSmooth(); 
      }
			if(selec){
				glEnable(GL_LIGHTING);
			}
		}else if(!malla1.getSeleccion()){
					GLfloat materialDifuso2[] = { 0.2f, 1.0f, 0.2f, 1.0f };   // Verde
					GLfloat materialEspecular2[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // alto especular
					GLfloat materialAmbiente2[] = { 0.1f, 0.2f, 0.1f, 1.0f };  

					// Configurar material de malla2
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso2);
					glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular2);
					glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente2);

					// mucho brillo para la esmeralda
					glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
					if(malla1.getModo() == "GL_FLAT"){
            malla1.drawFlat(); 
          }else{
            malla1.drawSmooth(); 
          }
		}

	//Objeto practica 3
	glTranslatef(7,0,0);
	ColorSeleccion(250);
  Robot.Setcolor(250);

	if(Robot.getSeleccion()){
			glDisable(GL_LIGHTING);
      if(Robot.getModo() == "GL_FLAT"){
        Robot.drawFlat(); 
      }else{
        Robot.drawSmooth(); 
      }
			if(selec){
				glEnable(GL_LIGHTING);
			}
		}else if(!Robot.getSeleccion()){
			GLfloat materialDifuso1[] = { 0.8f, 0.1f, 0.1f, 1.0f };   // Rojo 
			GLfloat materialEspecular1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // baja especular
			GLfloat materialAmbiente1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 

			// Configurar material de malla1
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso1);
			glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular1);
			glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente1);

			// sin brillo para el objeto mate
			glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
			if(Robot.getModo() == "GL_FLAT"){
        Robot.drawFlat(); 
      }else{
        Robot.drawSmooth(); 
      }
		}

    if(Robot.HayAnimacion){
      Animacion();
    }

	glDisable(GL_CULL_FACE); 
	glTranslatef(0, -10, 5);
  ResetOpenGLState();
  ColorSeleccion(40);
  Dado.Setcolor(40);
  if(Dado.getSeleccion()){
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    Dado.drawNoTextura();

  }else if(!Dado.getSeleccion()){
    if(Dado.texId == 0){
      Dado.CargarTextura("dado.jpg");
    }
    Dado.draw();    
  }

  //printf(" size: %d \n",todosLosObjetos.size());

  glPopMatrix();
  //glutSwapBuffers ();		// Intercambia el buffer de dibujo y visualizacion
}

int pick(int x, int y,int * i)
{
 
    GLint viewport[4];
    unsigned char data[4];
	  int f=0;

    glGetIntegerv (GL_VIEWPORT, viewport);
    glDisable(GL_DITHER); 
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    DibujaEscena(false);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER); 

   glReadPixels(x, viewport[3] - y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
      if(data[0] > 0){
    *i = data[0];
   }else{
    *i = -1;
   }

   return *i;
}

void HacerSeleccion(int k){

  for (size_t i = 0; i < todosLosObjetos.size(); i++) {
    if (k == todosLosObjetos[i]->getId()) {
      todosLosObjetos[i]->setSeleccion(); // Marca el objeto como seleccionado
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
void idle (int v)
{

  glutPostRedisplay ();		// Redibuja
  glutTimerFunc (30, idle, 0);	// Vuelve a activarse dentro de 30 ms
}
