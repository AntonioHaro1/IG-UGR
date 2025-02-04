
/*
=======================================================/
Malla.c 

*/
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		
#include "practicasIG.h"
#include "lector-jpg.h"
#include "malla.h"
using namespace std;



Objeto3D::Objeto3D(){}

void  Objeto3D::setSeleccion(){
    if(seleccion){
        seleccion = false;
    }else if(!seleccion){
        seleccion = true;
    }
}

void  Objeto3D::Setcolor(unsigned char color){
    this->color = color;
}

GLboolean  Objeto3D::getSeleccion(){
    return seleccion;
}

unsigned char  Objeto3D::getId(){
    return color;
}

string Objeto3D::getTipo(){
    return tipo;
 }

void Objeto3D::setTipo(string cad){
    tipo = cad;
 }

malla::malla(){}

malla::malla(std::string nombrearchivo, std::string flatSmooth, GLboolean si) {
    ply::read(nombrearchivo.c_str(), vertices_ply, caras_ply, texturas_ply);
    
    // Establecer el modo de sombreado
    this->modo = flatSmooth;
}

void malla::draw(){

} 

void malla::drawFlat(GLuint texturaID){

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturaID);

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
        glNormal3f(x, y, z);  
        glTexCoord2f(texturas_ply[indice1 * 2], texturas_ply[indice1 * 2 + 1]);
        glVertex3f(vertices_ply[indice1 * 3], vertices_ply[indice1 * 3 + 1], vertices_ply[indice1 * 3 + 2]);

        glTexCoord2f(texturas_ply[indice2 * 2], texturas_ply[indice2 * 2 + 1]);
        glVertex3f(vertices_ply[indice2 * 3], vertices_ply[indice2 * 3 + 1], vertices_ply[indice2 * 3 + 2]);
        glTexCoord2f(texturas_ply[indice3 * 2], texturas_ply[indice3 * 2 + 1]);
        glVertex3f(vertices_ply[indice3 * 3], vertices_ply[indice3 * 3 + 1], vertices_ply[indice3 * 3 + 2]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void malla::drawFlatNotexture(){
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
        glNormal3f(x, y, z);  
        glTexCoord2f(texturas_ply[indice1 * 2], texturas_ply[indice1 * 2 + 1]);
        glVertex3f(vertices_ply[indice1 * 3], vertices_ply[indice1 * 3 + 1], vertices_ply[indice1 * 3 + 2]);

        glTexCoord2f(texturas_ply[indice2 * 2], texturas_ply[indice2 * 2 + 1]);
        glVertex3f(vertices_ply[indice2 * 3], vertices_ply[indice2 * 3 + 1], vertices_ply[indice2 * 3 + 2]);
        glTexCoord2f(texturas_ply[indice3 * 2], texturas_ply[indice3 * 2 + 1]);
        glVertex3f(vertices_ply[indice3 * 3], vertices_ply[indice3 * 3 + 1], vertices_ply[indice3 * 3 + 2]);
    }
    glEnd();

}

void malla::drawSmooth(){
      
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

void malla::calcular_normales(){

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


void malla::drawFlat(){
      
    //glShadeModel(GL_FLAT);
    // Desactivar iluminación temporalmente para probar

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
        glNormal3f(x, y, z);  
        glVertex3f(vertices_ply[indice1 * 3], vertices_ply[indice1 * 3 + 1], vertices_ply[indice1 * 3 + 2]);

        glVertex3f(vertices_ply[indice2 * 3], vertices_ply[indice2 * 3 + 1], vertices_ply[indice2 * 3 + 2]);
        glVertex3f(vertices_ply[indice3 * 3], vertices_ply[indice3 * 3 + 1], vertices_ply[indice3 * 3 + 2]);
    }
    glEnd();

}


string malla::getModo(){
    return this->modo;
}

void malla::SetcolorMalla(unsigned char color) {
    Setcolor(color); 
}

GLboolean malla::getSeleccionMalla(){
    return getSeleccion();
}

void malla::setModoMalla() {
    if (this->modo == "GL_FLAT") {
        this->modo = "GL_SMOOTH";
    } else {
        this->modo = "GL_FLAT";
    }
}

void malla::CargarTextura(const char* archivo) {

      if(this->Text != 0){
        return;
      }

        unsigned ancho, alto;
        unsigned char* data = LeerArchivoJPEG(archivo, ancho, alto); 


        // Genera un identificador de textura y la activa
        glGenTextures(1, &Text);
        glBindTexture(GL_TEXTURE_2D, Text);

        // Configuración de parámetros de la textura dados en teoria
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


        std::cout << "Textura cargada y aplicada con éxito." << std::endl;
}

GLuint malla::getText(){
    return Text;
}





