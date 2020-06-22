#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Item {
  int id;
  string name;
  string brand;
  int price;
  int stock;
};

struct Purchase {
  int total_price;
  vector<int> qtys;
  vector<Item> items;
};

vector<Item> load_items_from_file();
void save_items_to_file(vector<Item>);

string item_to_string(Item, char delimiter = ' ');
Item create_null_item();
bool is_null_item(Item);
Item create_item_from_user();
int get_next_item_id(vector<Item>);
Item find_item_by_id(vector<Item>);

void show_items(vector<Item>);
void insert_item(vector<Item>*, Item);
void update_item(vector<Item>*, int, Item);
void delete_item(vector<Item>*, int);

Purchase purchase_items(vector<Item>*);
void show_invoice(Purchase);

void menu_main(vector<Item>*);
void menu_item_management(vector<Item>*);
void menu_show_items(vector<Item>*);
string menu_insert_item(vector<Item>*);
string menu_update_item(vector<Item>*);
string menu_delete_item(vector<Item>*);
void menu_purchasing(vector<Item>*);

int main() {
  vector<Item> items = load_items_from_file();

  menu_main(&items);

  save_items_to_file(items);

  return 0;
}

vector<Item> load_items_from_file() {
  vector<Item> loaded_items;
  ifstream file_reader("items.txt");
  string item_row;

  while (getline(file_reader, item_row)) {
    stringstream cell_reader(item_row);
    string id_str, price_str, stock_str;
    Item item;

    getline(cell_reader, id_str, ';');
    getline(cell_reader, item.name, ';');
    getline(cell_reader, item.brand, ';');
    getline(cell_reader, price_str, ';');
    getline(cell_reader, stock_str, ';');

    item.id = stoi(id_str);
    item.price = stoi(price_str);
    item.stock = stoi(stock_str);

    loaded_items.push_back(item);
  }

  file_reader.close();

  return loaded_items;
}

void save_items_to_file(vector<Item> items) {
  ofstream file_writer("items.txt");
  
  for (Item item : items) {
    string item_row = item_to_string(item, ';');
    file_writer << item_row << endl;
  }

  file_writer.close();
}

string item_to_string(Item item, char delimiter) {
  return to_string(item.id) + delimiter + item.name + delimiter + item.brand + delimiter + to_string(item.price) + delimiter + to_string(item.stock);
}

Item create_null_item() {
  return {0, "", "", 0, 0};
}

bool is_null_item(Item item) {
  return item.id == 0;
}

Item create_item_from_user() {
  Item item;
  cout << "Nama: ";
  getline(cin, item.name);
  cout << "Brand: ";
  getline(cin, item.brand);
  cout << "Harga Satuan: ";
  cin >> item.price;
  cout << "Stok: ";
  cin >> item.stock;

  return item;
}

int get_next_item_id(vector<Item> items) {
  int latest_id = 1;
  for (Item item : items) {
    latest_id = max(latest_id, item.id);
  }
  return latest_id + 1;
}

Item find_item_by_id(vector<Item> items) {
  Item found_item = create_null_item();
  int id;

  while (true) {
    cout << "Pilih Item berdasarkan ID: ";
    cin >> id;

    for (Item item : items) {
      if (item.id == id) {
        found_item = item;
      }
    }

    if (is_null_item(found_item)) {
      cout << "Item dengan ID " << id << " tidak ditemukan. Coba lagi." << endl;
    } else {
      break;
    }
  }
  
  return found_item;
}

void show_items(vector<Item> items) {
  for (Item item : items) {
    cout << item_to_string(item) << endl;
  }
}

void insert_item(vector<Item>* items, Item new_item) {
  int new_id = get_next_item_id(*items);
  new_item.id = new_id;
  items->push_back(new_item);
}

void update_item(vector<Item>* items, int id, Item new_item_data) {
  for (Item& item : *items) {
    if (item.id == id) {
      item = new_item_data;
      item.id = id;
    }
  }
}

void delete_item(vector<Item>* items, int id) {
  int index = -1;
  for (int i = 0; i < items->size(); i++) {
    if (items->at(i).id == id) {
      index = i;
    }
  }

  if (index >= 0) {
    items->erase(items->begin() + index);
  }
}

Purchase purchase_items(vector<Item>* items) {
  Purchase purchase{0, {}, {}};
  string message = "";
  Item item;
  int qty, id;

  while (true) {
    item = create_null_item();

    system("clear");
    cout << "Total: " << purchase.total_price << endl;
    cout << "------------------------" << endl;

    if (message != "") {
      cout << message << endl;
      cout << "------------------------" << endl;
      message = "";
    }

    for (int i = 0; i < purchase.items.size(); i++) {
      cout << purchase.items[i].name << " x ";
      cout << purchase.qtys[i] << endl;
    }

    cout << "------------------------" << endl;
      
    cout << "ID Item: ";
    cin >> id;

    for (Item search_item : *items) {
      if (search_item.id == id) {
        item = search_item;
      }
    }

    if (id == 0) {
      break;
    } else if (is_null_item(item)) {
      message = "Item dengan ID " + to_string(id) + " tidak ditemukan";
      continue;
    }

    cout << "Qty: ";
    cin >> qty;

    if (qty > item.stock) {
      message = "Barang out of stock";
      continue;
    }

    item.stock -= qty;
    update_item(items, item.id, item);

    purchase.total_price += item.price * qty;
    purchase.qtys.push_back(qty);
    purchase.items.push_back(item);
  }

  return purchase;
}

