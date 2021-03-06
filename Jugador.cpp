#include "pch.h"
#include "Jugador.h"
#include "Limites_mapa.h"
#include <iostream>
Jugador::Jugador() {}

Jugador::Jugador(int pini_f, int pfin_f, int pini_c, int pfin_c): Personaje(pini_f, pfin_f, pini_c, pfin_c) {
	pos_x = 100;
	pos_y = 40;
	direccion = puntos = 0;
	velocidad = 5;
}
Jugador::~Jugador(){}

void Jugador::a�adir_10puntos() { puntos += 10; }
int Jugador::retornar_puntos() { return puntos; }

void Jugador::disparar() {
	Proyectil** copia = new Proyectil * [cantidad_proyectiles + 1];
	//Proyectil proyectil_nuevo = Proyectil(0, 0, 0, 8);
	for (int j = 0; j < cantidad_proyectiles; j++) copia[j] = arreglo_proyectiles[j];

	copia[cantidad_proyectiles] = new Proyectil(0, 8, 0, 0);//Establecer par�metros.
	copia[cantidad_proyectiles]->cambiar_sf(rand()%5);
	copia[cantidad_proyectiles]->cambiar_sc(0);
	copia[cantidad_proyectiles]->set_dy(0);
	copia[cantidad_proyectiles]->set_dx(0);


	if (sf == 3) {// arriba
		copia[cantidad_proyectiles]->set_dy(-8);
		//Para decir de donde sale la bala
		copia[cantidad_proyectiles]->set_pos_y(pos_y);
		copia[cantidad_proyectiles]->set_pos_x(pos_x + w / 2);

	}
	if (sf == 0) {//Abajo
		copia[cantidad_proyectiles]->set_dy(8);
		//
		copia[cantidad_proyectiles]->set_pos_y(pos_y + h);
		copia[cantidad_proyectiles]->set_pos_x(pos_x + w / 2);
	}
	if (sf == 1) {// derecha
		copia[cantidad_proyectiles]->set_dx(8);
		//
		copia[cantidad_proyectiles]->set_pos_y(pos_y + h / 2);
		copia[cantidad_proyectiles]->set_pos_x(pos_x + w);
	}
	if (sf == 2) {// izquierda
		copia[cantidad_proyectiles]->set_dx(-8);
		//
		copia[cantidad_proyectiles]->set_pos_y(pos_y + h / 2);
		copia[cantidad_proyectiles]->set_pos_x(pos_x - 100);
	}




	(cantidad_proyectiles)++;
	arreglo_proyectiles = copia;

}

void Jugador::mover_proyectiles(Graphics^ g, Bitmap^ img_bala) {
	//Borrar las balas que han sido marcadas como eliminadas:

	//Cuento cu�ntas hay
	int cantidad_balas_a_eliminar = 0;
	for (int j = 0; j < cantidad_proyectiles; j++) {
		cantidad_balas_a_eliminar += arreglo_proyectiles[j]->retonar_eliminar() ? 1 : 0;
	}

	//Creo arreglo con la cantidad de balas que quedan
	Proyectil** copia = new Proyectil * [cantidad_proyectiles - cantidad_balas_a_eliminar];
	//Las paso al arreglo
	int ind = 0;
	for (int j = 0; j < cantidad_proyectiles; j++) {
		if (!arreglo_proyectiles[j]->retonar_eliminar()) {
			copia[ind] = arreglo_proyectiles[j];
			ind++;
		}
	}
	//Actualizo variables
	cantidad_proyectiles -= cantidad_balas_a_eliminar;
	arreglo_proyectiles = copia;

	//Muevo todas las balas que quedaron
	for (int j = 0; j < cantidad_proyectiles; j++) {

		arreglo_proyectiles[j]->Mover(g, img_bala, 1);
	}
}

void Jugador::caminar(Graphics^ g, Bitmap^ img, Bitmap^ img_proyectiles, int mapa_principal) {
	/*
	0 = arriba
	1 = abajo
	2 = izquierda
	3 = derecha
	4 = neutro
	*/
	dy = dx = 0;
	w = img->Width / 4;
	h = img->Height / 4;


		//Coordenadas en base a los pies (abajo derecha).
	pos_x += w;
	pos_y += h;
	switch (direccion) {

	case 0: {//Arriba
		if (determinar_disponibilidad_posicion(pos_x,pos_y - velocidad, 0, mapa_principal)) dy = -velocidad; dx = 0;
		sc++;
		sf = 3;
		break;
	}
	case 1: {// Abajo
		if (determinar_disponibilidad_posicion(pos_x , pos_y + velocidad, 1, mapa_principal)) dy = velocidad; dx = 0;
		sc++;
		sf = 0;
		break;
	}
	case 2: {//izq
		if (determinar_disponibilidad_posicion(pos_x - 5 - velocidad - (w/6), pos_y, 2, mapa_principal)) dx = -velocidad; dy = 0;
		sc++;
		sf = 2;
		break;

	}
	case 3: {//derecha
		if (determinar_disponibilidad_posicion(pos_x + velocidad , pos_y, 3, mapa_principal)) dx = velocidad; dy = 0;
		sc++;
		sf = 1;
		break;

	}
	case 4: {dy = 0; dx = 0; sc = 1;
		if (sf == 2) sc = 0;
		break; }
	default: break;
	}
	//Vuelta a las coordenadas normales

	pos_x -= w;
		pos_y -= h;
	pos_x += dx;
	pos_y += dy;

	if (sc == 4) sc = 0;
	dibujar(g, img);

	direccion = 4;

	mover_proyectiles(g, img_proyectiles);
}

bool Jugador::determinar_disponibilidad_posicion(int x, int y, int direccion, int mapa_principal) {

	/*
0 = arriba
1 = abajo
2 = izquierda
3 = derecha
4 = neutro
*/
	if (mapa_principal == 0) {
		switch (direccion)
		{
		case 0: {
			if (pos_y - velocidad < 0) return false;
			else return true;
		}
		case 1: {
			if (pos_y + velocidad > 898) return false;
			else return true;
		}
		case 2: {
			if (pos_x - velocidad < 0) return false;
			else return true;
		}
		case 3: {
			if (pos_x + velocidad > 1100) return false;
			else return true;
		}
		default:
			break;
		}
	}
	else if (mapa_principal == 1) {
		if (limites_mapa[y][x] == 1) return true;
		else {
			dy = 0; dx = 0; 
			return false;
		}

	}



}