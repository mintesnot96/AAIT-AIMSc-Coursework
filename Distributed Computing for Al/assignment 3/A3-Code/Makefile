include ./arch.gnu

app: cardiacsim_1D cardiacsim_2D cardiacsim_OpenMP

OBJECTS_1D = cardiacsim_1D.o
OBJECTS_2D = cardiacsim_2D.o
OBJECTS_1D_OMP = cardiacsim_OpenMP.o

cardiacsim_1D: $(OBJECTS_1D)
		$(C++LINK) $(LDFLAGS) -o $@ $(OBJECTS_1D) $(LDLIBS)

cardiacsim_2D: $(OBJECTS_2D)
		$(C++LINK) $(LDFLAGS) -o $@ $(OBJECTS_2D) $(LDLIBS)

cardiacsim_OpenMP: $(OBJECTS_1D_OMP)
		$(C++LINK) $(LDFLAGS) -o $@ $(OBJECTS_1D_OMP) $(LDLIBS)

.C.o:
		$(C++) $(C++FLAGS) -c $<

.c.o:
		$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o cardiacsim_1D cardiacsim_2D cardiacsim_OpenMP *~
	$(RM) core
