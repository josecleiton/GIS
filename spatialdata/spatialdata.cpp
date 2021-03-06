#include "spatialdata.hpp"
namespace SpatialData{

Ponto::Ponto(double X, double Y): x(X), y(Y){
}

Ponto operator+(const Ponto& This, const Ponto& Other){
    return Ponto(This.GetX()+Other.GetX(), This.GetY()+Other.GetY());
}

Ponto operator-(const Ponto& This, const Ponto& Other){
    return Ponto(This.GetX()-Other.GetX(), This.GetY()-Other.GetY());
}

Ponto operator*(double K, const Ponto& Other){
    return Ponto(K*Other.GetX(), K*Other.GetY());
}

double operator*(const Ponto& P, const Ponto& K){
    return P.x*K.x+P.y*K.y;
}

bool operator==(const Ponto& This, const Ponto& Other){
    double DX = This.GetX()-Other.GetX();
    double DY = This.GetY()-Other.GetY();
    return (DX==0.0 and DY==0.0)?true:false;
}
bool operator!=(const Ponto& This, const Ponto& Other){
    return !(This==Other);
}
bool operator<(const Ponto& This, const Ponto& Other){
    bool comparacao = (This.GetX()-Other.GetX() == 0.0)?true:false;
    return ((This.GetX() < Other.GetX()) or ((comparacao) and (This.GetY() < Other.GetY())));
}
bool operator<=(const Ponto& This, const Ponto& Other){
    return This < Other or This == Other;
}

bool operator>=(const Ponto& This, const Ponto& Other){
    return This > Other or This == Other;
}

bool operator>(const Ponto& This, const Ponto& Other){
    bool comparacao = (This.GetX()-This.GetY() == 0.0)?true:false;
    return ((This.GetX() > Other.GetX()) or ((comparacao) and (This.GetY() > Other.GetY())));
}

double Ponto::operator[](int k){
    return (k%2)?y:x;
}

int Ponto::Orientacao(Ponto& p0, Ponto& p1, Ponto& p2){
    Ponto a = p1 - p0;
    Ponto b = p2 - p0;
    double sa = a.x*b.y-b.x*a.y;
    if(sa>0.0)
        return 1;
    if(sa<0.0)
        return -1;
    return  0;
}

int Ponto::Classificacao(Ponto& p0, Ponto& p1){
    Ponto p2 = *this;
    Ponto a = p1-p0;
    Ponto b = p2-p0;
    double sa = a.x*b.y-b.x*a.y;
    if(sa>0.0)
        return ESQUERDA;
    if(sa<0.0)
        return DIREITA;
    if((a.x*b.x<0.0) or (a.y*b.y<0.0))
        return ATRAS;
    if(a.Tamanho()<b.Tamanho())
        return FRENTE;
    if(p0 == p2)
        return ORIGEM;
    if(p1 == p2)
        return DESTINO;
    return ENTRE;
}

int Ponto::Classificacao(Aresta E){
    return Classificacao(E.origem, E.destino);
}

double Ponto::AnguloPolar(){
    if((x==0.0) and (y==0.0))
        return -1.0;
    if(x==0.0)
        return (y>0.0)?90:270;
    double theta = atan(y/x); // em radianos
    theta *= 360/(2*PI); // conversão para ângulo
    if(x>0.0) // quadrante 1 e 4
        return (y>=0.0)?theta:360+theta;
    else // quadrante 2 e 3
        return 180+theta;
}

double Ponto::Distancia(Aresta& E){
    Aresta ab = E;
    ab.Flip().Rotacao(); // rotacional em 90º antihorario
    Ponto n(ab.destino-ab.origem); // n = vetor perpendincular a e;
    n = (1.0/n.Tamanho())*n; // normalizando n
    Ponto k = (*this)+n;
    Aresta f(*this, k); // f = n
    double t;
    f.Interseccao(E, t);
    return t;
}

double Ponto::Tamanho(){
    return sqrt(x*x+y*y);
}

double Ponto::Distancia(Ponto& p1){
    return sqrt(pow(x-p1.x, 2)+pow(y-p1.y, 2));
}

QPointF Ponto::toQPoint(){
    return QPointF(this->x, this->y);
}

ostream& operator<<(ostream& out, const Ponto& P){
    out << fixed << showpoint << setprecision(5);
    out << "(" << P.GetX() << ", " << P.GetY() << ")";
    return out;
}

// IMPLEMENTAÇÕES DA CLASSE VERTICE ABAIXO

Vertice::Vertice(double x, double y): Ponto(x,y){
}

Vertice::Vertice(Ponto p): Ponto(p){
}

Vertice* Vertice::Horario(){
    return static_cast<Vertice*>(m_prev);
}

Vertice* Vertice::Antihorario(){
    return static_cast<Vertice*>(m_next);
}

Vertice* Vertice::Vizinho(int rotacao){
    return (rotacao==HORARIO)?Horario():Antihorario();
}

Ponto& Vertice::GetPonto(){
    return *(static_cast<Ponto*>(this));
}

Vertice* Vertice::Push(Vertice* V){
    return static_cast<Vertice*>(Node::Push(V));
}

Vertice* Vertice::Push(Ponto P){
    Vertice* temp = new Vertice(P);
    return Vertice::Push(temp);
}

Vertice* Vertice::Pop(){
    return static_cast<Vertice*>(Node::Pop());
}

void Vertice::Splice(Vertice* V){ // TENHO QUE EXPLICAR PARA OS MENINOS
    Node::Splice(V);
}

Vertice* Vertice::Split(Vertice* b){ // TENHO QUE EXPLICAR PARA OS MENINOS
    Vertice* bp = b->Antihorario()->Push(new Vertice(b->GetPonto()));
    Push(new Vertice(GetPonto()));
    Splice(bp);
    return bp;
}

double Vertice::GetMin(){
    double minx = 0.0, miny= 0.0;
    for(Vertice* i=this; ; i = i->Horario()){
        minx = min(i->GetX(), minx);
        miny = min(i->GetY(), miny);
        if(i==this->Antihorario()) break;
    }
    return min(minx, miny);
}

void Vertice::Kai(){
    Vertice* list = this;
    if(list != nullptr){
        Vertice* aux = list->Horario();
        while(list != aux){
            delete aux->Pop();
            aux = list->Horario();
        }
        delete list;
        list = nullptr;
    }
}

pair<QPointF*, int> Vertice::toQPoint(double gap){
    int tam = 0;
    double menorX = 0.0, menorY = 0.0;
    Ponto temp;
    Vertice* fim = this->Horario();
    for(Vertice* i = this; ; i = i->Antihorario()){
        //array[j] = i->GetPonto().toQPoint();
        temp = i->GetPonto();
        if(gap == 0.0){
            menorX = min(temp.x, menorX);
            menorY = min(temp.y, menorY);
        }
        tam++;
        if(i == fim) break;
    }
    QPointF* array = new QPointF[tam];
    if(gap == 0.0) gap = abs(min(menorX, menorY))+1.0;
    int j = 0;
    for(Vertice* i = this; j < tam; i = i->Antihorario()){
        temp = i->GetPonto();
        temp.x += gap;
        temp.y += gap;
        array[j++] = temp.toQPoint();
    }
    return make_pair(array, tam);
}

// CLASS POLIGONO

Poligono::Poligono(): list(nullptr), m_size(0){
}
Poligono::Poligono(Poligono& p){
    m_size = p.m_size;
    if(!m_size)
        list = nullptr;
    else{
        list = new Vertice(p.GetPonto());
        for(unsigned i=1; i<m_size; i++){
            p.Avancar(HORARIO);
            list = list->Push(new Vertice(p.GetPonto()));
        }
        p.Avancar(HORARIO);
        list = list->Horario();
    }
}

Poligono::Poligono(Vertice* ListaVertice): list(ListaVertice){
    Resize();
}

void Poligono::Resize(){ // DEVE SER CHAMADO SEMPRE QUE UMA CADEIA DE VERTICES DE TAMANHO DESCONHECIDA FOR ADICIONADA AO POLIGONO
    if(list == nullptr)
        m_size = 0;
    else{
        m_size = 1;
        for(Vertice* v = list->Horario(); v != list; ++m_size, v = v->Horario());
    }
}

Poligono::~Poligono(){
    if(!fakeKai) this->GetVertice()->Kai();
    this->list = nullptr;
}

/*
 * Funções de acesso à classe
 */
Vertice* Poligono::GetVertice(){
    return list;
}

unsigned Poligono::GetSize(){
    return m_size;
}

Ponto& Poligono::GetPonto(){
    return list->GetPonto();
}

Vertice* Poligono::Horario(){
    return list->Horario();
}

Vertice* Poligono::Antihorario(){
    return list->Antihorario();
}
Vertice* Poligono::Vizinho(int rotacao){
    return list->Vizinho(rotacao);
}

Vertice* Poligono::Avancar(int rotacao){
    return list = list->Vizinho(rotacao);
}

Vertice* Poligono::SetV(Vertice* v){
    return list = v;
}

Vertice* Poligono::Push(Ponto p){
    if(m_size++ == 0)
        list = new Vertice(p);
    else
        list = list->Push(new Vertice(p));
    return list;
}

void Poligono::Pop(){
    Vertice* aux = list;
    list = (--m_size == 0) ? nullptr : list->Antihorario();
    delete aux->Pop();
}

Poligono* Poligono::Split(Vertice* b){
    Vertice* bp = list->Split(b);
    Resize();
    return new Poligono(bp);
}

double Poligono::Area(){
    double area = 0.0;
    unsigned tam = this->GetSize();
    vector<double> X(tam+1), Y(tam+1);
    Ponto temp;
    for(unsigned i=0; i<tam; i++, this->Avancar(HORARIO)){
        temp = this->GetPonto();
        X[i] = temp.x;
        Y[i] = temp.y;
    }
    X[tam] = X[0];
    Y[tam] = Y[0];
    for(unsigned i=0, j=tam-1; i<tam; j=i, i++)
        area += (X[j]+X[i])*(Y[j]-Y[i]);
    return abs(area/2.0);
}

Ponto Poligono::Centroide(double area){
    unsigned numVertices = this->GetSize();
    vector<double> X(numVertices+1), Y(numVertices+1);
    Ponto temp;
    for(unsigned i=0; i<numVertices; i++, this->Avancar(HORARIO)){
        temp = this->GetPonto();
        X[i] = temp.x;
        Y[i] = temp.y;
    }
    X[numVertices] = X[0];
    Y[numVertices] = Y[0];
    double xc = 0.0, yc = 0.0;
    for(unsigned i=0; i < numVertices; i++){
        xc+=(X[i]+X[i+1])*(X[i]*Y[i+1]-X[i+1]*Y[i])/(6*area);
        yc+=(Y[i]+Y[i+1])*(X[i]*Y[i+1]-X[i+1]*Y[i])/(6*area);
    }
    return Ponto(xc, yc);
}


void Poligono::Apagar(){
    this->list = nullptr;
}

void Poligono::setFakeKai(bool a){
    this->fakeKai = a;
}

int Poligono::PontoNoPoligono(Ponto& P){
    int paridade = 0;
    Aresta a;
    for(unsigned i=0; i<this->GetSize(); i++, this->Avancar(HORARIO)){
        a = this->GetAresta();
        switch(a.Tipo(P)){
            case TOCANDO:
                return FRONTEIRA;
             case CRUZANDO:
                paridade = 1 - paridade;
        }
    }
    return (paridade ? DENTRO: FORA);
}

bool Poligono::PontoNoPoligonoConvexo(Ponto& s){
    unsigned tam = this->GetSize();
    if(tam == 1)
        return (s == this->GetPonto());
    if(tam == 2){
        int c = s.Classificacao(this->GetAresta());
        return (c == ENTRE) or (c == ORIGEM) or (c == DESTINO);
    }
    Vertice* origem = this->GetVertice();
    for(unsigned i=0; i<tam; i++, this->Avancar(HORARIO)){
        if(s.Classificacao(this->GetAresta()) == ESQUERDA){
            this->SetV(origem);
            return false;
        }
    }
    return true;
}

Aresta* Poligono::clipping(Aresta& alvo){
    double t0 = 0.0, t1 = 1.0, t;
    Ponto v = alvo.GetDestino()-alvo.GetOrigem(), n;
    Aresta e, f;
    for(unsigned i=0; i<this->GetSize(); i++, this->Avancar(HORARIO)){
        e = this->GetAresta();
        if(alvo.Interseccao(e, t) == CONSECUTIVO){
            f = e;
            f.Rotacao();
            n = f.GetDestino()-f.GetOrigem();
            if(n*v > 0.0){
                if(t > t0) t0 = t;
            }
            else{
                if(t < t1) t1 = t;
            }
        }
        else{
            if(alvo.GetOrigem().Classificacao(e) == ESQUERDA)
                return nullptr;
        }
    }
    return (t0 <= t1) ? new Aresta(alvo.GetPonto(t0), alvo.GetPonto(t1)) : new Aresta;
}


Poligono* Poligono::clipping(Poligono& alvo){
    Poligono* q = new Poligono(*this), *r = nullptr;
    bool flag = true;
    Aresta a;
    for(unsigned i=0; i<alvo.GetSize(); i++, alvo.Avancar(HORARIO)){
        a = alvo.GetAresta();
        if(clipPoligonoAresta(*q, a, r)){
            delete q;
            q = r;
        }
        else{
            delete q;
            flag = false;
            break;
        }
    }
    return (flag) ? q: new Poligono;
}

bool Poligono::clipPoligonoAresta(Poligono& s, Aresta& e, Poligono*& result){
    Poligono *p = new Poligono;
    Ponto crossingPt, origem, destino;
    bool origemDentro, destDentro;
    for(unsigned i=0; i<s.GetSize(); s.Avancar(HORARIO), i++){
        origem = s.GetPonto();
        destino = s.Horario()->GetPonto();
        origemDentro = origem.Classificacao(e) != ESQUERDA;
        destDentro = destino.Classificacao(e) != ESQUERDA;
        if(origemDentro != destDentro){
            double t;
            e.Interseccao(s.GetAresta(), t);
            crossingPt = e.GetPonto(t);
        }
        if(origemDentro and destDentro)
            p->Push(destino);
        else if(origemDentro and !destDentro){
            if(origem != crossingPt)
                p->Push(crossingPt);
        }
        else if(!origemDentro and !destDentro);
        else{
            p->Push(crossingPt);
            if(destino != crossingPt) p->Push(destino);
        }
    }
    result = p;
    return (result->GetSize() > 0);
}

ostream& operator<<(ostream& out, const Poligono& p){
    Vertice* i = p.list, *fim = i->Antihorario();
    while(true){
        out << i->GetPonto();
        out << " ";
        if(i == fim) break;
        i = i->Horario();
    }
    return out;
}

Aresta Poligono::GetAresta(){
    return Aresta(GetPonto(), list->Horario()->GetPonto());
}

Retangulo Vertice::Envelope(){
    vector<double> X, Y;
    for(auto it = this->Antihorario(); it != this; it = it->Antihorario()){
        X.push_back(it->x);
        Y.push_back(it->y);
    }
    X.push_back(this->x);
    Y.push_back(this->y);
    sort(X.begin(), X.end());
    sort(Y.begin(), Y.end());
    Ponto origem(X.front(), Y.front());
    Ponto destino(X.back(), Y.back());
    return Retangulo(origem, destino);
}

// CLASSE ARESTA

Aresta::Aresta(Ponto _origem, Ponto _dest): origem(_origem), destino(_dest){
}

Aresta::Aresta(): origem(Ponto(0.0, 0.0)), destino(Ponto(0.0,0.0)){

}

Aresta& Aresta::Rotacao(){
    Ponto m = 0.5 * (origem+destino);
    Ponto v = destino - origem;
    Ponto n(v.y, -v.x);
    origem = m - 0.5 * n;
    destino = m + 0.5 * n;
    return *this;
}

Aresta& Aresta::Flip(){
    return Rotacao().Rotacao();
}

Ponto Aresta::GetPonto(double& t){
    return Ponto(origem + t * (destino-origem));
}

double Aresta::Dist() const{
    return sqrt(pow(origem.x - destino.x, 2)+pow(origem.y - destino.y, 2));
}

int Aresta::Interseccao(Aresta E, double& t){
    Ponto a = origem;
    Ponto b = destino;
    Ponto c = E.origem;
    Ponto d = E.destino;
    Ponto n = Ponto((d-c).y, (c-d).x);
    Ponto k = b-a;
    double denom = n*k;
    if(denom == 0.0){
        int aclass = origem.Classificacao(E);
        if(aclass == ESQUERDA or aclass == DIREITA)
            return PARALELA;
        else
            return COLINEAR;
    }
    k = a-c;
    double num = n*k;
    t = -num/denom;
    return CONSECUTIVO;
}

int Aresta::Cruza(Aresta& E, double& t){
    double s;
    int TipoCruzamento = E.Interseccao(*this, s);
    if(TipoCruzamento==COLINEAR or TipoCruzamento==PARALELA)
        return TipoCruzamento;
    if(s < 0.0 or s > 1.0)
        return CONSECUTIVO_NAO_CRUZADO;
    Interseccao(E, t);
    if(t >= 0.0 and t <= 1.0)
        return CONSECUTIVO_CRUZADO;
    return  CONSECUTIVO_NAO_CRUZADO;
}

bool Aresta::isVertical(){
    double t = abs(origem.x-destino.x);
    return (t==0.0)?true:false;
}

double Aresta::y(double x){
    return Inclinacao()*(x-origem.x)+origem.y;
}

double Aresta::Inclinacao(){
    if(destino.x - origem.y == 0.0) return 0.0;
    if(!isVertical())
        return (destino.y - origem.y)/(destino.x - origem.y);
    return DBL_MAX;
}

int Aresta::Tipo(Ponto& a){
    Ponto v = this->origem, w = this->destino;
    switch(a.Classificacao(*this)){
        case ESQUERDA:
            return ((v.y < a.y) and (a.y <= w.x)) ? CRUZANDO : INESSENTIAL;
        case DIREITA:
            return ((w.y < a.y) and (a.y <= v.y)) ? CRUZANDO : INESSENTIAL;
        case ENTRE:
        case ORIGEM:
        case DESTINO:
            return TOCANDO;
        default:
            return INESSENTIAL;
    }
}

double Aresta::Angulo(Ponto& P){
    Ponto v = this->origem - P;
    Ponto w = this->destino - P;
    double va = v.AnguloPolar();
    double wa = w.AnguloPolar();
    if((va+1.0 == 0.0) or (wa+1.0 == 0.0))
        return 180.0;
    double x = wa - va;
    if((x == 180.0) or (x+180.0 == 0.0))
        return 180.0;
    else if(x < -180.0)
        return (x + 360.0);
    else if(x > 180.0)
        return (x - 360.0);
    else
        return x;
}

Ponto Aresta::GetOrigem() const{
    return this->origem;
}

Ponto Aresta::GetDestino() const{
    return this->destino;
}

bool operator==(const Aresta& This, const Aresta& Other){
    return (This.GetDestino() == Other.GetDestino() and This.GetOrigem() == Other.GetOrigem()) or
            (This.GetDestino() == Other.GetOrigem() and This.GetOrigem() == Other.GetDestino());
}

bool operator!=(const Aresta& This, const Aresta& Other){
    return !(This == Other);
}

Retangulo::Retangulo(){
    Ponto A, B;
    Aresta R(A, B);
    diagonal = R;
}

Retangulo::Retangulo(const Retangulo& R){
    this->diagonal = R.diagonal;
    this->area = R.area;
    if(this->area == 0.0)
        CalcularArea();
}

Retangulo::Retangulo(Ponto& origem, Ponto& destino): diagonal(origem, destino){
    CalcularArea();
}

bool Retangulo::Contem(Ponto& P){
    return ((P.x<=max(this->diagonal.origem.x,this->diagonal.destino.x))
           and (P.x >=min(this->diagonal.origem.x,this->diagonal.destino.x))
           and (P.y<=max(this->diagonal.origem.y,this->diagonal.destino.y))
           and (P.y >=min(this->diagonal.origem.y,this->diagonal.destino.y)));
}

bool Retangulo::Contem(Retangulo& R){
    return (this->diagonal.origem <= R.diagonal.origem or (diagonal.origem.x - R.diagonal.origem.x == 0.0) or (diagonal.origem.y - R.diagonal.origem.y == 0.0))
            and (this->diagonal.destino >= R.diagonal.destino or (diagonal.destino.x - R.diagonal.destino.x == 0.0) or (diagonal.destino.y - R.diagonal.destino.y == 0.0));
}

double Retangulo::CalcularArea(){
    this->area =  abs(diagonal.origem.x-diagonal.destino.x) * abs(diagonal.origem.y-diagonal.destino.y);
    return this->area;
}

double Retangulo::GetArea(){
    return area;
}

double Retangulo::Ajusta(Retangulo& R, bool& mod){
    if(R.diagonal != this->diagonal){
            mod = true;
            this->diagonal = R.diagonal;
            this->area = R.GetArea();
            return this->GetArea() - R.GetArea();
    }
    return double(0);
}

Retangulo Retangulo::CresceParaConter(Retangulo& R){
    if(this->Contem(R))
        return *this;
    Ponto A;
    Ponto B;
    if(this->diagonal.origem > R.diagonal.origem and this->diagonal.destino < R.diagonal.destino){
        A = R.diagonal.origem;
        B = R.diagonal.destino;
    }
    else if(this->diagonal.origem > R.diagonal.origem){
        A = R.diagonal.origem;
        B = this->diagonal.destino;
    }
    else if(this->diagonal.destino < R.diagonal.destino){
        A = this->diagonal.origem;
        B = R.diagonal.destino;
    }
    return Retangulo(A,B);
}

bool Retangulo::Interseccao(Retangulo& R){
    //implementar
    Aresta DP = this->GetDiagonal();
    Aresta DR = R.GetDiagonal();
    if((DP.GetDestino().x >= DR.GetOrigem().x) and (DR.GetDestino().x >= DP.GetOrigem().x) and
            (DP.GetDestino().y >= DR.GetOrigem().y) and (DP.GetDestino().y >= DR.GetOrigem().y)) // (Pmax2.y >= Pmin.y))
        return true;
    return false;
}

const Aresta& Retangulo::GetDiagonal(){
    return this->diagonal;
}

bool operator<(const Retangulo& a, const Retangulo& b){
    return b.diagonal.origem < a.diagonal.origem;
}

bool operator>(const Retangulo& a, const Retangulo& b){
    return b.diagonal.destino > a.diagonal.destino;
}

bool operator==(const Retangulo& a, const Retangulo& b){
    return a.diagonal == b.diagonal;
}

bool operator!=(const Retangulo& a, const Retangulo& b){
    return !(a==b);
}

Circulo::Circulo(){
    raio= 0.0;
    centro.x=0.0;
    centro.y=0.0;
}
Circulo::Circulo(double R,Ponto Centro){
    raio= R;
    centro.x= Centro.x;
    centro.y= Centro.y;
}
Ponto Circulo::GetCentro(){
    return this->centro;
}
double Circulo::GetRaio(){
    return  this->raio;
}
void Circulo::SetCentro(Ponto _centro){
    this->centro = _centro;
}
void Circulo::SetRaio(double _raio){
    this->raio = _raio;
}
double Circulo::Diametro(){
    return this->raio*2;
}
double Circulo::Comprimento(){
    return this->raio*2*PI;
}
double Circulo::Area(){
    return PI*pow(this->raio,2);
}

int Circulo::Interseccao(Ponto &P){

    if(pow((this->centro.x-P.y),2)+pow((this->centro.y-P.y),2)< pow(this->raio,2))
        return DENTRO;// PONTO DENTRO DO CIRCULO
    if(pow((this->centro.x-P.y),2)+pow((this->centro.y-P.y),2)> pow(this->raio,2))
        return FORA; // PONTO FORA
    else return FRONTEIRA; // PONTO NA CIRCUNFER?NCIA

}

int Circulo::VerificaInterseccao(Circulo& c2)// VERIFICA SE EXISTE INTERSEÇÃO ENTRE CIRCULOS
{
    Circulo c1 = *this;
    double dist= sqrt ( pow ((c2.centro.x - c1.centro.x), 2.0 ) + pow((c2.centro.y - c1.centro.y ), 2.0 ));
    double soma=c1.raio+c2.raio;
    double subt= c1.raio-c2.raio;

    if(dist>soma|| dist<abs(subt))
        return NAO_HA_INTERSECCAO; // círculos não se interceptam
    double diff = c1.raio-c2.raio;
    if(dist==0.0 && diff==0.0)
        return CIRCULOS_INDENTICOS; // circulos idênticos

    return HA_INTERSECCAO; //saida de interseção
}

pair<Vertice*, unsigned> Circulo::Interseccao(Circulo& c2){ //   PONTOS QUE INTERCEPTA DOIS CIRCULOS
    Circulo c1 = *this;
    double dist= sqrt ( pow ((c2.centro.x - c1.centro.x), 2.0 ) + pow((c2.centro.y - c1.centro.y ), 2.0 ));
    double A , H;
    Ponto P,P1 ,P2;

    A= (pow(c1.raio,2.0)-pow(c2.raio,2.0)+pow(dist,2.0))/(dist*2.0);
    H= sqrt(pow(c1.raio,2.0) - pow(A,2.0));

    P.x=c1.centro.x + A*(c2.centro.x-c1.centro.x)/dist;
    P.y=c1.centro.y +A*(c2.centro.y-c1.centro.y)/dist;

    P1.x= P.x + H*(c2.centro.y-c1.centro.y)/dist;
    P1.y= P.y - H*(c2.centro.x-c1.centro.x)/dist;

    Vertice* resultado = new Vertice(P1);
    unsigned tam = 1;
     P2.x= P.x - H*(c2.centro.y-c1.centro.y)/dist;
     P2.y= P.y + H*(c2.centro.x-c1.centro.x)/dist;
     resultado->Push(P2);
     tam++;

    return make_pair(resultado, tam);
}

pair<Vertice*, unsigned> Circulo::Interseccao(Aresta& R){
    Circulo T = *this;
    Ponto p1 = R.GetDestino(), p2 = R.GetOrigem();
    Vertice* resultado = nullptr;
    double A ,B, C, Delta;
    double u1,u2;
    Ponto R1,R2;

    A= pow((p2.x-p1.x),2.0) + pow((p2.y-p1.y),2.0);
    B= -2.0*(p1.x*p1.x+p1.y*p1.y-p1.x*p2.x-p1.y*p2.y+T.centro.x*(p2.x-p1.x)+T.centro.y*(p2.y-p1.y));
    C= pow((T.centro.x-p1.x),2.0)+ pow((T.centro.y-p1.y),2.0)- pow(T.raio,2.0);

    Delta = B*B- 4.0* A * C;

    u1 = (-B + sqrt(Delta))/(2.0*A);
    R1.x= p1.x+ u1*(p2.x-p1.x);
    R1.y=p1.y+ u1*(p2.y-p1.y);
    unsigned tam = 0;
    if(Delta==0.0) {// um ponto de interseção
        u1=-B /(2.0*A);
        R1.x= p1.x + (p2.x-p1.x)*u1;
        R1.y=p1.y + (p2.y-p1.y)*u1;
        resultado = new Vertice(R1);
        tam=1;
    }
    else if(Delta>0) // dois pontos de interseção
    {        u1 = (-B - sqrt(Delta))/(2.0*A);
             u2 = (-B + sqrt(Delta))/(2*A);
             R1.x= p1.x + (p2.x-p1.x)*u1;
             R1.y= p1.y + (p2.y-p1.y)*u1;
             R2.x= p1.x+ (p2.x-p1.x)*u2;
             R2.y= p1.y+ (p2.y-p1.y)*u2;
             resultado = new Vertice(R1);
             resultado->Push(R2);
            // Retornar R1 e R2
             tam=2;
    }
    return make_pair(resultado, tam);
}

Retangulo Circulo::Envelope(){
    Ponto destino(centro.x+raio, centro.y+raio);
    Ponto origem(centro.x-raio, centro.y-raio);
    return Retangulo(origem, destino);
}

Vertice* MenorVertice(Poligono &p, int (*cmp)(Ponto*, Ponto*)){
    Vertice* aux = p.GetVertice();
    p.Avancar(HORARIO);
    for(unsigned i = 1; i < p.GetSize(); p.Avancar(HORARIO), i++){
        if((*cmp)(p.GetVertice(), aux) < 0)
            aux = p.GetVertice();
    }
    p.SetV(aux);
    return aux;
}

int EsquerdaDireita(Ponto* a, Ponto* b){
    if(*a < *b) return -1;
    if(*a > *b) return 1;
    return 0;
}

int DireitaEsquerda(Ponto* a, Ponto* b){
    return EsquerdaDireita(b, a);
}

}// NAMESPACE SPATIAL DATA
