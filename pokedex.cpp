#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Pokemon {
    int numero;
    string nombre, tipo1, tipo2;
    int total, hp, ataque, defensa, ataqueEsp, defensaEsp, velocidad, generacion;
    bool legendario;
};

vector<Pokemon> cargarPokemons(string ruta) {
    vector<Pokemon> lista;
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cout << "No se pudo abrir " << ruta << endl;
        return lista;
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;

        stringstream ss(linea);
        string campo;
        Pokemon p;

        getline(ss, campo, ','); p.numero = stoi(campo);
        getline(ss, p.nombre, ',');
        getline(ss, p.tipo1, ',');
        getline(ss, p.tipo2, ',');
        getline(ss, campo, ','); p.total = stoi(campo);
        getline(ss, campo, ','); p.hp = stoi(campo);
        getline(ss, campo, ','); p.ataque = stoi(campo);
        getline(ss, campo, ','); p.defensa = stoi(campo);
        getline(ss, campo, ','); p.ataqueEsp = stoi(campo);
        getline(ss, campo, ','); p.defensaEsp = stoi(campo);
        getline(ss, campo, ','); p.velocidad = stoi(campo);
        getline(ss, campo, ','); p.generacion = stoi(campo);
        getline(ss, campo, ','); p.legendario = (campo == "True");

        lista.push_back(p);
    }
    return lista;
}

string aMinusculas(string texto) {
    for (char &c : texto) c = tolower(c);
    return texto;
}

void mostrarPokemon(const Pokemon &p) {
    cout << "\n----------------------------------------" << endl;
    cout << "#" << p.numero << " " << p.nombre << endl;
    cout << "Tipo: " << p.tipo1;
    if (p.tipo2 != "") cout << " / " << p.tipo2;
    cout << endl;
    cout << "HP: " << p.hp << endl;
    cout << "Ataque: " << p.ataque << endl;
    cout << "Defensa: " << p.defensa << endl;
    cout << "Ataque Esp.: " << p.ataqueEsp << endl;
    cout << "Defensa Esp.: " << p.defensaEsp << endl;
    cout << "Velocidad: " << p.velocidad << endl;
    cout << "Total: " << p.total << endl;
    cout << "Generacion: " << p.generacion << endl;
    cout << "Legendario: " << (p.legendario ? "Si" : "No") << endl;
    cout << "----------------------------------------" << endl;
}

void verPorNumero(const vector<Pokemon> &lista) {
    int numero;
    cout << "Numero de pokedex (1-150): ";
    cin >> numero;
    cin.ignore();

    for (const Pokemon &p : lista) {
        if (p.numero == numero) {
            mostrarPokemon(p);
            return;
        }
    }
    cout << "No existe un pokemon con ese numero." << endl;
}

void buscarPorNombre(const vector<Pokemon> &lista) {
    string texto;
    cout << "Escribe el nombre o parte del nombre: ";
    getline(cin, texto);
    texto = aMinusculas(texto);

    int encontrados = 0;
    for (const Pokemon &p : lista) {
        if (aMinusculas(p.nombre).find(texto) != string::npos) {
            cout << "#" << p.numero << " " << p.nombre << endl;
            encontrados++;
        }
    }

    if (encontrados == 0) cout << "No se encontro ningun pokemon." << endl;
    else cout << "Encontrados: " << encontrados << endl;
}

void verPorTipo(const vector<Pokemon> &lista) {
    vector<string> tipos;

    for (const Pokemon &p : lista) {
        if (find(tipos.begin(), tipos.end(), p.tipo1) == tipos.end())
            tipos.push_back(p.tipo1);
    }
    for (const Pokemon &p : lista) {
        if (p.tipo2 != "" && find(tipos.begin(), tipos.end(), p.tipo2) == tipos.end())
            tipos.push_back(p.tipo2);
    }

    cout << "\nTipos disponibles:" << endl;
    for (int i = 0; i < (int)tipos.size(); i++) {
        cout << i + 1 << ". " << tipos[i] << endl;
    }

    int opcion;
    cout << "Elige un tipo: ";
    cin >> opcion;
    cin.ignore();

    if (opcion < 1 || opcion > (int)tipos.size()) {
        cout << "Opcion invalida." << endl;
        return;
    }

    string elegido = tipos[opcion - 1];
    int contador = 0;
    cout << "\nPokemon de tipo " << elegido << ":" << endl;
    for (const Pokemon &p : lista) {
        if (p.tipo1 == elegido || p.tipo2 == elegido) {
            cout << "#" << p.numero << " " << p.nombre << endl;
            contador++;
        }
    }
    cout << "Total: " << contador << endl;
}

void verInfoPorNombre(const vector<Pokemon> &lista) {
    string nombre;
    cout << "Nombre exacto del pokemon: ";
    getline(cin, nombre);

    for (const Pokemon &p : lista) {
        if (aMinusculas(p.nombre) == aMinusculas(nombre)) {
            mostrarPokemon(p);
            return;
        }
    }
    cout << "No existe un pokemon con ese nombre." << endl;
}

bool esMasFuerte(const Pokemon &a, const Pokemon &b) {
    return a.total > b.total;
}

void verTop(vector<Pokemon> lista) {
    sort(lista.begin(), lista.end(), esMasFuerte);

    cout << "\n===== TOP 3 MAS FUERTES =====" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i + 1 << ". " << lista[i].nombre << " (Total: " << lista[i].total << ")" << endl;
    }

    cout << "\n===== TOP 3 MAS DEBILES =====" << endl;
    for (int i = 0; i < 3; i++) {
        const Pokemon &p = lista[lista.size() - 1 - i];
        cout << i + 1 << ". " << p.nombre << " (Total: " << p.total << ")" << endl;
    }
}

int main() {
    vector<Pokemon> pokemons = cargarPokemons("pokemon_150_registros.csv");
    if (pokemons.empty()) return 1;

    int opcion;
    do {
        cout << "\n========== POKEDEX ==========" << endl;
        cout << "1. Ver info de pokemon por numero de pokedex" << endl;
        cout << "2. Buscar pokemon por nombre" << endl;
        cout << "3. Ver pokemon de un tipo" << endl;
        cout << "4. Ver info de pokemon por nombre" << endl;
        cout << "5. Ver top 3 mas fuertes y mas debiles" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = -1;
        } else {
            cin.ignore(10000, '\n');
        }

        switch (opcion) {
            case 1: verPorNumero(pokemons); break;
            case 2: buscarPorNombre(pokemons); break;
            case 3: verPorTipo(pokemons); break;
            case 4: verInfoPorNombre(pokemons); break;
            case 5: verTop(pokemons); break;
            case 0: cout << "Hasta luego!" << endl; break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);

    return 0;
}
