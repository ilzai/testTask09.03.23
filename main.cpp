#include <iostream>
#include <vector>
#include <ctime>
#include <thread>
#include <valarray>
#include <mutex>

using namespace std;

mutex m;

template<typename T>
void calc(const vector<T> &nums, vector<T> &vect, int window){
    T summ = 0;
    int len = nums.size();
    //cout << "ID" << this_thread::get_id() << endl;
    for(int i = 0; i < len; i++){
        summ += nums[i];
        if(i % (window - 1) == 0 && i != 0){
            vect.push_back(summ / (i + 1));
            //cout << nums[i] << endl;
        }
    }
}

template<typename T>
void show(vector<T> &nums){
    int len = nums.size();
    for (int i = 0; i < len; i++){
        cout << nums[i] << " ";
    }
}

int main(){
    //Генерация входного массива данных
    vector<float> v;
    srand(time(0));
    int min = 1;
    int max = 100;
    int count = 53;//количество входных данных
    for(int i = 0; i < count; i++){
        v.push_back((float)(rand()) / RAND_MAX * (max - min) + min);
    }
    show<float>(v);
    cout << endl;

    //засекаем время старта алгоритма
    unsigned int startTime = clock();

    //Распараллеливание
    vector<vector<float>> vect;//массив для разделения вектора на n частей

    int window = 4;//окно усреднения
    int b;
    auto minV = v.cbegin();
    auto maxV = v.cbegin();
    if ((v.size() / 4 + 1) % window == 0){
        maxV = v.cbegin() + v.size() / 4 + 1;
    }else{
        b = (v.size() / 4 + 1) % window;
        maxV = v.cbegin() + v.size() / 4 + 1 - b;
    }
    
    //массив делится на 4 части, по одной части на каждый поток
    for(int i = 0; i < 4; i++){
        vector<float> n(minV, maxV);
        vect.push_back(n);
        show<float>(n);
        cout << endl;
        n.clear();
        minV = maxV;
        if (i != 2) maxV = maxV + (v.size() / 4 + 1 - b);
        else(maxV = v.end());
    }

    vector<thread*> th(4, nullptr);//массив потоков
    vector<vector<float>> res;//массив результатов

    //инициализация массива результатов
    for (int i = 0; i < 4; i++){
        vector<float> temp;
        res.push_back(temp);
    }
    
    
    for(int i = 0; i < 4; i++){
        th[i] = new thread(calc<float>, std::ref(vect[i]), std::ref(res[i]), window);
    }

    for(int i = 0; i < 4; i++){
        th[i]->join();
    }

    //засекаем время окончания алгоритма
    unsigned int endTime = clock();
    unsigned int resultTime = endTime - startTime;
    cout << "/////////////////////" << endl;
    for(int i = 0; i < res.size(); i++){
        show(res[i]);
    }
    cout << "/////////////////////" << endl;
    cout << "Время работы " << resultTime << " мс." << endl;
    return 0;
}