// AUTOR: 
// FECHA: 
// EMAIL: 
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos
// PRÁCTICA Nº: 4
// ESTILO: Google C++ Style Guide
// COMENTARIOS:
// 

#ifndef SLLPOLYNOMIAL_H_
#define SLLPOLYNOMIAL_H_

#include <iostream>
#include <math.h>  // fabs, pow

#include "pair_t.h"
#include "sll_t.h"
#include "vector_t.h"

#define EPS 1.0e-6

typedef pair_t<double> pair_double_t;  // Campo data_ de SllPolynomial
typedef sll_node_t<pair_double_t> SllPolyNode;  // Nodos de SllPolynomial

// Clase para polinomios basados en listas simples de pares
class SllPolynomial : public sll_t<pair_double_t> {
 public:
  // constructores
  SllPolynomial(void) : sll_t() {};
  SllPolynomial(const vector_t<double>&, const double = EPS);

  // destructor
  ~SllPolynomial() {};

  // E/S
  void Write(std::ostream& = std::cout) const;
  
  // operaciones
  double Eval(const double) const;
  bool IsEqual(const SllPolynomial&, const double = EPS) const;
  void Sum(const SllPolynomial&, SllPolynomial&, const double = EPS);
};


bool IsNotZero(const double val, const double eps = EPS) {
  return fabs(val) > eps;
}

// FASE II
// constructor
SllPolynomial::SllPolynomial(const vector_t<double>& v, const double eps) {
  // poner el código aquí
  // recorremos el vector "al reves" para aprovechar el metodo push_front que ya esta implementado
  for (int i = v.get_size() - 1; i >= 0; --i) {
    if (IsNotZero(v[i], eps)) {
      pair_double_t pair_dummy(v[i], i); // construimos un pair_double_t temporal
      push_front(new SllPolyNode(pair_dummy)); // introducimos el nodo temporal al principio de nuestra lista
    }
  }
}

// E/S
void SllPolynomial::Write(std::ostream& os) const {
  os << "[ ";
  bool first{true};
  SllPolyNode* aux{get_head()};
  while (aux != NULL) {
    int inx{aux->get_data().get_inx()};
    double val{aux->get_data().get_val()};
    if (val > 0)
      os << (!first ? " + " : "") << val;
    else
      os << (!first ? " - " : "-") << fabs(val);
    os << (inx > 1 ? " x^" : (inx == 1) ? " x" : "");
    if (inx > 1)
      os << inx;
    first = false;
    aux = aux->get_next();
  }
  os << " ]" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const SllPolynomial& p) {
  p.Write(os);
  return os;
}


// Operaciones con polinomios

// FASE III
// Evaluación de un polinomio representado por lista simple
double SllPolynomial::Eval(const double x) const {
  double result{0.0};
  SllPolyNode *aux = get_head();
  while (aux != NULL) {
    result = result + aux->get_data().get_val() * pow(x, aux->get_data().get_inx());
    aux = aux->get_next();
  }
  return result;
}

// Comparación si son iguales dos polinomios representados por listas simples
bool SllPolynomial::IsEqual(const SllPolynomial& sllpol,
			    const double eps) const {
  bool differents = false;
  if (fabs(Eval(1) - sllpol.Eval(1)) > eps) {
    return false;
  }
  return !differents;
}

// FASE IV
// Generar nuevo polinomio suma del polinomio invocante mas otro polinomio
void SllPolynomial::Sum(const SllPolynomial& sllpol,
			SllPolynomial& sllpolsum,
			const double eps) {
  SllPolyNode *aux = get_head();
  // calculamos el tamaño del primer polinomio
  int pol1_size = 0;
  while (aux != NULL) {
    pol1_size = aux->get_data().get_inx();
    aux = aux->get_next();
  }
  // calculamos el tamaño del segundo polinomio
  aux = sllpol.get_head();
  int pol2_size = 0;
  while (aux != NULL) {
    pol2_size = aux->get_data().get_inx();
    aux = aux->get_next();
  }
  // creamos dos vectores del mismo tamaño que la lista correspondiente
  vector_t<double> pol1_vector(pol1_size + 1);
  vector_t<double> pol2_vector(pol2_size + 1);  
  // rellenamos los vectores con los valores correspondientes en cada indice
  aux = get_head();
  while (aux != NULL) {
    pol1_vector[aux->get_data().get_inx()] = aux->get_data().get_val();
    aux = aux->get_next();
  }
  aux = sllpol.get_head();
  while (aux != NULL) {
    pol2_vector[aux->get_data().get_inx()] = aux->get_data().get_val();
    aux = aux->get_next();
  }
  // asignamos como tamaño del vector suma el tamaño del polinomio de mayor grado
  vector_t<double> suma;
  if (pol1_size >= pol2_size) {
    suma.resize(pol1_size + 1);
  }
  else {
    suma.resize(pol2_size + 1);
  }
  // recorremos el vector suma y sumamos los valores de los polinomios
  for (int i = 0; i < suma.get_size(); ++i) {
    if (i > pol1_size) {
      suma[i] = pol2_vector[i];
    }
    else if (i > pol2_size) {
      suma[i] = pol1_vector[i];
    }
    else {
      suma[i] = pol1_vector[i] + pol2_vector[i];
    }
  }
  // construimos el sllpolsuma
  for (int i = suma.get_size() - 1; i >= 0; --i) {
    if (IsNotZero(suma[i])) {
      pair_double_t pair_dummy(suma[i], i);
      sllpolsum.push_front(new SllPolyNode(pair_dummy));
    }
  }
}

#endif  // SLLPOLYNOMIAL_H_
