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
public:
  MFILE ( int mem1, string name1){
    memory = mem1; name = name1;
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
unsigned int cur_index = 0;
int empty_pages = MS;

// массив страниц
// изначально пустые. считаем, что страница вмещает 512 бит
// файл занимает страницу полностью, даже если ему столько не надо


bool AddFile(){
  int file_ind = cur_index++;
  cout << "Enter the memory(bytes) of file: ";
  int mem;
  cin >> mem;
  cout << "Enter name of file: ";
  string name1;
  cin >> name1;
  MFILE *nf = new MFILE(mem, name1);
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
  return true;
}

void Display(){
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
cout << endl << endl;
}

void ShowPagesOfFile(int index){
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
  cout << endl << endl;
}

bool DeleteFile(int index){
  map<int, vector<int> > :: iterator it;
  it = file_mem.find(index);
  if (it == file_mem.end()) {
    cout << "File not found\n";
    return false;
  }
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

int main(){
  //cout << "hw";
  AddFile();
  Display();
  //ShowPagesOfFile(0);
  AddFile();
  Display();
  DeleteFile(0);
  Display();
  //ShowPagesOfFile(1);
  AddFile();
  ShowPagesOfFile(2);
  return 0;
}
