#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
/* Consts */
const int MS = 100;
const int Size_of_page = 512;


class MFILE{
public:
  int memory = 0;
  string name  = "";
  bool can_be_changed = true;
  bool is_folder = false;
  vector <int> fls; // if folder contains files 
  int id_parent = -1; // to go back
  int ID = -1;
public:
/*MFILE ( int mem1, string name1, bool f){
    memory = mem1; name = name1; can_be_changed = f;
  }*/
  MFILE ( int mem1, string name1){
    memory = mem1; name = name1;
  }
  MFILE ( int mem1, string name1, int index){
    memory = mem1; name = name1, ID = index;
  }
  MFILE ( ){
    memory = 0; name = "";
  }

};


bool operator<(const MFILE &f1, const MFILE &f2 ){
  return f1.name < f2.name;
}




/* Variables */
map<int, MFILE> files; // храним информацию о файле
map<int, vector<int> > file_mem; // храним страницы памяти
vector<int> pages(MS, 0);
unsigned int cur_index = 1;
int empty_pages = MS;
int NOW_POS = 0;
// массив страниц
// изначально пустые. считаем, что страница вмещает 512 бит
// файл занимает страницу полностью, даже если ему столько не надо


bool AddFile(int index){
  //system("clear");
  int file_ind = cur_index++;
  cout << "Enter the memory(bytes) of file: ";
  int mem;
  cin >> mem;
  cout << "Enter name of file: ";
  string name1;
  cin >> name1;

  /* Срздание и занимание памяти */
  MFILE *nf = new MFILE(mem, name1, file_ind);
  nf->id_parent = index;
  if (mem % Size_of_page) mem+= Size_of_page;
  mem /= Size_of_page;  // округление вверх
                        // mem - pages for now
  if (mem > empty_pages) {cur_index--;
                          return false;} // cant add that file
  
  empty_pages -= mem;
  files[file_ind] = *nf;
  for(int i = 0; i < MS && mem; i++)
    if (!pages[i]){
      pages[i] = 1;
      file_mem[file_ind].push_back(i);
      mem--;
    }

  /* Память выделилась */
  map<int, MFILE > :: iterator it;
  it = files.find(index);
  it->second.fls.push_back(file_ind);

  return true;
}

void Display(){
  //system("clear");
  cout << "Current memory stash:\n";

  for(int i = 0 ; i < MS; i++){
    
    if (pages[i]){
      cout << "\033[1;41m \033[0m";
    } else cout << "\033[1;44m \033[0m";
    
  }
cout << endl;
cout << "Current files in stash:\n";
for( map<int, MFILE>::iterator it = files.begin(); it != files.end(); it++ )
  if (!it->second.is_folder) 
  cout << it->first << ") " << it->second.name << " ( memory needed: " << it->second.memory << ")\n";
  cout << "Enter any char to continue";
  char s = '1';
  cin >> s;
cout << endl << endl;
}


void ShowPagesOfFile(int index){
  system("clear");
  map<int, vector<int> > :: iterator it;
  it = file_mem.find(index);
  if (it == file_mem.end()) {
    cout << "File not found\n";
    return;
  }
  vector<int> curpgs = it->second;
  cout << "File with index = " << index << " is found\n";
  cout << "Name: " <<  files[index].name << ", Memory: " << files[index].memory << endl;
  cout << "This file gets this pages in memory storage:\n";
  int curr_ind = 0;
  for(int i = 0; i < MS; i++){
    if ( curr_ind < curpgs.size() && i == curpgs[curr_ind]){
      cout << "\033[1;43m \033[0m";
      curr_ind++;
    }else
    {
      if (pages[i]) cout << "\033[1;41m \033[0m";
      else
      {
         cout << "\033[1;44m \033[0m";
      }
      
    }
    

  }
  cout << "\nEnter any char to continue";
  char s = '1';
  cin >> s;
  cout << endl << endl;

}

void CreateFolder(int index){
  //system("clear");
  int file_ind = cur_index++;
  cout << "Enter name of folder: ";
  string name1;
  cin >> name1;
  MFILE *nf = new MFILE(0, name1, file_ind);
  nf->is_folder = true;
  nf->id_parent = index;
  files[file_ind] = *nf;
  vector<int> n(0);
  file_mem[file_ind] = n;
  //if (!index){ return ;}
 
  
  map<int, MFILE > :: iterator it;
  it = files.find(index);
  it->second.fls.push_back(file_ind);
  //cout << it->second.fls.size();
}

