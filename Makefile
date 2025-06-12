NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -g
SRCDIR = src
OBJDIR = object
INCLUDES = -I ./includes

RM = rm -rf

SRCS = $(wildcard ./src/*.cpp) $(wildcard ./src/message/*.cpp)
OBJS = $(subst $(SRCDIR),$(OBJDIR),$(SRCS:.cpp=.o))


# Default target
all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	$(RM) $(OBJS)

# Clean everything
fclean: clean
	$(RM) $(NAME)

# Rebuild
re: fclean all

.PHONY: all clean fclean re