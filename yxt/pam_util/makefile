all:$(OBJDIR)/PamClass.o  $(OBJDIR)/login.o
.PHONY:all clean test
login_test:*.cpp
	g++ login_test.cpp login.cpp PamClass.cpp -o login_test -lpam
clean:
	-rm login_test *.o
$(OBJDIR)/%.o:%.cpp %.h
	g++ $< -c  -o $@ $(FLAGS)
test:login_test
