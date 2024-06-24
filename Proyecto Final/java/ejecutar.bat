@echo off
cls
javac -d ./bin src/def/*.java src/interfaz/*.java
java -cp bin def.Compilador
