CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

TARGET = RPN

SRCS = src/main.cpp \
		src/RPN.cpp

OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I ./src

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(TARGET)

re: fclean all
