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
modulo entradaTeclado.c
	Gestion de eventos de teclado
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>		// Libreria de utilidades de OpenGL
#include "practicasIG.h"
#include <iostream>
using namespace std;

/** 

Imprime en la consola las instrucciones del programa

**/
void printHelp ()
{

  printf ("\n\n     Prácticas de Informática Gráfica	Curso 2023-2024");
  printf ("\n\n Dpto. Lenguajes y Sistemas Informaticos");
  printf ("\n E.T.S.I. Informatica		Univ. de Granada ");
  printf ("\n");
  printf ("\n Opciones: \n\n");
  printf ("h, H: Imprime informacion de ayuda \n");
  printf ("PgUp, PgDn: avanza y retrocede la cámara \n\n");
  printf ("+,-: avanza y retrocede la cámara \n\n");
  printf ("Teclas de movimiento de cursor: giran la camara\n");
  // Anyade la informacion de las opciones que introduzcas aqui !!       

  printf ("\n Escape: Salir");
  printf ("\n\n\n");
}




/* @teclado ---------------------------------------------------------------- */

/** 		void letra (unsigned char k, int x, int y)

Este procedimiento es llamado por el sistema cuando se pulsa una tecla normal
El codigo k es el ascii de la letra

Para anyadir nuevas ordenes de teclado coloca el correspondiente case.

Parametros de entrada:

k: codigo del caracter pulsado

x:

y:

**/
float rotyCamara = 0;
float ejex = 0;
float ejez = 0;

float normalizarAngulo(float angulo) {
    angulo = fmod(angulo, 360.0f); 
    if (angulo < 0) angulo += 360.0f; 
    return angulo;
}

void letra(unsigned char k, int x, int y) {
    // Calcular dirección de movimiento basada en rotyCamara
    get_angulo_xz(rotyCamara,ejex,ejez);
    rotyCamara = normalizarAngulo(rotyCamara);

    float dirX; 
    float dirZ;
    float lateralx;
    float lateralz;
    float rotyRadianes = rotyCamara * M_PI / 180.0; 
    float auxz,auxx;
    cout << rotyRadianes << endl;
    switch (k) {
        case 'h':
        case 'H':
            printHelp(); // H y h imprimen ayuda
            break;

        // MOVIMIENTO DEL PERSONAJE
        case 'w': // Adelante
        case 'W':
        dirX = cos(rotyRadianes);
        dirZ = sin(rotyRadianes);
        ejez += dirZ * 0.75;
        ejex -= dirX * 0.75; 
        break;

        case 's': // Atrás
        case 'S':
            dirX = cos(rotyRadianes);
            dirZ = sin(rotyRadianes);
            ejez -= dirZ * 0.75; 
            ejex += dirX * 0.75; 
        break;

        case 'a': // Izquierda
        case 'A': {
            float anguloIzquierda = normalizarAngulo(rotyCamara - 90.0f); 
            float anguloIzquierdaRad = anguloIzquierda * M_PI / 180.0f;  
            float lateralx = cos(anguloIzquierdaRad);
            float lateralz = sin(anguloIzquierdaRad);
            ejex -= lateralx * 0.5; 
            ejez += lateralz * 0.5;
        break;
        }

        case 'd': // Derecha
        case 'D': {
            float anguloDerecha = normalizarAngulo(rotyCamara + 90.0f); 
            float anguloDerechaRad = anguloDerecha * M_PI / 180.0f;  
            float lateralx = cos(anguloDerechaRad);
            float lateralz = sin(anguloDerechaRad);
            ejex -= lateralx * 0.5; 
            ejez += lateralz * 0.5;
        break;
        }
        case 27: // Escape: Terminar
            exit(0);
            break;
        default:
            return;
    }

    // Actualizar la cámara con los valores modificados
    setCamaraTecla(ejex, ejez);

    // Actualizar la pantalla
    glutPostRedisplay();
}