void DisplayFolder(int index){
  MFILE fil = files[index];
  //if (!index) cout << "Folder root" << endl;
  //else {
    cout << "Folder " <<  fil.name << endl;
    //if (!index)
    cout << "\t..>(" << fil.id_parent << ")\n";
 //}
  
  if (!fil.fls.size()) {
    cout << "empty...\n"; return;
  }

  for (int i = 0; i < fil.fls.size(); i++)
  {
    MFILE f = files[fil.fls[i]];
    if (f.is_folder)
      cout << "\tdir>";
    else
      cout << "\tfile>";
    cout << f.name << "(" << f.ID << ")\n";
  }
}

bool DeleteFile(int index){
  //system("clear");
  if (!index) {cout << "root cannot be deleted\n"; return false;}
  map<int, vector<int>> :: iterator it;
  it = file_mem.find(index);
  if (it == file_mem.end()) {
    cout << "File not found\n";
    return false;
  }
  map<int, MFILE > :: iterator it1;
  it1 = files.find(index);
  map<int, MFILE> :: iterator it2;
  it2 = files.find(it1->second.id_parent);
  //MFILE fil = files[index];
  if (it1->second.is_folder){
    for (int i = 0 ; i < it1->second.fls.size(); i++)
      DeleteFile(it1->second.fls[i--]);
    file_mem.erase(index); 
    
    
    for(int i = 0; i < it2->second.fls.size(); i++){
      if (it2->second.fls[i] == index) {it2->second.fls.erase(it2->second.fls.begin() + i);
      
       break;}
    }

    files.erase(index);
    return true;
  }
  files[index].memory += files[index].memory % Size_of_page ? Size_of_page : 0;
  empty_pages += files[index].memory / Size_of_page;
  int cur_ind = 0; 
  
  for(int i = 0 ; i < MS; i++)
  {
    if( cur_ind < it->second.size() && i == it->second[cur_ind]){
      pages[i] = 0;
      cur_ind++;
    }
  }
  MFILE f = files[index];
  file_mem.erase(index); 
  //files.erase(index);
  
    
    //map<int, MFILE> :: iterator it2;
    //it2 = files.find(it1->second.id_parent);
    
    for(int i = 0; i < it2->second.fls.size(); i++){

      if (it2->second.fls[i] == index) {it2->second.fls.erase(it2->second.fls.begin() + i); break;}
    }
  files.erase(index);
  
  cout << "File " << f.name << "successfuly deleted from memory\n"; 

  return true;
}

void GoToFolder(int index){
  map<int, MFILE> :: iterator it;
  it = files.find(index);
  if (it == files.end()) {
    cout << "Folder not found\n";
    return ;
  }
  NOW_POS = index;
}


void HelpPage(){
  cout << "There are commands:\n\tls - to watch current directory\n\t";
  cout << "help - for show this menu\n\t";
  cout << "cd - to enter some directory(by index)\n\tdisp - watch all files and memory status\n\t";
  cout << "del or chng- to delete or change file or folder(by index)\n\t";
  cout << "sh - to see whole info about file(by index)\n\t";
  cout << "crFld - to create new folder\n\tcrFl - to create new file\n\t";
  cout << "rel - to relocate file or folder(index of file, then index of folder)\n\t";
  cout << "cls - clear screen\n\tquit - close program\n";

}

