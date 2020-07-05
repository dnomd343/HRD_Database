CC := g++
RM = rm -rf
TAR = engine
OBJ = HRD_cal.o HRD_analy.o HRD_group.o HRD_statistic.o main.o

$(TAR):$(OBJ)
	$(CC) $(OBJ) -o $(TAR)
	$(RM) $(OBJ)

%.o:%.cpp
	$(CC) -O3 -c $^ -o $@

.PHONY:
clear:
	$(RM) $(OBJ) $(TAR)