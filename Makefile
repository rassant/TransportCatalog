	####################################################################################################
	# make -f MyMakefile для выбора конкретного make файла
	# для копирования файла в буфер обмена
	# cat ./main.cpp|xclip -i -selection clipboard
	# valgrind --leak-check=full ./a.out
  # strip ./a.out он удалит из программы всю отладочную информацию
	####################################################################################################

# CXXFLAGS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# CXXFLAGS += -gsplit-dwarf # при оптимизации мы можем увидеть <optimized out> вот что бы этого не было используем этот флаг. Минус он создает дополнительные файлы с информацией для отладки optimized.dwo

# CXXFLAGS += -DPARALLEL -ltbb -lpthread
# CXXFLAGS += -pg # для работы с параллейными алгоритмами -pg для профилировщика 
				# (замера времени работы функций)после компиляции появиться файл gmon.out  для создания рапорта:
				# gprof ./a.out gmon.out > report.txt
				# файл gmon.out появляется только после запуска и завершения программы

# CXXFLAGS += -ggdb3 # создает более крутую отладочную информацию
CXXFLAGS += -O3
# CXXFLAGS += -Og    # оптимизировать и сохранить отладочную информацию
CXXFLAGS += -std=c++2a 
CXXFLAGS += -pipe # не создавать промежуточные файлы о и s. Ускоряет время компиляции.

CXXFLAGS += -Wall -Wextra -Wpedantic -Werror -pedantic -pedantic-errors
CXXFLAGS += -Wcast-align                    # проверка что бы память была выровнена.
CXXFLAGS += -Wcast-qual                     # проверяет на правильное использование const например константный char (const char*) к указателю на не-константный char (char*)
CXXFLAGS += -Wconversion                    # предупреждать о потери точности
CXXFLAGS += -Wctor-dtor-privacy             # проверяет конструкторы и деструкторы их доступность и ло
CXXFLAGS += -Wduplicated-branches           # проверка дублирования ветвлений
CXXFLAGS += -Wduplicated-cond               # проверка дублирования условий
CXXFLAGS += -Werror                         # делает все предупреждения ошибками. Если есть предупреждение код не скомпилируется
CXXFLAGS += -Wfatal-errors                  # останавливает компиляцию после первой же ошибки. т.е выдаст 1 ошибку а не 10
CXXFLAGS += -Wfloat-equal                   # Предупреждает о проверке на равенство между двумя числами с плавающей точкой.
CXXFLAGS += -Wlogical-op                    # Предупреждает о подозрительных логических выражениях. Например, когда вместо побитового "И" поставили логическое "И"
CXXFLAGS += -Wnon-virtual-dtor              # Предупреждает о том, что у класса есть виртуальные функции-члены, но деструктор при этом не виртуальный
CXXFLAGS += -Wold-style-cast                # Предупреждает о приведении типов в стиле языка C
CXXFLAGS += -Woverloaded-virtual            # Предупреждает о попытке в классе-наследнике перегрузить виртуальную функцию базового класса (забыл добавить overload)
CXXFLAGS += -Wredundant-decls               # предупреждений о ненужных или избыточных объявлениях (декларациях) в программе.
CXXFLAGS += -Wshadow=compatible-local       # не даёт перекрыть локальную переменную другой локальной переменной при условии, что они имеют совместимые типы.
CXXFLAGS += -Wsign-compare                  # предупреждений при сравнении значений разных знаков
CXXFLAGS += -Wsign-conversion               # предотвратить большое количество неявных преобразований,
CXXFLAGS += -Wsign-promo                    # Предупреждает о неочевидном выборе перегруженной функции
CXXFLAGS += -Wzero-as-null-pointer-constant # Предупреждает об использовании целочисленного нуля вместо nullptr

CXXFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_LIBCPP_DEBUG=2 # для проверки выхода за границы массива, правильность работы итераторов
# CXXFLAGS += -fanalyzer  									# бывает ошибки при его работе. Выдает ошибку там где ее нет.

# санитайзеры для проверки работы с памятью
# CXXFLAGS += -fsanitize=address    # медленный и много жрет памяти. можем добавить  -static-libasan  если хотим статически связать AddressSanitizer
CXXFLAGS += -fno-omit-frame-pointer     # используется для отключения оптимизации
CXXFLAGS += -fno-optimize-sibling-calls # используется для отключения оптимизации родственных вызовов

#-------------------------------- дополнительно -------------------------------------------
# CXXFLAGS += -w # Запретить все предупреждающие сообщения.
# CXXFLAGS += -fsyntax-only#  # только проверка на синтактические ошибки и больше ничего.
# CXXFLAGS += -fmax-errors=n # ограничить вывод ошибок до n
# CXXFLAGS += -static указывает компилятору собрать все используемые библиотечные файлы в исполняемый файл.
# CXXFLAGS += 
# CXXFLAGS += 
# CXXFLAGS += 
# CXXFLAGS += 
# CXXFLAGS += 
# CXXFLAGS += 
#------------------------------------------------------------------------------------------

LDLIBS += -ltbb -lpthread # многопоточка
LDLIBS += -lgtest # тесты Google
SOURCES =  *.cpp

all: program
program: 
	@g++ $(CXXFLAGS) $(SOURCES) $(LDLIBS)
	@echo "Компиляция прошла успешна."

#----------------------------- gdb + valgring -------------------------------------------
# valgrind --vgdb=full --vgdb-error=0 ./a.out

# выдаст такое сообщение:
# ==2221211== Memcheck, a memory error detector
# ==2221211== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
# ==2221211== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
# ==2221211== Command: ./a.out
# ==2221211==
# ==2221211== (action at startup) vgdb me ...
# ==2221211==
# ==2221211== TO DEBUG THIS PROCESS USING GDB: start GDB like this
# ==2221211==   /path/to/gdb ./a.out
# ==2221211== and then give GDB the following command
# ==2221211==   target remote | /usr/bin/vgdb --pid=2221211    <--------------------------
# ==2221211== --pid is optional if only one valgrind process is running
# ==2221211==

# затем нам надо во втором окне терминала запустить как обычно
# gdb ./a.out
# а потом
# target remote | /usr/bin/vgdb --pid=222121 (эту строчку мы взяли выше)

# запускаем программу в gdb
# continue

# bt - показать стек

# frame 2 - показать проблемную строку.
#------------------------------------------------------------------------------------------
