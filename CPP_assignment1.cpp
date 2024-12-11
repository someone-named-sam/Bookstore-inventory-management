#include <iostream>
#include <string>
#include <vector>

#define pb  push_back 
using namespace std;

// Class for a Book- it has attributes title, author, isbn id, price as well as quantity 
class Book {
private:
    string title;
    string auth;
    string isbn;
    double price;
    int qty;

public:
    // Constructor
    Book(string t, string a, string i, double p, int q){
        title=t;
        auth=a;
        isbn=i;
        price=p;
        qty=q;
    }
    //methods to access the private variables
    string getTitle() const{
        return title;
    }
    string getISBN() const{ 
        return isbn;
    }
    double getPrice() const{ 
        return price;
    }
    int getQuantity() const{
        return qty;
    }

    // Update stock
    void addStock(int amount){
        qty += amount;
    }
    void sellBook(int amount){
        if(qty>= amount){
            qty-= amount;
        }
        else{
            cout <<"Not enough stock available."<<endl;
        }
    }

    // Display details
    virtual void display() const{
        cout<<"\t"<<title <<"\t"<<auth <<"\t"<<isbn <<"\t"<<price <<"\t"<<qty <<"\t"<<"N/A"<<endl;
    }
};

// Class for a Magazine- it has all the same attributes as a book but doesnt have an author, instead it has publish date to indicate the edition
class Magazine : public Book{
private:
    string pDate;

public:
    // Constructor
    Magazine(string t, string i, double p, string pubDate, int q)
        : Book(t, "N/A", i, p, q), pDate(pubDate) {}

    string getpDate() const { 
        return pDate; }

    // Display details (override)
    void display() const override {
        cout<<"\t"<<getTitle() <<"\t"<<"N/A" <<"\t"<<getISBN() <<"\t"<<getPrice() <<"\t"<<getQuantity() <<"\t"<<pDate <<endl;
    }
};

// Class for the Inventory- it consists of all the books
class Inventory {
private:
    vector<Book*> items;

public:
    void addItem(Book *item){
        items.pb(item);
    }

    Book *searchbyISBN(const string &isbn){
        for (auto &item : items){
            if(item->getISBN()== isbn){
                return item;
            }
        }
        return nullptr;
    }

    // Display all items
    void displayInventory() const{
        cout<<"\t"<<"Title" <<"\t"<<"Author" <<"\t"<<"ISBN" <<"\t"<<"Price" <<"\t"<<"Stock" <<"\t"<<"Publication Date" <<endl;
        for (const auto &item : items){
            item->display();
        }
    }
};

// Class for the StoreManager
class StoreManager {
public:
    void restockItem(Inventory &ivt, const string &isbn, int q){
        Book *item = ivt.searchbyISBN(isbn);
        if(item){
            item->addStock(q);
            cout<<"Restocked successfully."<<endl;
        }
        else{
            cout<<"Item not found ."<<endl;
        }
    }

    void addNewItem(Inventory &ivt, Book *item) {
        ivt.addItem(item);
        cout<<"New item added successfully."<<endl;;
    }
};

// Class for the Cashier
class Cashier {
public:
    void sellItem(Inventory &ivt, const string &isbn, int q) {
        Book *item = ivt.searchbyISBN(isbn);
        if(item){
            item->sellBook(q);
            cout<<"sale successful.\n";
        }
        else{
            cout << "Item not found\n";
        }
    }
};

// Customer class to track customer's purchases and history
class Customer{
private:
    string customerID;
    string name;
    vector<string> History;

public:
    Customer(string id, string nm){
        customerID=id;
        name=nm;
    } 

    void addPurchase(const string &isbn) {
        History.pb(isbn);
    }

    void displayHistory() const{
        if(History.size()==0){
            cout<<name <<" has no purchase history." <<endl;
        } 
        else{
            cout<<"Purchase history for " <<name <<": " <<endl;
            for(const string &isbn :History){
                cout<<isbn <<endl;
            }
        }
    }

    string getCustomerID() const{
        return customerID;
    }

    string getName() const{
        return name;
    }
};
//Global customers vector to track all customers
vector<Customer> customers; 

Customer *findCustomer(const string &id){
    for (auto &customer : customers){
        if (customer.getCustomerID()== id){
            return &customer;
        }
    }
    return nullptr;
}

void addCustomer(){
    string id, name;
    cout<<"Enter Customer ID: ";
    cin>>id;
    cin.ignore();
    cout<<"Enter Customer Name: ";
    getline(cin, name);

    customers.pb(Customer(id, name));
    cout<<"Customer added successfully!" <<endl;
}

int main() {
    Inventory ivt;
    StoreManager mg;
    Cashier ca;

    while (true) {
        cout<<"\n--- Inventory Management System ---\n";
        cout<<"1. Add Customer\n";
        cout<<"2. Sell Item to Customer\n";
        cout<<"3. Show Purchase history\n";
        cout<<"4. Add Book/ Magazine\n";
        cout<<"5. Display Inventory\n";
        cout<<"6. Exit\n";
        cout<<"Enter your choice: ";
        int choice;
        cin>> choice;

        if (choice == 6){
            cout<<"Exiting the program.\n";
            break;
        }

        switch (choice) {
        case 1:
            addCustomer();
            break;
        case 2: {
            string customerID;
            cout<<"Enter Customer ID for purchase: ";
            cin>>customerID;
            string isbn;
            cout << "Enter ISBN of item to sell: ";
            cin >> isbn;
            
            Customer *customer = findCustomer(customerID);
            if(customer){
                ca.sellItem(ivt, isbn, 1);
                customer->addPurchase(isbn);

                cout<<"Purchase complete" <<customer->getName() <<endl;
            } 
            else {
                cout << "Customer ID not found." <<endl;
            }
            break;
        }
        case 3:{
            string customerID;
            cout<<"Enter Customer ID for purchase: ";
            cin>>customerID;
            Customer *customer = findCustomer(customerID);
            if(customer){
                customer->displayHistory();
            } 
            else {
                cout << "Customer ID not found. Please add the customer first." << endl;
            }
            break;
        }
        case 4: {
            int c;
            cout<<"1. Book, 2. Magazine\n";
            cin>>c;
            string title, author, isbn, date;
            double price;
            int q;

            cout << "Enter title: ";
            cin.ignore();
            getline(cin, title);
            
            if(c==1){
                cout<< "Enter author name: ";
                getline(cin, author);
            }
            cout<< "Enter ISBN: ";
            cin>> isbn;
            cout<< "Enter price: ";
            cin>> price;
            cout<< "Enter quantity: ";
            cin>> q;
            if(c==2){
                cout<< "Enter date: ";
                cin>> date;
            }
            if(c==2){
                mg.addNewItem(ivt, new Magazine(title, isbn, price, date, q));
            }
            else if(c==1){
                mg.addNewItem(ivt, new Book(title, author, isbn, price, q));
            }
            else{
                cout<<"Enter a valid choice";
            }
            break;
        }
        case 5:
            ivt.displayInventory();
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}