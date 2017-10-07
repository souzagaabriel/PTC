#ifndef FILA_H
#define	FILA_H
    
template <typename T> class fila {
public:
    fila(unsigned int max_itens);
    fila(const fila<T>& outra);
    ~fila();
    fila<T>& operator=(const fila<T> & outra);
    void enfileira(const T & algo);
    T desenfileira();
    const T & frente() const;
    bool vazia() const;
    bool cheia() const;
    unsigned int comprimento() const;
    unsigned int capacidade() const;
private:
    unsigned int itens, cap;
    T * buffer;
    unsigned int inicio, fim;
};

template <typename T> fila<T>::fila(unsigned int N) {
    // inicia os atributos da fila: inicialmente
    // ela está vazia

    cap = N;
    inicio = 0;
    fim = 0;
    itens = 0;
 
    // aloca memória para guardar "capacidade" dados do tipo "T"
    buffer = new T[cap];

}

template <typename T> fila<T>::fila(const fila <T> & outra) {
*this = outra;
 }

template <typename T> fila<T>::~fila() {
    delete[] buffer;
}

template <typename T> fila<T>& fila<T>::operator=(const fila & outra) {
  itens = outra.itens;
  cap = outra.cap;
  inicio = 0;
  fim = cap;

  delete[] buffer;

  buffer = new T[cap];
  
  for (int j=0;j<fim;j++){
   int pos = (outra.inicio + j) % outra.itens;
   buffer[j] = outra.buffer[pos];
  }
  
  
  /*int n = inicio;
  while(n < aux && n!= fim){
	buffer[n] = outra.buffer[n];
	n++;   
  }
  n = 0;
  
  while(n <= fim){
     buffer[n] = outra.buffer[n];
     n++;  
  }*/
}

template <typename T> void fila<T>::enfileira(const T& algo) {
  if(cheia()) throw "Fila cheia!";
 
  buffer[fim] = algo;
  
  fim = (fim + 1) % cap;

  itens++;
}

template <typename T> T fila<T>::desenfileira() {
  if(cap > 0){
    T mostra;
    mostra = buffer[inicio];
    inicio++;
    if(inicio == cap) inicio = 0;
    itens--;

    return mostra;
  }else throw -1;
}

template <typename T> const T & fila<T>::frente() const {
	if( not vazia())return buffer[inicio];
        else throw -1;
}

template <typename T> bool fila<T>::vazia() const {
    return itens == 0;
}

template <typename T> bool fila<T>::cheia() const {
     return itens == cap;
}

template <typename T> unsigned int fila<T>::capacidade() const {
    return cap;
}

template <typename T> unsigned int fila<T>::comprimento() const {
    return itens;
}
#endif	/* FILA_IMP_H */
