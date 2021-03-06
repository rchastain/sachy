# Compilateur & options de base
CC = gcc
CFLAGS = -Wall -O3
 
# La liste des sources
SOURCES = main.c inic.c interf.c movegen.c value.c rules.c makemove.c search.c test.c sort.c book.c
# Ici on crée deux listes, une pour chaque architecture
# Elles sont créées en remplaçant l'extension `.c` de la liste de sources par `.o32`/`.o64`
OBJ32 = $(SOURCES:.c=.o32)
OBJ64 = $(SOURCES:.c=.o64)
 
# Première cible, par défaut on essaye de construire les deux exécutables
all: sachy32 sachy64 sachy
 
# Les cibles par architecture
# $@ est le nom de la cible, $^ est la totalité des dépendances
sachy32: $(OBJ32)
	$(CC) $(LDFLAGS) -m32 $(LDLIBS) -o $@ $^
 
sachy64: $(OBJ64)
	$(CC) $(LDFLAGS) -m64 $(LDLIBS) -o $@ $^
 
# Par défaut, sans indication on préfèrera la version 64 bits
sachy: sachy64
	cp $< $@
 
# On rajoute une dépendance vers `data.h` sur tous les fichiers objets
$(OBJ32): data.h
 
$(OBJ64): data.h
 
# Cibles inconditionnelles de maintenance
.phony: clean clean32 clean64
 
clean: clean32 clean64
	$(RM) sachy
 
clean32:
	$(RM) sachy32 $(OBJ32)
 
clean64:
	$(RM) sachy64 $(OBJ64)
 
# Les règles par défaut pour produire des objets 32 bits
# $< est le fichier source concerné par la compilation
%.o32: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -m32 -c -o $@ $<
 
# Idem pour une architecture 64 bits
%.o64: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -m64 -c -o $@ $<
