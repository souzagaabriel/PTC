# Instituto Federal de Santa Catarina
## PTC29008

### Compilação do arquivo proto.cpp
>- Baixe os arquivos para sua pasta de trabalho e execute no terminal o comando $make proto
*	No arquivo proto.cpp é preciso definir a Serial que será utilizada
*	Exemplo: Serial rf("/dev/ttyUSB0", B9600);

### Tutorial de utilização da SerialEmu

#### Utilizando a serialemu:
>- Baixe e execute a serialemu conforme tutorial do Professor Marcelo Maia Sobral disponibilizado na pagina da disciplina na wiki, Serialemu disponivel [neste link](http://tele.sj.ifsc.edu.br/~msobral/ptc/Serialemu.zip)

#### Executando o programa
>- Após executado a serialemu, abra os arquivos A.ccp e B.cpp, e coloque o valor das portas gerado no campo `Serial rf(*digite*, B9600);`
>- Execute no terminal o comando *make*
>- Execute digitando `./A1` e `./B1` no terminal

##### Observações : 
>- Nesta versão da serialemu não está sendo utilizado *timeouts*



