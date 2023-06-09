# IndustrialCPPLab2
Проект реализует технологию Map-Reduce.
## Описание работы
Основным звеном является класс Job, который реализует многопоточный подсчет количество встречаний слов в исходном тексте.
Ход работы Job:
1. Происходит сплит исходного файла. По итогу создаются новые файлы, которые далее попадут в маппер.
2. Определяется количество потоков для маппинга. Максимальное количество - количество ядер. Далее каждый поток выполняет функцию Map (класс Map), которая берет соответсвующие файлы из тех, что были созданы на этапе сплита и обрабатывает их. Каждое слово кладется в storage в виде ключа, значением является вектор, в который каждый раз при встрече того же слова кладется 1.
3. Определяется количество потоков для редьюсера. Максимальное количество - количество ядер. По итогу шаффлер создает отдельные хранилища для каждого редьюсера, которые содержат соответствующие мапы со словами из исходного хранилища. Слова будут распределены в алфавитном порядке
4. Каждый редьюсер обрабатывает соответсвующие ему мапы из хранилища, сокращает значения ключей (см.п.2 - вектора встречаний слов), сохраняет пары ключ - количество встречаний слова в соответсвующие файлы. Файлы не объединяются
```bash
.
└── MapReduce
    ├── CMakeLists.txt
    ├── build                               # сборочная папка
    ├── include                             # папка с библиотеками
    │   ├── CMakeLists.txt
    │   └── mapreduce
    │       ├── job.hpp                     # класс Job
    │       ├── map.hpp                     # класс Map
    │       ├── reduce.hpp                  # класс Reduce
    │       ├── shuffler.hpp                # класс Shuffle
    │       ├── splitter.hpp                # Split файлов  
    │       ├── multithread_vector.hpp      # потокобезопасный вектор
    │       └── storage.hpp                 # класс Storage
    ├── projects
    │   ├── CMakeLists.txt
    │   └── src
    │       ├── CMakeLists.txt
    │       └── main.cpp                    # Main, отсюда происходит запуск Job
    ├── testsuite
    │       ├── wordcount
    │       │      └── integrate.py           # интеграционный тест
    │       └── libsrc
    │           └── wordcount
    │               ├── CMakeLists.txt
    │               └── src
    │                   └── test.cpp        # unit-test
    └── data  
            ├── input                         # папка с исходным файлом и файлами, полученными после сплита
            └── output                        # папка с итоговыми файлами
  
  
  ```           
 Проект написан для запуска в многопоточном режиме с помощью std::thread.
 ## Запуск
 
 Проект собирается с помощью CMake. Для запуска программы:
 ```bash
 mkdir build
 cd build
 cmake ..
 make
 cd bin
 ./wordcount
 ```
 
 Для запуска юнит-тестов (из папки проекта):
  ```bash
  mkdir third-party
  cd third-party
  git clone https://github.com/catchorg/Catch2
  cd Catch2
  cmake -Bbuild -H. -DBUILD_TESTING=OFF
  sudo cmake --build build/ --target install
  cd ..
  cd ..
  cd build
  cmake ..
  cd bin
 ./test
 ```
 Для запуска интеграционного теста (из папки проекта)
  ```bash
  cd testsuite
  python3 integrate.py
 ```
