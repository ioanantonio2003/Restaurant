#include <iostream>
#include <string>
#include<vector>
#include<algorithm>

class Produs{

private:
    std::string nume_produs;
    int calorii;
    int pret;

public:
    Produs() : nume_produs(""), calorii(0), pret(0){} //constructor de initializare

    Produs(const std::string& numele, int caloriile, int pretul) : nume_produs(numele), calorii(caloriile),pret(pretul) {} //constructor supraincarcat

    bool operator==(const Produs& produs) const{ //operator == care ne ajuta la stergerea unui elment de tip Produs din meniul clasei Restaurant
        return(this->nume_produs == produs.nume_produs && this->calorii == produs.calorii && this->pret == produs.pret);
    }

    const std::string &getNumeProdus() const { //getter pt nume
        return nume_produs;
    }

    int getCalorii() const { //getter pt calorii
        return calorii;
    }

    int getPret() const { //getter pt pret
        return pret;
    }

    ~Produs() {} //destructor

    friend std::istream& operator>>(std::istream& in,Produs& produs){ //operator>> pentru citire
        std::cout<<"Nume produs:"<<std::endl;
        in.ignore();
        std::getline(in,produs.nume_produs);
        std::cout<<std::endl;
        std::cout<<"Calorii:"<<std::endl;
        in>> produs.calorii;
        std::cout<<"Pret:"<<std::endl;
        in>>produs.pret;
        return in;
    }
};

class Restaurant{
private:
    static int m;
    std::string nume_restaurnat; //numele restaurantului
    int an_finantare;
    std::vector<Produs>meniu; //pt a folosi agregarea meniu va contine produse din clasa Produs

public:
    Restaurant() : nume_restaurnat(""), an_finantare(0){} // constructor de initializare

    Restaurant(const std::string& numele, int an) : nume_restaurnat(numele), an_finantare(an){} // constructor supraincarcat

    Restaurant(const Restaurant& alt) : nume_restaurnat(alt.nume_restaurnat), an_finantare(alt.an_finantare), meniu(alt.meniu){} // constructor de copiere

    Restaurant& operator=(const Restaurant& restaurant){ //operator= de copiere
        if(this != &restaurant){
            nume_restaurnat = restaurant.nume_restaurnat;
            an_finantare = restaurant.an_finantare;
            meniu = restaurant.meniu;
        }
        return *this;
    }

    ~Restaurant() {} // destructor

    void adaugaProdus(const Produs& produs){ // adauga un produs in meniu
        meniu.push_back(produs);
        incrementMax();
    }

    void stergeProdus(const Produs& produs){ // stergem un element specificat din meniu
        for (auto elem = meniu.begin(); elem != meniu.end(); ++elem) {
            if (*elem == produs) {
                meniu.erase(elem);
                return;
            }
        }
        std::cout<<"Nu exista acest produs"<<std::endl;
    }


    friend std::ostream& operator<<(std::ostream& out, const Restaurant& restaurant){ //operator<< pentru afisare
        out<<"Nume restaurant : "<<restaurant.nume_restaurnat<<std::endl;
        out<<"An finantare : "<<restaurant.an_finantare<<std::endl;
        out<<"Meniul : ";
        for(int i = 0; i < m;i++){
            out<<restaurant.meniu[i].getNumeProdus()<<" ,";
        }
        std::cout<<std::endl;
        return out;
    }

    friend std::istream& operator>>(std::istream& in,Restaurant& restaurant){ //operator>> pentru citire
        std::cout<<"Nume restaurant:"<<std::endl;
        in.ignore();
        std::getline(in,restaurant.nume_restaurnat);
        std::cout<<"An finantare:"<<std::endl;
        in>> restaurant.an_finantare;
        return in;
    }

    Restaurant& operator+(const Restaurant& restaurant){//operator supraincarcat ca functie membra
        this->nume_restaurnat += restaurant.nume_restaurnat;
        this->an_finantare = restaurant.an_finantare;
        this->meniu.insert(this->meniu.end(),restaurant.meniu.begin(),restaurant.meniu.end());
        return *this;
    }

    friend Restaurant operator-(const Restaurant& res1, int nr);//operator supraincarcat ca functie non-membra

    void creareMeniu(){ // functie complexa
        std::cout<<"Buna ziua , va vom crea un meniu personaliza"<<std::endl;
        std::cout<<"Va rog introduceti bugetul dvs : ";
        int buget;
        std::cin>>buget;
        int calorii;
        std::cout<<"Care este numarul maxim de calorii pe care vreti sa il mancati :";
        std::cin>>calorii;
        std::vector<Produs>menu = meniu;
        std::sort(menu.begin(), menu.end(),[](const Produs& pr1, const Produs& pr2){
            return pr1.getCalorii() > pr2.getCalorii();
        });
        std::vector<Produs>alegere;
        std::cout<<"Va rog raspundeti cu 1 pt DA SAU 2 pt NU"<<std::endl;
        for(const auto& produs:menu) {
            if (buget > 0) {}
            std::cout << "Vreti sa adaugam : " << produs.getNumeProdus()<<" ?"<<std::endl;
            int i;
            std::cin >> i;
            if (i == 1) {
                alegere.push_back(produs);
                buget = buget - produs.getPret();
            }
        }
        std::cout<<"Meniul dvs arata asa : ";
        int i = 1;
        for(const auto& produs : alegere){
            std::cout<<i<<"."<<produs.getNumeProdus()<<std::endl;
        }
    }

