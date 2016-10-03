NAME = gedprogram
SRC =  Family.cpp People.cpp gedcom_code.cpp
OBJ = $(SRC:.c=.o)
RM = rm -f

all: $(OBJ)
	g++ $(SRC) -o $(NAME)
clean:
	-$(RM) ../my/*~
	-$(RM) ../my/.o
	-$(RM) *.swp
	-$(RM) *.core
	-$(RM) *.o
	-$(RM) *~
	-$(RM) \#*
fclean: clean
	-$(RM) $(NAME)
re: fclean all
