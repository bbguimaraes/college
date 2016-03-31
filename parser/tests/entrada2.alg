algoritmo "Billy"
var
    x, i: inteiro
    v: vetor [0..5] de inteiro
inicio
    leia(x)

    se x > 5 entao
        v[0] <- x
    senao
        para i de 0 ate x faca
            v[i] <- x
        fimpara
    fimse
fimalgoritmo
