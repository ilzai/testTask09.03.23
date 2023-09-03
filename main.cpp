#include <iostream>
#include <vector>
#include <ctime>
#include <thread>
#include <valarray>

using namespace std;

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

template<typename T>
vector<T> generating(int min, int max, int count){
    vector<T> v;
    for(int i = 0; i < count; i++){
        v.push_back((T)(rand()) / RAND_MAX * (max - min) + min);
    }
    //show<float>(v);
    cout << endl;
    return v;
}

//Разбиение массива на подмассивы для каждого потока
template<typename T>
vector<vector<T>> splittingUp(vector<T> &v, int window, int threadsCount){
    vector<vector<T>> vect;
    int b;
    auto minV = v.cbegin();
    auto maxV = v.cbegin();
    if ((v.size() / threadsCount + 1) % window == 0){
        maxV = v.cbegin() + v.size() / threadsCount + 1;
    }else{
        b = (v.size() / threadsCount + 1) % window;
        maxV = v.cbegin() + v.size() / threadsCount + 1 - b;
    }
    
    //массив делится на n частей, по одной части на каждый поток
    for(int i = 0; i < threadsCount; i++){
        vector<T> n(minV, maxV);
        vect.push_back(n);
        //show<T>(n);
        cout << endl;
        n.clear();
        minV = maxV;
        if (i != 2) maxV = maxV + (v.size() / 4 + 1 - b);
        else(maxV = v.end());
    }
    return vect;
}

template<typename T>
vector<vector<T>> threads(vector<vector<T>> &vect, int threadsCount, int window){
    vector<thread*> th(threadsCount, nullptr);//массив потоков
    vector<vector<T>> res;//массив результатов

    //инициализация массива результатов
    for (int i = 0; i < threadsCount; i++){
        vector<T> temp;
        res.push_back(temp);
    }
    
    //пулл потоков
    for(int i = 0; i < threadsCount; i++){
        th[i] = new thread(calc<T>, std::ref(vect[i]), std::ref(res[i]), window);
    }

    for(int i = 0; i < threadsCount; i++){
        th[i]->join();
    }

    return res;
}

int main(){
    //Генерация входного массива данных
    vector<double> v = generating<double>(1, 20, 1000000);

    //засекаем время старта алгоритма
    clock_t startTime = clock();

    //Распараллеливание
    int window = 128;//окно усреднения
    int threadsCount = 4;//количество потоков
    vector<vector<double>> vect = splittingUp<double>(v, window, threadsCount);//массив для разделения вектора на n частей
    vector<vector<double>> res = threads<double>(vect, threadsCount, window);

    //засекаем время окончания алгоритма
    clock_t endTime = clock();
    clock_t resultTime = endTime - startTime;
    cout << "/////////////////////" << endl;
    for(int i = 0; i < res.size(); i++){
        //show(res[i]);
    }
    cout << "/////////////////////" << endl;
    cout << "Время работы " << ((double)resultTime) / CLOCKS_PER_SEC << " с" << endl;
    return 0;
}