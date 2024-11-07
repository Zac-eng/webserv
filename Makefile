NAME = webserv
CXX = c++
SRCDIR = src
OBJDIR = object
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
INCFLAG = -I$(SRCDIR)

RM = rm -rf

SRCS = $(wildcard ./src/*.cpp) $(wildcard ./src/message/*.cpp) $(wildcard ./src/cgi/*.cpp)
OBJS = $(subst $(SRCDIR),$(OBJDIR),$(SRCS:.cpp=.o))

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCFLAG) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCFLAG) $(OBJS) -o $@

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
