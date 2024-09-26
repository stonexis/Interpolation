#pragma once
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <Python.h>
//задаваемые значения
const long double aConst = -5.0;
const long double bConst = 5.0;
const size_t NConst = 8;
const size_t MConst = 800;
const long double Epsilon = 1e-3;

template <typename T>
void func(const T *arrX, T *arrFx, size_t Length) {
	for (size_t i = 0; i < Length; i++)
		arrFx[i] = sin(arrX[i]); //заданная функция
}

template <typename T>
void genRandomArrWithSpacing(T *arr, T minSpacing, size_t Length) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<T> distrib(aConst, bConst);
	arr[0] = aConst;
	for (std::size_t i = 1; i < Length - 1; i++) {
		T value;
		bool isValid;
		do {
			value = distrib(gen);  //Генерируем новое значение
			isValid = true;
			//Находим место для вставки значения с помощью бинарного поиска
			auto lower = std::lower_bound(arr, arr + i, value);//находит наименьший элемент, больший данного
			//Проверяем ближайших соседей на расстояние
			if (lower != arr + i && abs(*lower - value) < minSpacing) {//первая проверка на попадание lower в конец текущего диапазона(если попали, то с правым соседом не проверяем(второе условие))
				isValid = false;
			}
			if (lower != arr && abs(*(lower - 1) - value) < minSpacing) {//первая проверка на попадание в начало диапазона, если попали, с левым не проверяем
				isValid = false;
			}
		} while (!isValid);
		//Вставляем значение в правильное место, чтобы сохранить массив отсортированным
		auto position = std::lower_bound(arr, arr + i, value);
		std::rotate(position, arr + i, arr + i + 1);//циклически сдвигаем от позиции posit все элементы заполненного массива на 1
		*position = value;
	}
	arr[Length - 1] = bConst;
}

template <typename T>
void genRandomArrSealing(T *newarr, const T *oldarr, size_t LengthOld, size_t M, T a, T b) {
	long double sizeInterval = abs(b-a)/M; //определяем размер интервала сетки
	for (size_t i = 0; i < LengthOld; i++)//заполняем новый массив старыми значениями
		newarr[i] = oldarr[i];
	for (size_t i = LengthOld; i < M; i++)
		newarr[i] = a + sizeInterval*i; //добавляем в новый массив значения равномерной сетки, после значений старого массива
	std::sort(newarr, newarr + M);
}

template <typename T>
void denominatorFun(const T *arrNodesX, T *denominator, size_t NConst) {
	for(size_t i = 0; i < NConst; i++){
		T product = 1;
		for(size_t j = 0; j < NConst; j++){
			 if (i != j) 
            	product *= arrNodesX[i] - arrNodesX[j];
		}
		denominator[i] = product;
	}
}

template <typename T>
void lagrange3(const T *arrNodesX, const T *arrFx, const T *arrEvalPoints, T *arrLagrange, size_t NConst, size_t MConst) {
	auto denominator = new T[NConst];
	denominatorFun(arrNodesX,denominator, NConst); //вызываем функцию заполения массива знаменателя
	//Цикл по точкам, в которых хотим вычислить значения многочлена
	for (size_t i = 0; i < MConst; i++) {
		T evalPoint = arrEvalPoints[i];
		T sum = 0;
		//Цикл по узлам интерполяции
		for (size_t j = 0; j < NConst; j++) {
			T product = 1;
			//Цикл для вычисления базисного многочлена Лагранжа L_j(x)
			for (size_t k = 0; k < NConst; k++) {
				if (k != j) {
					product *= evalPoint - arrNodesX[k]; //посчитать знаменатель отдельно
				}
			}      
			sum += (arrFx[j] * product) / denominator[j]; //Добавляем вклад j-го базисного многочлена в результат
		}
		arrLagrange[i] = sum;
	}
	delete[] denominator;
}
template <typename T>
void checkErrorsFun(T a, T b, size_t N, size_t M, T Epsilon){
	if (bConst<aConst || abs(bConst-aConst)<Epsilon)
		throw std::invalid_argument("needs: b > a");
	if (NConst == 0)
		throw std::invalid_argument("N != 0");
}

template <typename T>
void writeToFileArr(std::ofstream &out, T *array, size_t Length, std::string nameArray){
 out << "\"" << nameArray << "\"" << ": ["; //форматируем файл под json
    for (size_t i = 0; i < Length; ++i) {
        out << array[i];
        if (i != Length - 1) 
			out << ", ";
    }
    out << "]";
}

template <typename T>
void writeDataToFile(const T *arrX, const T *arrFx, const T *arrLagrange, const T *arrNodesX, const T *valueInterpolation, size_t NConst, size_t MConst){
	std::ofstream out;
    out.open("data.json"); 
	if (!out.is_open())
		throw std::runtime_error("Can`t open file");
	else
    {
		out << '{' << '\n';
		writeToFileArr(out,arrX,MConst, "arrX");
		out << ',' << '\n';
		writeToFileArr(out,arrFx,MConst, "arrFx");
		out << ',' << '\n';
		writeToFileArr(out,arrLagrange,MConst, "arrLagrange");
		out << ',' << '\n';
		writeToFileArr(out,arrNodesX,NConst, "arrNodesX");
		out << ',' << '\n';
		writeToFileArr(out,valueInterpolation,NConst, "valueInterpolation");
		out << '}';
		out.close();
    }
}

template <typename T>
void callPythonScript2(const T* arrX, const T* arrFx, const T* arrLagrange, const T* arrNodesX, const T* valueInterpolation, size_t NConst, size_t MConst) {
    writeDataToFile(arrX, arrFx, arrLagrange, arrNodesX, valueInterpolation, NConst, MConst);
    Py_Initialize(); //Инициализируем интерпретатор Python

    PyObject* sysPath = PySys_GetObject("path"); //Добавляем путь к директории с модулем plotter.py
    PyList_Append(sysPath, PyUnicode_FromString("."));

    PyObject* pName = PyUnicode_DecodeFSDefault("plotter"); //Загружаем модуль plotter
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);//уменьшаем счетчик ссылок на имя, оно больше не нужно

    if (pModule != nullptr) {//модуль существует
        PyObject* pFunc = PyObject_GetAttrString(pModule, "plot_graph"); //Загружаем функцию plot_graph
        if (pFunc && PyCallable_Check(pFunc)) { //нашли функцию и ее можно вызвать
            PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString("data.json")); //Передаем имя файла в Python функцию
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);  //Вызываем функцию
            if (pValue != nullptr) { //Проверяем, выполнен ли вызов успешно
                Py_DECREF(pValue);
            } else {
                PyErr_Print();
                std::cerr << "Function call failed" << std::endl;
            }
            Py_DECREF(pArgs);//Убираем использованные объекты
        } else {
            PyErr_Print();
            std::cerr << "Failed to find or call function 'plot_graph'" << std::endl;
        }
        Py_XDECREF(pFunc); //Освобождаем память
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        std::cerr << "Failed to load module 'plotter'" << std::endl;
    }
    Py_Finalize();//Завершаем интерпретатор Python
}
