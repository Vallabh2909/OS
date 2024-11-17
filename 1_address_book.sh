#!/bin/bash

ADDRESS_BOOK="address_book.txt"

# Function to display the menu
show_menu() {
    echo "Address Book Options:"
    echo "a) Create address book"
    echo "b) View address book"
    echo "c) Insert a record"
    echo "d) Delete a record"
    echo "e) Modify a record"
    echo "f) Exit"
    echo "Enter your choice: "
}

# Function to create the address book
create_address_book() {
    > "$ADDRESS_BOOK"
    echo "Address book created successfully!"
}

# Function to view the address book
view_address_book() {
    if [ -s "$ADDRESS_BOOK" ]; then
        echo "Address Book Records:"
        cat "$ADDRESS_BOOK"
    else
        echo "Address book is empty!"
    fi
}

# Function to insert a record
insert_record() {
    echo "Enter name:"
    read name
    echo "Enter phone number:"
    read phone
    echo "Enter email:"
    read email
    echo "$name | $phone | $email" >> "$ADDRESS_BOOK"
    echo "Record added successfully!"
}

# Function to delete a record
delete_record() {
    echo "Enter the name of the record to delete:"
    read name
    if grep -q "^$name |" "$ADDRESS_BOOK"; then
        grep -v "^$name |" "$ADDRESS_BOOK" > temp.txt && mv temp.txt "$ADDRESS_BOOK"
        echo "Record deleted successfully!"
    else
        echo "Record not found!"
    fi
}

# Function to modify a record
modify_record() {
    echo "Enter the name of the record to modify:"
    read name
    if grep -q "^$name |" "$ADDRESS_BOOK"; then
        grep -v "^$name |" "$ADDRESS_BOOK" > temp.txt && mv temp.txt "$ADDRESS_BOOK"
        echo "Enter new name:"
        read new_name
        echo "Enter new phone number:"
        read new_phone
        echo "Enter new email:"
        read new_email
        echo "$new_name | $new_phone | $new_email" >> "$ADDRESS_BOOK"
        echo "Record modified successfully!"
    else
        echo "Record not found!"
    fi
}

# Main loop
while true; do
    show_menu
    read choice
    case $choice in
        a) create_address_book ;;
        b) view_address_book ;;
        c) insert_record ;;
        d) delete_record ;;
        e) modify_record ;;
        f) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid option! Please try again." ;;
    esac
done
