#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
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
  system("clear");
  int file_ind = cur_index++;
  cout << "Enter the memory(bytes) of file: ";
  int mem;
  cin >> mem;
  cout << "Enter name of file: ";
  string name1;
  cin >> name1;

  /* Срздание и занимание памяти */
  MFILE *nf = new MFILE(mem, name1, file_ind);
  
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
  //MFILE p = files[index];
  cout << "parents name " << it->second.name << endl;
  char r;
  
  //p.fls.push_back(file_ind);
  cin >> r; 
  return true;
}

void Display(){
  system("clear");
  cout << "Current memory stash:\n";

  for(int i = 0 ; i < MS; i++){
    
    if (pages[i]){
      cout << "\033[1;41m \033[0m";
    } else cout << "\033[1;44m \033[0m";
    
  }
cout << endl;
cout << "Current files in stash:\n";
for( map<int, MFILE>::iterator it = files.begin(); it != files.end(); it++ )
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
  system("clear");
  int file_ind = cur_index++;
  cout << "Enter name of folder: ";
  string name1;
  cin >> name1;
  MFILE *nf = new MFILE(0, name1, file_ind);
  nf->is_folder = true;
  files[file_ind] = *nf;
  if (!index){ return ;}
  nf->id_parent = index;
  
  map<int, MFILE > :: iterator it;
  it = files.find(index);
  it->second.fls.push_back(file_ind);
}

void DisplayFolder(int index){
  MFILE fil = files[index];
  cout << "Folder " <<  fil.name << endl;
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
  system("clear");
  map<int, vector<int>> :: iterator it;
  it = file_mem.find(index);
  if (it == file_mem.end()) {
    cout << "File not found\n";
    return false;
  }
  MFILE fil = files[index];
  if (fil.is_folder){
    for (int i = 0 ; i < fil.fls.size(); i++)
      DeleteFile(fil.fls[i]);
    file_mem.erase(index); 
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


bool ChangeFile(int ID){
  system("clear");
  map<int, vector<int> > :: iterator it;
  it = file_mem.find(ID);
  if (it == file_mem.end() || !files[ID].can_be_changed) {
    cout << "File not found or cant be modified\n";
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

int main(){
  
  /*bool exit = false;
  do {
    int ind = 0;
    system("clear");
    cout << "MAIN MENU:\n";
    cout << "Free memory: " << empty_pages * Size_of_page << " from " << MS * Size_of_page << endl;
    cout << "1. Add file\n2. Delete file\n3. Find in storage file\n4. Show status\n5. Change File\n6. Exit\n";
    cout << "Input number of comand to continue"; 
    int comand = -1;
    cin >> comand;
    switch(comand){
      case 1:
      AddFile();
      break;
      case 2:
      cout << "Input index of file to delete: ";
      cin >> ind;
      DeleteFile(ind);
      break;
      case 3:
      cout << "Input index of file to find: ";
      cin >> ind;
      ShowPagesOfFile(ind);
      break;
      case 4: Display();break;
      case 5: cout << "Input index of file to change: "; cin >> ind; ChangeFile(ind); break;
      case 6: 
      cout << "exiting..";
      exit = true;
      break; 
    } 
  } while (!exit);*/

  CreateFolder(0);
  DisplayFolder(1);
  AddFile(1);
  CreateFolder(1);
  //Display();
  
  DisplayFolder(1);
  //DisplayFolder(0);
  cout << files[1].fls.size();
  return 0;
}