void show_invoice(Purchase purchase) {
  system("clear");
  cout << "Invoice" << endl;
  cout << "-----------------------------------" << endl;
  cout << "Subtotal   Satuan   Jumlah   Barang" << endl;
  cout << "-----------------------------------" << endl;

  for (int i = 0; i < purchase.items.size(); i++) {
    cout << purchase.items[i].price * purchase.qtys[i] << "   ";
    cout << purchase.items[i].price << "   ";
    cout << purchase.qtys[i] << "   ";
    cout << purchase.items[i].name << endl;
  }

  cout << "-----------------------------------" << endl;

  cout << "Total: " << endl;
  cout << purchase.total_price << endl;
}

void menu_main(vector<Item>* items) {
  char choice;

  while (true) {
    system("clear");
    cout << "APLIKASI KASIR SEDERHANA" << endl;
    cout << "------------------------" << endl;
    cout << "1. Manajemen Barang" << endl;
    cout << "2. Pembayaran" << endl;
    cout << "3. Exit" << endl;
    cout << "------------------------" << endl;
    cout << "Pilihan: ";
    cin >> choice;

    if (choice == '3') {
      break;
    }

    switch (choice) {
      case '1': menu_item_management(items); break;
      case '2': menu_purchasing(items); break;
      default: cout << "Invalid" << endl; break;
    }
  }
}

void menu_item_management(vector<Item>* items) {
  char choice;
  string message = "";

  while (true) {
    system("clear");
    cout << "MANAJEMEN BARANG" << endl;
    cout << "------------------------" << endl;

    if (message != "") {
      cout << message << endl;
      cout << "------------------------" << endl;
      message = "";
    }

    cout << "1. Lihat Semua Barang" << endl;
    cout << "2. Tambah Barang Baru" << endl;
    cout << "3. Ubah Barang" << endl;
    cout << "4. Hapus Barang" << endl;
    cout << "5. Kembali" << endl;
    cout << "------------------------" << endl;
    cout << "Pilihan: ";
    cin >> choice;

    if (choice == '5') {
      break;
    }

    switch (choice) {
      case '1': menu_show_items(items); break;
      case '2': message = menu_insert_item(items); break;
      case '3': message = menu_update_item(items); break;
      case '4': message = menu_delete_item(items); break;
      default: cout << "Invalid" << endl; break;
    }
  }
}

void menu_show_items(vector<Item>* items) {
  char choice;

  while (true) {
    system("clear");
    cout << "LIHAT SEMUA BARANG" << endl;
    cout << "------------------------" << endl;
    show_items(*items);
    cout << "------------------------" << endl;
    cout << "1. Kembali" << endl;
    cout << "Pilihan: ";
    cin >> choice;

    if (choice == '1') {
      break;
    }

    cout << "Invalid" << endl;
  }
}

string menu_insert_item(vector<Item>* items) {
  system("clear");
  cout << "TAMBAH BARANG BARU" << endl;
  cout << "------------------------" << endl;

  cin.ignore();
  Item item = create_item_from_user();
  insert_item(items, item);
  return "Item telah ditambahkan";
}

string menu_update_item(vector<Item>* items) {
  char confirm_update;

  system("clear");
  cout << "UBAH BARANG" << endl;
  cout << "------------------------" << endl;
  show_items(*items);
  cout << "------------------------" << endl;

  Item item_to_edit = find_item_by_id(*items);
  int id_to_edit = item_to_edit.id;

  cin.ignore();

  cout << item_to_string(item_to_edit) << endl;

  cout << "Tuliskan data baru untuk item tersebut" << endl;
  Item new_item_data = create_item_from_user();

  new_item_data.id = id_to_edit;

  cout << item_to_string(new_item_data) << endl;
  cout << "Simpan perubahan? (y/n)";
  cin >> confirm_update;

  if (confirm_update == 'y') {
    update_item(items, id_to_edit, new_item_data);
    return "Item telah diubah";
  } else {
    return "Ubah dibatalkan";
  }  
}

string menu_delete_item(vector<Item>* items) {
  char confirm_delete;

  system("clear");
  cout << "HAPUS BARANG" << endl;
  cout << "------------------------" << endl;
  show_items(*items);
  cout << "------------------------" << endl;

  Item item_to_delete = find_item_by_id(*items);
  int id_to_delete = item_to_delete.id;

  cout << item_to_string(item_to_delete) << endl;
  cout << "Hapus item ini? (y/n) ";
  cin >> confirm_delete;

  if (confirm_delete == 'y') {
    delete_item(items, id_to_delete);
    return "Item telah terhapus";
  } else {
    return "Hapus dibatalkan";
  }
}

void menu_purchasing(vector<Item>* items) {
  Purchase purchase = purchase_items(items);
  char choice;

  while (true) {
    show_invoice(purchase);
    cout << "------------------------" << endl;
    cout << "1. Kembali" << endl;
    cout << "Pilihan: ";
    cin >> choice;

    if (choice == '1') {
      break;
    }

    cout << "Invalid" << endl;
  }
}