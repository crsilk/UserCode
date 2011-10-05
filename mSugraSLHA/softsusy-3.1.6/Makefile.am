bin_PROGRAMS = softsusy.x softpoint.x rpvsoftsusy.x

lib_LTLIBRARIES = libsoft.la

softsusy_x_SOURCES = main.cpp
softsusy_x_LDADD = libsoft.la

softpoint_x_SOURCES = softpoint.cpp 
softpoint_x_LDADD = libsoft.la

rpvsoftsusy_x_SOURCES = rpvmain.cpp
rpvsoftsusy_x_LDADD = libsoft.la

OUTPUTFILES = slha2Output outputTest rpvOutputTest \
lesHouchesOutput rpvHouchesOutput \
spsSLHAfiles/sps1a.out spsSLHAfiles/sps1b.out \
spsSLHAfiles/sps2.out spsSLHAfiles/sps3.out spsSLHAfiles/sps4.out \
spsSLHAfiles/sps5.out spsSLHAfiles/sps6.out spsSLHAfiles/sps7.out \
spsSLHAfiles/sps8.out spsSLHAfiles/sps9.out

INPUTFILES = slha2Input \
lesHouchesInput rpvHouchesInput \
spsSLHAfiles/sps1a.in spsSLHAfiles/sps1b.in \
spsSLHAfiles/sps2.in spsSLHAfiles/sps3.in spsSLHAfiles/sps4.in \
spsSLHAfiles/sps5.in spsSLHAfiles/sps6.in spsSLHAfiles/sps7.in \
spsSLHAfiles/sps8.in spsSLHAfiles/sps9.in

dist_pkgdata_DATA = $(INPUTFILES) $(OUTPUTFILES)
CLEANFILES = 

pkginclude_HEADERS = \
def.h main.h numerics.h \
softpars.h susy.h linalg.h \
physpars.h softpoint.h twoloophiggs.h \
lowe.h mycomplex.h rge.h softsusy.h \
utils.h rpvsoft.h rpvsusypars.h \
tensor.h rpvmain.h flavoursoft.h \
xpr-base.h xpr-matrix.h xpr-vector.h

libsoft_la_SOURCES = def.cpp linalg.cpp lowe.cpp numerics.cpp \
physpars.cpp rge.cpp softpars.cpp softsusy.cpp susy.cpp utils.cpp \
twoloophiggs.f tensor.cpp rpvsusypars.cpp rpvsoft.cpp flavoursoft.cpp
libsoft_la_LDFLAGS = $(FLIBS)

%Output : %Input softpoint.x
	./softpoint.x leshouches < $< > $@

outputTest: softsusy.x
	./softsusy.x > outputTest

rpvOutputTest: rpvsoftsusy.x
	./rpvsoftsusy.x > rpvOutputTest

spsSLHAfiles/%.out : spsSLHAfiles/%.in softpoint.x
	$(install_sh) -d spsSLHAfiles
	./softpoint.x leshouches < $< > $@


