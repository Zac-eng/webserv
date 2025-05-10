NAME = webserv
CXX = c++
CXXFLAGS =-std=c++11 -g
SRCDIR = src
OBJDIR = object
INCLUDES = -I ./includes

RM = rm -rf

SRCS = $(wildcard ./src/*.cpp) $(wildcard ./src/message/*.cpp)
OBJS = $(subst $(SRCDIR),$(OBJDIR),$(SRCS:.cpp=.o))


all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re