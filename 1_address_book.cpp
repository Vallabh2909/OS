#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Contact {
public:
    string name;
    string phone;
    string email;

    // Constructor to initialize a contact
    Contact(string n, string p, string e) {
        name = n;
        phone = p;
        email = e;
    }
};

class AddressBook {
private:
    vector<Contact> contacts;

public:
    // Function to create an address book (already handled by the constructor)
    void createAddressBook() {
        cout << "Address book created successfully." << endl;
    }

    // Function to view all contacts in the address book
    void viewAddressBook() {
        if (contacts.empty()) {
            cout << "Address book is empty." << endl;
        } else {
            cout << "Contacts in the address book: " << endl;
            for (size_t i = 0; i < contacts.size(); ++i) {
                cout << i + 1 << ". Name: " << contacts[i].name << ", Phone: " << contacts[i].phone << ", Email: " << contacts[i].email << endl;
            }
        }
    }

    // Function to insert a new contact
    void insertRecord() {
        string name, phone, email;
        cout << "Enter Name: ";
        cin.ignore(); // To ignore the leftover newline character in the buffer
        getline(cin, name);
        cout << "Enter Phone Number: ";
        getline(cin, phone);
        cout << "Enter Email Address: ";
        getline(cin, email);
        
        Contact newContact(name, phone, email);
        contacts.push_back(newContact);
        cout << "New contact added successfully." << endl;
    }

    // Function to delete a contact by index
    void deleteRecord() {
        int index;
        cout << "Enter the index of the contact to delete (1-based index): ";
        cin >> index;

        if (index < 1 || index > contacts.size()) {
            cout << "Invalid index!" << endl;
        } else {
            contacts.erase(contacts.begin() + index - 1);
            cout << "Contact deleted successfully." << endl;
        }
    }

    // Function to modify a contact's details
    void modifyRecord() {
        int index;
        cout << "Enter the index of the contact to modify (1-based index): ";
        cin >> index;

        if (index < 1 || index > contacts.size()) {
            cout << "Invalid index!" << endl;
        } else {
            string name, phone, email;
            cout << "Enter new Name: ";
            cin.ignore(); // To ignore the leftover newline character in the buffer
            getline(cin, name);
            cout << "Enter new Phone Number: ";
            getline(cin, phone);
            cout << "Enter new Email Address: ";
            getline(cin, email);

            contacts[index - 1].name = name;
            contacts[index - 1].phone = phone;
            contacts[index - 1].email = email;
            cout << "Contact modified successfully." << endl;
        }
    }

    // Function to exit the program
    void exit() {
        cout << "Exiting address book program." << endl;
    }
};

int main() {
    AddressBook addressBook;
    int choice;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Create Address Book\n";
        cout << "2. View Address Book\n";
        cout << "3. Insert Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Modify Record\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addressBook.createAddressBook();
                break;
            case 2:
                addressBook.viewAddressBook();
                break;
            case 3:
                addressBook.insertRecord();
                break;
            case 4:
                addressBook.deleteRecord();
                break;
            case 5:
                addressBook.modifyRecord();
                break;
            case 6:
                addressBook.exit();
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
