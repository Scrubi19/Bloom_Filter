#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

const char * alphabet[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t",
        "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T",
        "U", "V", "W", "X", "Y", "Z",};

class BloomFilter {
public:
    unsigned int m_cells;                      //размер байтового массива m
    unsigned int k_hash;                      //Количество хэш-функций
    std::vector<bool> cell;                  //указатель на будущий массив байтов m
    
    //Конструктор для выделения памяти под байтовый массив m
    BloomFilter(int numbCells) {
        this->m_cells = numbCells;
        cell.resize(numbCells);
    }

    void addElement(std::string str) {
        //цикл будет выполняться ровно столько раз, сколько хэш-функций было использовано для хранения данных
        for (int i = 0; i < (this->k_hash); i++) {
            cell[this->murmurhash2(str.c_str(), str.length(), i) % this->m_cells] = true;
        }
    }

    bool searchElement(std::string str) {
        bool strInSet = true;

        for (int i = 0; i < (this->k_hash); i++) {
            if (cell[this->murmurhash2(str.c_str(), str.length(), i) % this->m_cells] == false) {
                strInSet = false;
                break;
            }
        }
        return strInSet;
    }

    void OptimalBloom(int n_members, double error_probability)
    {
		std::cout << "Подсчет кол-ва ячеек фильтра и хэш-функций для значений = " << n_members << " и вероятности = " << error_probability * 100 << std::endl;
        this->m_cells = -(n_members * log(error_probability)) / (log(2) * log(2));
        this->cell.resize(this->m_cells);
        this->k_hash = (this->m_cells / n_members) * log(2)+1;
		std::cout << "Оптимальное кол-во ячеек памяти = " << this->m_cells << std::endl
			<< "Оптимальное кол-во хэш-функций = " << this->k_hash << std::endl;
    }
    unsigned int murmurhash2(const void * key, int len, const unsigned int seed) {
        const unsigned int m = 0x5bd1e995;
        const int r = 24;
        unsigned int h = seed ^ len;
        const unsigned char * data = (const unsigned char *)key;

        while (len >= 4) {
            unsigned int k = *(unsigned int *)data;
            k *= m;
            k ^= k >> r;
            k *= m;
            h *= m;
            h ^= k;
            data += 4;
            len -= 4;
        }

        switch (len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
            h *= m;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }
};

std::vector<std::string> open_dictionary(int n_words) {
    std::vector<std::string> arr;
    std::vector<std::string>::iterator i;
    std::ifstream file;
    std::string str;

    file.open("./dictionary.txt");
    if ( !file.is_open() )
        std::cerr << "Can't open file" << std::endl;

    for (int i = 0; i < n_words; i++){
		file >> str;
        arr.emplace_back(str);
    }

    file.close();
    return arr;
}

std::string random_str(std::string s) {
    s.clear();
    for(int i = 0; i < 5; i++) {
        s += alphabet[rand() % (sizeof alphabet/sizeof (char *))];
    } 
    return s;
}

int main() {
    //Создаем BloomFilter
    int i = 0, k = 0;
    int false_point = 0;
    int true_point = 0;
 	int n_words = 10000;
    double error_probability = 0.1;
    std::string s;
    
    BloomFilter *bfilter = new BloomFilter(n_words);
    bfilter->OptimalBloom(n_words, error_probability);
    
    std::vector<std::string> dictionary = open_dictionary(92928);
 
 	int p = 92859;
    std::cout << "Добавлено слов в фильтр: " << p <<std::endl;
    for (std::vector<std::string>::iterator iter = dictionary.begin(); k < p; iter++) {
        //std::cout << "    " << *iter << std::endl;
        bfilter->addElement(*iter);
        k++;
    }
    int n = 1000000;
    for(int i = 0; i < n; i++) {
        s = random_str(s);
        //std::cout << "s = " << s << std::endl;
        if(bfilter->searchElement(s) == true) {
            false_point++;
        } else {
            true_point++;
        }
    }

    printf("Процент ложноположительного срабатывания при %d иттераций = False = %d - True = %d\n", n, false_point, true_point);

    return 0;
}