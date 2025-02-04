/*	Prácticas de Informática Gráfica

	Grupo C					Curso 2023-24

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

=======================================================

	 mouse.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "practicasIG.h"
#include "modelo.h"



#define __mouse__ 
#include "mouse.h"

/**

Procedimiento para gestionar los eventos de pulsacion de los botones del raton.

Argumentos:

boton: Codigo glut del boton que ha manipulado el usuario.

estado: Estado en el que esta el boton que se ha pulsado.

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursor.

**/
void clickRaton(int boton, int estado, int x, int y) {
        
        if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
            setDisparo2();
        }
        if (boton == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN) {
            setDisparo();

        }
 
}

/* Centra el raton al centro de la ventana*/
void centrarRaton() {
    int centroX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centroY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    glutWarpPointer(centroX, centroY);
}

/**

Procedimiento para gestionar los eventos de movimiento del raton.

Argumentos:

x,y: Posicion, en coordenadas de pantalla, en que se encuantra el cursosr.

**/

void RatonMovido(int x, int y) {
    float ax, ay, az, d, izq, der;
    getCamara(ax, ay, az, d, izq, der);

    // Coordenadas del centro de la ventana
    int centroX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int centroY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

    // Calcula los desplazamientos
    int deltaX = x - centroX;
    int deltaY = y - centroY;

    // Aplica los desplazamientos a la cámara
    if (deltaX != 0 || deltaY != 0) {
        ay += deltaX * 0.1f; 
        ax += deltaY * 0.1f;
        
        setCamara(ax, ay, az, d, izq, der);

        centrarRaton();
    }

    glutPostRedisplay();
}