bool ChangeFile(int ID){
  //system("clear");
  map<int, vector<int> > :: iterator it;
  it = file_mem.find(ID);
  if (it == file_mem.end() || !files[ID].can_be_changed) {
    cout << "File not found or cant be modified\n";
    return false;
  }
  if (files[ID].is_folder){
    cout << "folder cannot be changed\n";
    return false;
  }
  int ns = 0;
  cout << "Enter new size of this file: ";
  cin >> ns;
  int tmp = ns;
  if (ns < 0) ns = 0;
  if (ns & Size_of_page) ns += Size_of_page;
  ns /= Size_of_page;
  
  int delta = ns - file_mem[ID].size();
  if (delta > empty_pages)
    {
      cout << "There is no enouth memory to change";
      return false;
    }
  
  files[ID].memory = tmp;
  empty_pages -= delta;
  if (delta <= 0)
  while (delta < 0){
    
    int cur_pg = file_mem[ID][file_mem[ID].size() - 1];
    //cout << "DEBUG " << delta << " " <<cur_pg << endl;;
    pages[cur_pg] = 0;
    file_mem[ID].pop_back();
    delta++;
  }
  else{
    for(int i = 0; i < MS && delta; i++){
      if (!pages[i]){
        file_mem[ID].push_back(i);
        pages[i] = 1;
        delta--;
      }
    }
  }  
  char c;
  cout << "\nEnter any char to continue";
  cin >> c;
  return true;
}

void Relocate(int index, int folder){
  map<int, MFILE> :: iterator it;
  it = files.find(folder);
  if (it == files.end() || !it->second.is_folder) {
    cout << "Folder not found or its unavailebale\n";
    return;
  }
  it = files.find(index);
  if (it == files.end()) {
    cout << "File not found\n";
    return;
  }
  
  map<int, MFILE> :: iterator it2;
  it2 = files.find(it->second.id_parent);
  for(int i = 0 ;i < it2->second.fls.size(); i++){
    if (it2->second.fls[i] == index) it2->second.fls.erase(it2->second.fls.begin() + i);
  }
  it->second.id_parent = folder;
  it2 = files.find(folder);
  it2->second.fls.push_back(index);
  }

int main(){
  
  system("clear");
  HelpPage();

  MFILE *nf = new MFILE(0, "_root", 0);
  nf->is_folder = true;
  nf->id_parent = 0;
  files[0] = *nf;
  int tmp = -1;
  string command = "";
  while (command != "quit"){
    cout << "command>>";
    getline(cin, command);
    stringstream iss(command);
    string cmd; int ind = -1;
    iss >> cmd >> ind;
    command = cmd;
    if (cmd == "ls"){
      DisplayFolder(NOW_POS);
    }
    else if (cmd == "cd") {
      if (ind < 0) {cout << "bad index, try again\n"; break;}


      bool f = false;
      for (int i = 0 ; i < files[NOW_POS].fls.size() && !f; i++)
        if (files[NOW_POS].fls[i] == ind) f= true;
      if (f || files[NOW_POS].id_parent == ind) GoToFolder(ind);
      else GoToFolder(-1);


      }
    else if (cmd == "del") {
      if (ind < 0) {cout << "bad index, try again\n"; break;}
      bool f = false;
      for (int i = 0 ; i < files[NOW_POS].fls.size() && !f; i++)
        if (files[NOW_POS].fls[i] == ind) f= true;
      if (f) DeleteFile(ind);
      else DeleteFile(-1);
    }
    else if (cmd == "chng") {
      if (ind < 0) {cout << "bad index, try again\n"; break;}


      bool f = false;
      for (int i = 0 ; i < files[NOW_POS].fls.size() && !f; i++)
        if (files[NOW_POS].fls[i] == ind) f= true;
      if (f) ChangeFile(ind);
      else ChangeFile(-1);
      }
    else if (cmd == "sh") {
      if (ind < 0) {cout << "bad index, try again\n"; break;}

      bool f = false;
      for (int i = 0 ; i < files[NOW_POS].fls.size() && !f; i++)
        if (files[NOW_POS].fls[i] == ind) f= true;
      if (f) ShowPagesOfFile(ind);
      else ShowPagesOfFile(-1);
      }
    else if (cmd == "rel"){
      cout << " input new location of file: ";
      cin >> tmp;
      if (tmp < 0) {cout << "Uncorrect place\n"; break;}
      Relocate(ind, tmp);
    }
    else if (cmd == "crFld") {CreateFolder(NOW_POS);}
    else if (cmd == "crFl") {AddFile(NOW_POS);}
    else if (cmd == "help") {HelpPage();}
    else if (cmd == "disp") {Display();}
    else if (cmd == "cls") {system("clear");}
    else if (cmd == "quit") cout << "exit\n";
    else {cout << "unnown command\n";}
  }
  return 0;
}
