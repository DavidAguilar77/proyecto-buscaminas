#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Estructura para representar un producto
struct Producto {
    int id;
    string nombre;
    double precio;
    int stock;
};

// Función para mostrar el menú principal
void mostrarMenu() {
    cout << "\n===============================\n";
    cout << "   SISTEMA DE TIENDA VIRTUAL   \n";
    cout << "===============================\n";
    cout << "1. Ver inventario de productos\n";
    cout << "2. Agregar nuevo producto\n";
    cout << "3. Realizar una compra (Venta)\n";
    cout << "4. Salir\n";
    cout << "Seleccione una opcion (1-4): ";
}

int main() {
    // Lista inicial de productos en la tienda
    vector<Producto> inventario = {
        {1, "Laptop HP", 750.00, 5},
        {2, "Mouse inalambrico", 25.50, 15},
        {3, "Teclado mecanico", 45.00, 10},
        {4, "Monitor 24 pulgadas", 180.00, 7}
    };

    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        // Limpiar el buffer de entrada para evitar errores con strings
        cin.ignore();

        switch (opcion) {
            case 1: {
                cout << "\n--- INVENTARIO ACTUAL ---\n";
                cout << left << setw(5) << "ID" 
                     << setw(22) << "Producto" 
                     << setw(10) << "Precio" 
                     << setw(8) << "Stock" << endl;
                cout << "-------------------------------------------\n";
                
                for (const auto& p : inventario) {
                    cout << left << setw(5) << p.id 
                         << setw(22) << p.nombre 
                         << "$" << setw(9) << fixed << setprecision(2) << p.precio 
                         << setw(8) << p.stock << endl;
                }
                break;
            }
            case 2: {
                Producto nuevoP;
                nuevoP.id = inventario.size() + 1;
                
                cout << "\nIngrese el nombre del producto: ";
                getline(cin, nuevoP.nombre);
                
                cout << "Ingrese el precio ($): ";
                cin >> nuevoP.precio;
                
                cout << "Ingrese la cantidad en stock: ";
                cin >> nuevoP.stock;

                inventario.push_back(nuevoP);
                cout << "\n¡Producto agregado exitosamente con ID " << nuevoP.id << "!\n";
                break;
            }
            case 3: {
                int idCompra, cantidad;
                cout << "\n--- REALIZAR COMPRA ---\n";
                cout << "Ingrese el ID del producto que desea comprar: ";
                cin >> idCompra;

                bool encontrado = false;
                for (auto& p : inventario) {
                    if (p.id == idCompra) {
                        encontrado = true;
                        cout << "Producto seleccionado: " << p.nombre << " (Disponible: " << p.stock << ")\n";
                        cout << "Ingrese la cantidad que desea llevar: ";
                        cin >> cantidad;

                        if (cantidad > 0 && cantidad <= p.stock) {
                            p.stock -= cantidad; // Descuenta del inventario
                            double total = cantidad * p.precio;
                            cout << fixed << setprecision(2);
                            cout << "\n-----------------------------------\n";
                            cout << "¡Compra realizada con exito!\n";
                            cout << "Total a pagar: $" << total << "\n";
                            cout << "-----------------------------------\n";
                        } else {
                            cout << "\n[Error] Cantidad no valida o stock insuficiente.\n";
                        }
                        break;
                    }
                }
                if (!encontrado) {
                    cout << "\n[Error] No existe ningun producto con el ID " << idCompra << ".\n";
                }
                break;
            }
            case 4:
                cout << "\nSaliendo del sistema. ¡Hasta luego!\n";
                break;
            default:
                cout << "\n[Error] Opcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 4);

    return 0;
}