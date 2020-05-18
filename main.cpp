#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
/* Consts */
const int MS = 1000;
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
  for(int i = 0; i < MS; i++)
    if (!pages[i]){
      pages[i] = 1;
      file_mem[file_ind].push_back(i);
    }
  return true;
}


int main(){
  cout << "hw";
  //AddFile();
  //files.insert({0, 0});
  return 0;
}
