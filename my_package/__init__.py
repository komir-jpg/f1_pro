import os
import sys
from cffi import FFI

ffi = FFI()

# Otteniamo il percorso della cartella dove si trova questo script
this_folder = os.path.dirname(os.path.abspath(__file__))

# 1. LETTURA AUTOMATICA DELL'HEADER
header_folder = os.path.join(this_folder, "api_python.h")

# Apriamo il file, leggiamo tutto il testo e lo passiamo a cdef
with open(header_folder, "r") as file_header:
    header_code = file_header.read()
    ffi.cdef(header_code)

lib_name = "libhello.so"
lib_path = os.path.join(this_folder, lib_name)
lib = ffi.dlopen(lib_path)

# 3. ESPORTAZIONE DIRETTA DELLA LIBRERIA
# Invece di creare funzioni Python personalizzate per ogni funzione C,
# rendiamo disponibile l'intero oggetto "lib" a chi usa il nostro pacchetto.
