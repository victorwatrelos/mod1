CC= g++
FLAGS=-Wall -Wextra -Wno-deprecated-declarations -std=c++11 -I./includes/ -I./includes/GLFW -g
NAME=mod1 

SRC= main.cpp \
	 Top.cpp \
	 Platform.cpp \
	 clUtil.cpp \
	 OpenCL.cpp \
	 Device.cpp \
	 Task.cpp \
	 TaskParticleInGrid.cpp \
	 TaskApplyForces.cpp \
	 TaskAddConst.cpp \
	 TaskInitBuffer.cpp \
	 TaskInitParticle.cpp \
	 TaskCalcLambda.cpp \
	 TaskFindNeighbors.cpp \
	 TaskEndSim.cpp \
	 Matrix.cpp \
	 Mesh.cpp \
	 OpenGLMatrix.cpp \
	 OpenGLShader.cpp \
	 OpenGLManager.cpp \
	 OpenGLScene.cpp \
	 FakeCamera.cpp \
	 LookAtCamera.cpp \
	 FreeCamera.cpp \
	 Noise.cpp \
	 ProcMap.cpp \
	 vector.cpp

OBJ= $(SRC:.cpp=.o)

LIB=  -L./libs/ -lglad -lglfw3

FRWK=   -framework AGL \
		-framework Cocoa \
		-framework OpenGL \
		-framework IOKit \
		-framework CoreFoundation \
		-framework CoreVideo \
		-framework OpenCL

.PHONY: clean fclean re
.SILENT:

all:$(NAME) 

$(NAME): $(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LIB) $(FRWK) $(INC) $(FLAGS)
		echo "\t\xF0\x9F\x8F\x81   Compiling \033[35m$(NAME) \033[0mDONE!"

%.o: %.cpp
		echo "\t\xF0\x9F\x94\xA7   Building \033[34m $@ \033[0m" 
		$(CC) -c $(FLAGS) $(INC) -o $@ $<

clean:
		echo "\t\xF0\x9F\x92\xA3   Cleaning"
		rm -rf $(OBJ)

fclean: clean
		echo "\t\xF0\x9F\x9A\xBD   Full Clean"
		rm -rf $(NAME)

re: fclean all
