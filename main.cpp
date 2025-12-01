#include <iostream>
#include <string>
#include <sstream>
#include "InventorySystem.h"

using namespace std;

// Global inventory system
InventorySystem inventory;

void printHelp()
{
    cout << "Supported list of commands: " << endl;
    cout << " 1. find <inventoryid> - Finds if the inventory exists. If exists, prints details. If not, prints 'Inventory not found'." << endl;
    cout << " 2. listInventory <category_string> - Lists just the id and name of all inventory belonging to the specified category. If the category doesn't exists, prints 'Invalid Category'.\n"
         << endl;
    cout << " Use :quit to quit the REPL" << endl;
}

bool validCommand(string line)
{
    return (line == ":help") ||
           (line.rfind("find", 0) == 0) ||
           (line.rfind("listInventory", 0) == 0);
}

void evalCommand(string line)
{
    if (line == ":help")
    {
        printHelp();
    }
    else if (line.rfind("find", 0) == 0)
    {
        stringstream ss(line);
        string cmd, id;
        ss >> cmd >> id;

        if (id.empty())
        {
            cout << "Usage: find <inventoryid>" << endl;
            return;
        }

        const Product* p = inventory.findProduct(id);
        if (!p)
        {
            cout << "Inventory not found" << endl;
        }
        else
        {
            cout << *p;
        }
    }
    else if (line.rfind("listInventory", 0) == 0)
    {
        string category = line.substr(14); // remove "listInventory "

        if (!category.empty() && category[0] == ' ')
            category.erase(0, 1);

        if (category.empty())
        {
            cout << "Usage: listInventory <category>" << endl;
            return;
        }

        inventory.listInventory(category);
    }
}

void bootStrap()
{
    cout << "\n Welcome to Amazon Inventory Query System" << endl;
    cout << " enter :quit to exit. or :help to list supported commands." << endl;

    // --------- Load CSV HERE ----------
    cout << "\nLoading dataset..." << endl;

    // Use your exact filename:
    string filename = "marketing_sample_for_amazon_com-ecommerce__20200101_20200131__10k_data-1.csv";

    if (!inventory.loadCSV(filename))
    {
        cout << "Failed to load CSV. Check filename." << endl;
    }
    else
    {
        cout << "Inventory loaded successfully!" << endl;
    }

    // ------------------------------------

    cout << "\n> ";
}

int main(int argc, char const *argv[])
{
    string line;
    bootStrap();

    while (getline(cin, line) && line != ":quit")
    {
        if (validCommand(line))
        {
            evalCommand(line);
        }
        else
        {
            cout << "Command not supported. Enter :help for list of supported commands" << endl;
        }
        cout << "> ";
    }
    return 0;
}
