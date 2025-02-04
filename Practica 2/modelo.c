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


/**	void initModel()

Inicializa el modelo y de las variables globales


**/
void
initModel ()
{
  glEnable (GL_LIGHTING);
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
    float halfLado = lado / 2.0f;

    glBegin(GL_QUADS);

    // Cara frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-halfLado, 0.0f, halfLado);     
    glVertex3f( halfLado, 0.0f, halfLado);    
    glVertex3f( halfLado, lado, halfLado);    
    glVertex3f(-halfLado, lado, halfLado);     

    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-halfLado, 0.0f, -halfLado);    
    glVertex3f(-halfLado, lado, -halfLado);    
    glVertex3f( halfLado, lado, -halfLado);    
    glVertex3f( halfLado, 0.0f, -halfLado);    

    // Cara izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-halfLado, 0.0f, -halfLado);    
    glVertex3f(-halfLado, 0.0f, halfLado);     
    glVertex3f(-halfLado, lado, halfLado);     
    glVertex3f(-halfLado, lado, -halfLado);   

    // Cara derecha
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(halfLado, 0.0f, -halfLado);    
    glVertex3f(halfLado, lado, -halfLado);     
    glVertex3f(halfLado, lado, halfLado);      
    glVertex3f(halfLado, 0.0f, halfLado);     

    // Cara superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-halfLado, lado, -halfLado);    
    glVertex3f(halfLado, lado, -halfLado);    
    glVertex3f(halfLado, lado, halfLado);      
    glVertex3f(-halfLado, lado, halfLado);     

    // Cara inferior (base)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-halfLado, 0.0f, -halfLado);    
    glVertex3f(-halfLado, 0.0f, halfLado);     
    glVertex3f(halfLado, 0.0f, halfLado);      
    glVertex3f(halfLado, 0.0f, -halfLado);    

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
        float halfLado = lado / 2.0f;  

        glBegin(GL_TRIANGLES);


        // Cara frontal
        glNormal3f(0.0f, 0.5f, 0.5f); 
        glVertex3f(0.0f, altura, 0.0f);         
        glVertex3f(-halfLado, 0.0f, halfLado);    
        glVertex3f(halfLado, 0.0f, halfLado);   

        // Cara derecha
        glNormal3f(0.5f, 0.5f, 0.0f); 
        glVertex3f(0.0f, altura, 0.0f);          
        glVertex3f(halfLado, 0.0f, halfLado);   
        glVertex3f(halfLado, 0.0f, -halfLado);   

        // Cara trasera
        glNormal3f(0.0f, 0.5f, -0.5f);
        glVertex3f(0.0f, altura, 0.0f);           
        glVertex3f(halfLado, 0.0f, -halfLado);    
        glVertex3f(-halfLado, 0.0f, -halfLado);   

        // Cara izquierda
        glNormal3f(-0.5f, 0.5f, 0.0f);
        glVertex3f(0.0f, altura, 0.0f);          
        glVertex3f(-halfLado, 0.0f, -halfLado);  
        glVertex3f(-halfLado, 0.0f, halfLado);   

        glEnd();

        // Dibuja la base de la pirámide (cuadrado)
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f); 

        glVertex3f(-halfLado, 0.0f, -halfLado);   
        glVertex3f(halfLado, 0.0f, -halfLado);   
        glVertex3f(halfLado, 0.0f, halfLado);    
        glVertex3f(-halfLado, 0.0f, halfLado);    

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
malla malla1("big_dodge.ply", "GL_FLAT");
malla malla2("footbones.ply", "GL_SMOOTH");


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



/**	void Dibuja( void )

Procedimiento de dibujo del modelo. Es llamado por glut cada vez que se debe redibujar.

**/

void Dibuja (void)
{

  static GLfloat  pos[4] = { 5.0, 5.0, 10.0, 0.0 };// Posicion de la fuente de luz

  float  color[4] = { 0.8, 0.0, 1, 1 };
  float  color2[4] = { 1.0, 0.0, 0.0, 1 };

  glPushMatrix ();		// Apila la transformacion geometrica actual

  glClearColor (0.0, 0.0, 0.0, 1.0);	// Fija el color de fondo a negro

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Inicializa el buffer de color y el Z-Buffer

  transformacionVisualizacion ();	// Carga transformacion de visualizacion

  glLightfv (GL_LIGHT0, GL_POSITION, pos);	// Declaracion de luz. Colocada aqui esta fija en la escena

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
