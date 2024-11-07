NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
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
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re