    const std::string &getNumeRestaurnat() const;


    int getAnFinantare() const;


    void average() const{ //calculcare cat costa un produs (average price)
        double suma = 0.0;
        for(const Produs& produs : meniu){
            suma = suma + produs.getPret();
        }
        if(meniu.size() > 0){
            std::cout<< suma / meniu.size();
        }
        std::cout<< 0.0;
    }

    void sortare_calorii(){
        std::sort(meniu.begin(),meniu.end(), [](const Produs& a,const Produs&b){
            return a.getCalorii() < b.getCalorii();
        });
    }

    void afisare_meniu() const{
        std::cout<<"Meniul arata in felul urmator : "<<std::endl;
        int i = 1;
        for(const Produs& produs : meniu){
            std::cout<<i<<"."<<produs.getNumeProdus()<<":"<<produs.getPret()<<"lei :"<<produs.getCalorii()<<"calorii"<<std::endl;
            i++;
        }
    }

    static void incrementMax() {
        m++;
    }
};
int Restaurant::m = 0;

Restaurant operator-(const Restaurant& res1, int nr){//operator supraincarcat ca functie non-membra
    return Restaurant(res1.getNumeRestaurnat(),res1.getAnFinantare() - nr);
}

const std::string &Restaurant::getNumeRestaurnat() const {
    return nume_restaurnat;
}


int Restaurant::getAnFinantare() const {
    return an_finantare;
}


void afisare_meniu(){
    std::cout<<"0.Exit"<<std::endl;
    std::cout<<"1.Vizualizare restaurante"<<std::endl;
    std::cout<<"2.Adauga restaurant"<<std::endl;
    std::cout<<"3.Modifica restaurant"<<std::endl;
    std::cout<<"4.Intrare Restaurant"<<std::endl;
    std::cout<<"Alegere: ";
}

void modifica_restaurant(){
    std::cout<<"0.Exit"<<std::endl;
    std::cout<<"1.Adauga produs"<<std::endl;
    std::cout<<"2.Sterge produs"<<std::endl;
    std::cout<<"Alegere:"<<std::endl;
}

void prelucrare_restaurant(){
    std::cout<<"0.Exit"<<std::endl;
    std::cout<<"1.Vizualizare meniu"<<std::endl;
    std::cout<<"2.Pret average/per produs"<<std::endl;
    std::cout<<"3.Sortare meniu in functie de calorii"<<std::endl;
    std::cout<<"4.Creare meniu"<<std::endl;
    std::cout<<"Alegere:";

}

int main() {
    std::vector<Restaurant>restaurante;
    int size = 0;
    int choice;
    bool display_menu = true;
    while (display_menu) {
        afisare_meniu();
        std::cin >> choice;
        switch (choice) {
            case 0 : {
                display_menu = false;
                break;
            }
            case 1 : {
                if(restaurante.size() == 0){
                    std::cout<<"Nu exista restaurante"<<std::endl;
                }
                int m = 0;
                for(auto& restaurant : restaurante){
                    std::cout<<m<<"."<<restaurant;
                    m++;
                }
                break;
            }
            case 2 : {
                Restaurant res;
                std::cin>>res;
                restaurante.push_back(res);
                size = size + 1;
                break;
            }
            case 3 : {
                std::cout<<"Alege nr restaurantului pe care vrei sa l modifici:";
                int c;
                std::cin>>c;
                if(c>=size){
                    break;
                }
                bool t = true;
                while(t){
                    modifica_restaurant();
                    int i;
                    std::cin>>i;
                    switch (i) {
                        case 0 : {
                            t = false;
                            break;
                        }
                        case 1 : {
                            Produs produs;
                            std::cin>>produs;
                            restaurante[c].adaugaProdus(produs);
                            break;
                        }
                        case 2 : {
                            Produs produs;
                            std::cin>>produs;
                            restaurante[c].stergeProdus(produs);
                            break;
                        }
                    }
                }
                break;
            }
            case 4 : {
                std::cout<<"Alege nr restaurantului pe care vrei sa l modifici:";
                int c;
                std::cin>>c;
                if(c>=size){
                    break;
                }
                bool b = true;
                while(b){
                    prelucrare_restaurant();
                    int j;
                    std::cin>>j;
                    switch (j) {
                        case 0 : {
                            b = false;
                            break;
                        }
                        case 1 : {
                            restaurante[c].afisare_meniu();
                            break;
                        }
                        case 2 : {
                            restaurante[c].average();
                            break;
                        }
                        case 3 : {
                            restaurante[c].sortare_calorii();
                            break;
                        }
                        case 4 : {
                            restaurante[c].creareMeniu();
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
