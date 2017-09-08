# Projeto 1

#### Coisas para fazer:
> - Bateria de testes ( Envio de mensagens corrompidas )
> - Recepção de mensagens livres de erros ( CRC 16 bits)
[link auxiliar](https://tools.ietf.org/html/rfc1662#page-18)
> - Garantia de entrega: Stop and Wait(ARQ)
    Coloca cabeçalho: Tipo quadro, nº sequência, Id app, e passa para Enquadramento.
    Vocabulário ={DATA0, DATA1, ACK0, ACK1}
    Funções: *Bloqueantes
        `envia()`
        `recebe()`
        `handle()` *Máquina de estados

    Classes:
        Enquadramento------->Protocolo<----    ARQ
> - Vocabulário ={DATA0, DATA1, ACK0, ACK1}

> - Time out - Máquina de estados recepção
> - Implementar para arduino
> - Documentação PDF


##### Discussão

###### Ultima alteração
IMplementado o gen_CRC (Ainda não testado)
